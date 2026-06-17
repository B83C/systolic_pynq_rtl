"""Test pw — isolate MAC (no quant) then with quant."""
from yunet_pynq import HwPointwise, find_quant_params
from pynq import Overlay, MMIO
import numpy as np, time

ol = Overlay("systolic.bit")
mmio    = MMIO(ol.ip_dict["sa_wrapper_axi_ctrl_0"]["phys_addr"], 0x1000)
mmio_ch = MMIO(ol.ip_dict["chlast_to_tiled_gath_0"]["phys_addr"], 0x1000)
mmio_q  = MMIO(ol.ip_dict["quantizer_wrapper_0"]["phys_addr"], 0x1000)
mmio_tc = MMIO(ol.ip_dict["tiled_to_chlast_gath_0"]["phys_addr"], 0x1000)

hw = HwPointwise(mmio, mmio_ch, mmio_q, mmio_tc, ol.axi_dma_0)
rng = np.random.default_rng(42)
w = rng.integers(-5,5,(16,16,1,1),dtype=np.int8)
bias = rng.integers(-100,100,16,dtype=np.int32)
a = rng.integers(-128,127,(320,320,16),dtype=np.int8)
hw.load_layer(w, bias, zp_w=0)

t0=time.perf_counter()
o=hw.run(a, zp_in=-128, mul_float=0.00429, zp_out=14)
dt=time.perf_counter()-t0

mq,sh=find_quant_params(0.00429)
act_i8=(a.reshape(-1,16).astype(np.int16)-(-128)).astype(np.int8)
acc=act_i8.astype(np.int32)@w[:,:,0,0].reshape(16,16).astype(np.int32).T+bias.astype(np.int32)
ref=np.clip(((acc*mq)>>sh).astype(np.int32)+14,-128,127).astype(np.int8).reshape(320,320,16)
d=np.max(np.abs(o.astype(np.int16)-ref.astype(np.int16)))
print(f"320x320 pw: {dt*1000:.2f}ms  maxdiff={d}  {'PASS' if d==0 else 'FAIL'}")
if d3>0:
    print(f"  hw[0,0,:8]={o3[0,0,:8]}")
    print(f"  ref[0,0,:8]={ref3[0,0,:8]}")
