"""Test multiple pw layer sizes for correctness."""
import numpy as np, time
from yunet_pynq import HwPointwise, sw_pointwise, find_quant_params
from pynq import Overlay, MMIO

ol = Overlay("systolic.bit")
mmio    = MMIO(ol.ip_dict["sa_wrapper_axi_ctrl_0"]["phys_addr"], 0x1000)
mmio_ch = MMIO(ol.ip_dict["chlast_to_tiled_gath_0"]["phys_addr"], 0x1000)
mmio_q  = MMIO(ol.ip_dict["quantizer_wrapper_0"]["phys_addr"], 0x1000)
mmio_tc = MMIO(ol.ip_dict["tiled_to_chlast_gath_0"]["phys_addr"], 0x1000)

hw = HwPointwise(mmio, mmio_ch, mmio_q, mmio_tc, ol.axi_dma_0)
rng = np.random.default_rng(123)

for Cout, Cin, H, W, mul, label in [
    (16,16,320,320,0.00429, "16->16 320x320"),
    (32,32,160,160,0.00546, "32->32 160x160"),
    (64,64,80,80,0.00212,  "64->64 80x80"),
    (64,16,80,80,0.00336,  "64->16 80x80"),
]:
    w=rng.integers(-5,5,(Cout,Cin,1,1),dtype=np.int8)
    b=rng.integers(-100,100,Cout,dtype=np.int32)
    a=rng.integers(-128,127,(H,W,Cin),dtype=np.int8)
    hw.load_layer(w,b,zp_w=0)
    t0=time.perf_counter()
    o=hw.run(a,zp_in=-128,mul_float=mul,zp_out=14)
    dt=time.perf_counter()-t0
    mq,sh=find_quant_params(mul)
    act=(a.reshape(-1,Cin).astype(np.int16)-(-128)).astype(np.int8)
    acc=act.astype(np.int32)@w[:,:,0,0].reshape(Cout,Cin).astype(np.int32).T+b
    ref=np.clip(((acc*mq)>>sh).astype(np.int32)+14,-128,127).astype(np.int8).reshape(H,W,Cout)
    d=np.max(np.abs(o.astype(np.int16)-ref.astype(np.int16)))
    print(f"{label}: {dt*1000:.1f}ms  maxdiff={d}")

print("\nDone")
