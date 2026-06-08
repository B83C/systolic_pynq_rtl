import sys
sys.path.insert(0, "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual")
from pynq import Overlay
import json

ol = Overlay("/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit")
dma0 = ol.axi_dma_0
print("=== DMA0 description ===")
print(json.dumps(dma0.description, indent=2, default=str))
