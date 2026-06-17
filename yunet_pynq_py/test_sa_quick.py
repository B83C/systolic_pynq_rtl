"""Quick test: does sa_pointwise still work?"""
import numpy as np, subprocess
from pynq import allocate, Overlay, MMIO
from systolic_regs import SystolicMap, SaReg, ChlastReg, QuantReg, TiledReg

ol = Overlay("systolic.bit")
m = SystolicMap()
mmio    = MMIO(ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr'], 0x1000)
mmio_ch = MMIO(ol.ip_dict['chlast_to_tiled_gath_0']['phys_addr'], 0x1000)
mmio_q  = MMIO(ol.ip_dict['quantizer_wrapper_0']['phys_addr'], 0x1000)
mmio_tc = MMIO(ol.ip_dict['tiled_to_chlast_gath_0']['phys_addr'], 0x1000)

rng = np.random.default_rng(42)

for Cout, Cin, N in [(16,16,8), (64,64,512), (16,16,102400)]:
    Cp = ((Cout+7)//8)*8; Kp = ((Cin+7)//8)*8
    Np = ((N+7)//8)*8
    w = rng.integers(-5,5,(Cout,Cin,1,1),dtype=np.int8)
    b = rng.integers(-100,100,Cout,dtype=np.int32)
    a = rng.integers(-128,127,(1,N,Cin),dtype=np.int8)
    
    ab=allocate(shape=(Cp*Kp,),dtype=np.int8)
    view=np.ascontiguousarray(w[:,:,0,0].reshape(Cp//8,8,Kp//8,8).transpose(0,2,1,3))
    ab[:]=view.reshape(-1)
    cb=allocate(shape=(Cp,),dtype=np.int64)
    c32=cb.view(np.int32).reshape(-1,2); c32[:,:]=0
    for i in range(Cout): c32[i,0]=np.int32(b[i])
    cb.sync_to_device()
    b_off=(a.reshape(-1,Cin).astype(np.int16)-(-128)).astype(np.int8)
    bb=allocate(shape=(Np*Kp,),dtype=np.int8); bb[:]=np.zeros((Np,Kp),dtype=np.int8).reshape(-1)
    bb[:N*Cin]=b_off.reshape(-1)
    ob=allocate(shape=(Np*Cp,),dtype=np.int8)
    
    mmio.write(SaReg.FB_CNT, Kp//8-1); mmio.write(SaReg.A_LOOP_END, Cp*Kp//8-1)
    mmio.write(SaReg.C_LOOP_END, Cp-1); mmio.write(SaReg.ZP_IN, 0)
    m.configure_chlast(mmio_ch, cfg_channels=Kp, repeat_cnt=Kp//8, bypass=0)
    m.configure_quantizer(mmio_q, mul_q=1, shift=0, zp_out=0)
    m.configure_tiled(mmio_tc, cfg_channels=Cp)
    
    r=subprocess.run(["./sa_pointwise",hex(ab.device_address),hex(cb.device_address),
        hex(bb.device_address),hex(ob.device_address),str(Cp*Kp),str(Cp*8),str(Np*Kp),str(Np*Cp)],
        capture_output=True,text=True,timeout=30)
    
    ref=(w[:,:,0,0].reshape(Cout,Cin).astype(np.int32).T @ b_off.reshape(N,Cin).T).T + b.reshape(1,Cout)
    hw=ob.view(np.int8).reshape(Np,Cp)[:N,:Cout]
    sw=np.clip(ref,-128,127).astype(np.int8)
    d=np.max(np.abs(hw.astype(np.int16)-sw.astype(np.int16)))
    print(f"{Cout}x{Cin} N={N}: {d} {'PASS' if d==0 else 'FAIL'}")
    ab.freebuffer();cb.freebuffer();bb.freebuffer();ob.freebuffer()
