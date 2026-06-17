"""Benchmark: C++ SA pointwise vs Python vs SW."""
import numpy as np, time, subprocess
from pynq import Overlay, allocate, MMIO
from systolic_regs import SystolicMap, SaReg, ChlastReg, QuantReg, TiledReg

ol = Overlay("systolic.bit")
dma = ol.axi_dma_0
m = SystolicMap()
mmio    = MMIO(ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr'], 0x1000)
mmio_ch = MMIO(ol.ip_dict['chlast_to_tiled_gath_0']['phys_addr'], 0x1000)
mmio_q  = MMIO(ol.ip_dict['quantizer_wrapper_0']['phys_addr'], 0x1000)
mmio_tc = MMIO(ol.ip_dict['tiled_to_chlast_gath_0']['phys_addr'], 0x1000)

T = 8

def bench(Co, Ci, N, use_cpp=True):
    rng = np.random.default_rng(42)
    weight = rng.integers(-5, 5, (Co, Ci, 1, 1), dtype=np.int8)
    bias = rng.integers(-100, 100, Co, dtype=np.int32)
    act = rng.integers(-128, 127, (1, N, Ci), dtype=np.int8)

    Cp = ((Co + T - 1) // T) * T
    Kp = ((Ci + T - 1) // T) * T
    Np = ((N + T - 1) // T) * T

    # Pre-process A
    w_2d = weight.reshape(Co, Ci)
    w_pad = np.zeros((Cp, Kp), dtype=np.int8)
    w_pad[:Co, :Ci] = w_2d
    A_bm = np.ascontiguousarray(
        w_pad.reshape(Cp//T, T, Kp//T, T).transpose(0, 2, 1, 3))

    ab = allocate(shape=(Cp*Kp,), dtype=np.int8); ab[:] = A_bm.reshape(-1)
    b_pad = np.zeros((Np, Kp), dtype=np.int8)
    b_pad[:N, :Ci] = act.reshape(N, Ci)
    bb = allocate(shape=(Np*Kp,), dtype=np.int8); bb[:] = b_pad.reshape(-1)
    ob = allocate(shape=(Np*Cp,), dtype=np.int8)

    fb_cnt = Kp // T - 1
    a_le = Cp * Kp // T - 1
    mmio.write(SaReg.FB_CNT, fb_cnt); mmio.write(SaReg.A_LOOP_END, a_le)
    mmio.write(SaReg.C_LOOP_END, Cp - 1); mmio.write(SaReg.ZP_IN, 0)
    m.configure_chlast(mmio_ch, cfg_channels=Kp, repeat_cnt=Kp//T, bypass=0)
    m.configure_quantizer(mmio_q, mul_q=1, shift=0, zp_out=0)
    m.configure_tiled(mmio_tc, cfg_channels=Cp)

    # A_LOAD + C_LOAD once
    mmio.write(SaReg.A_LOAD, 1)
    mmio_ch.write(ChlastReg.BYPASS, 1)
    dma.sendchannel.start(); dma.sendchannel.transfer(ab); dma.sendchannel.wait()
    mmio_ch.write(ChlastReg.BYPASS, 0)

    cb = allocate(shape=(Cp,), dtype=np.int64)
    c32 = cb.view(np.int32).reshape(-1,2); c32[:,:] = 0
    for i in range(Co): c32[i,0] = np.int32(bias[i])
    cb.sync_to_device()
    mmio.write(SaReg.C_LOAD, 1)
    mmio_ch.write(ChlastReg.BYPASS, 1)
    dma.sendchannel.start(); dma.sendchannel.transfer(cb); dma.sendchannel.wait()
    mmio_ch.write(ChlastReg.BYPASS, 0)

    if use_cpp:
        in_pa = bb.device_address
        out_pa = ob.device_address
        nB = Np * Kp
        nO = Np * Cp
        subprocess.run(["./sa_pointwise", "0", hex(in_pa), hex(out_pa), "0", str(nB), str(nO)],
                       capture_output=True, timeout=30)  # warmup
        cpp_times = []
        for _ in range(5):
            r = subprocess.run(
                ["./sa_pointwise", "0", hex(in_pa), hex(out_pa), "0", str(nB), str(nO)],
                capture_output=True, text=True, timeout=30)
            cpp_times.append(float(r.stdout.strip()))
        t = np.median(cpp_times)
    else:
        py_times = []
        for _ in range(3):
            t0 = time.perf_counter()
            dma.recvchannel.start(); dma.sendchannel.start()
            dma.recvchannel.transfer(ob); dma.sendchannel.transfer(bb)
            py_times.append(time.perf_counter() - t0)
        t = np.median(py_times)

    hw = ob.reshape(Np, Cp)[:N, :Co].copy()
    sw = (act.reshape(N,Ci).astype(np.int32) @ weight.reshape(Co,Ci).astype(np.int32).T
          + bias).clip(-128,127).astype(np.int8)
    d = np.max(np.abs(hw.astype(np.int16)-sw.astype(np.int16)))

    ab.freebuffer(); bb.freebuffer(); ob.freebuffer(); cb.freebuffer()
    return t, d

# Benchmark
sizes = [(16,16,8), (16,16,64), (16,16,256), (16,16,512),
         (64,64,8), (64,64,64), (64,64,128), (64,64,256), (64,64,512)]

print(f"{'Co':>3} {'Ci':>3} {'N':>5} | {'C++(ms)':>8} {'Py(ms)':>8} {'SpdUp':>6} {'Diff':>5}")
print("-"*42)
for Co, Ci, N in sizes:
    tc, dc = bench(Co, Ci, N, use_cpp=True)
    tp, dp = bench(Co, Ci, N, use_cpp=False)
    spd = tp/tc if tc > 0 else float('inf')
    print(f"{Co:3d} {Ci:3d} {N:5d} | {tc*1000:8.4f} {tp*1000:8.4f} {spd:5.1f}x {dc:4d}")
