import sys, time
sys.path.insert(0, "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual")
from pynq import Overlay, allocate, MMIO
import numpy as np

ol = Overlay("/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit")
mmio = MMIO(0x40000000, 0x1000)
dma_a = ol.axi_dma_0
dma_b = ol.axi_dma_1

size = 4
n = 4
out_words = size  # 4 words per beat (32-bit each -> 128-bit output)
in_words = 1      # 1 word per beat (32-bit -> 8-bit per element)

in_a = allocate(shape=(n * in_words,), dtype=np.uint32)
in_b = allocate(shape=(n * in_words,), dtype=np.uint32)
out = allocate(shape=(n * out_words,), dtype=np.uint32)

# A = ones(4,4)  -> packed as 0x01010101 per word
in_a[:] = np.uint32(0x01010101)
# B = 2*I(4) -> packed as [2,0,0,0], [0,2,0,0], ...
for i in range(n):
    val = np.uint32(2 << (i * 8))
    in_b[i] = val

# Configure SA: normal mode, loop_len=4
mmio.write(0x04, 0)  # MODE
mmio.write(0x08, n)  # LOOP

# Start DMA - recv first
dma_a.recvchannel.transfer(out)
dma_a.sendchannel.transfer(in_a)
dma_b.sendchannel.transfer(in_b)

print("Before SA start:")
print(f"  sendA sr: 0x{dma_a.mmio.read(4):x}")
print(f"  recv  sr: 0x{dma_a.mmio.read(0x34):x}")
print(f"  sendB sr: 0x{dma_b.mmio.read(4):x}")

# Start SA
mmio.write(0x00, 1)  # CTRL start

time.sleep(0.1)

print("\nAfter SA start:")
print(f"  sendA sr: 0x{dma_a.mmio.read(4):x}")
print(f"  recv  sr: 0x{dma_a.mmio.read(0x34):x}")
print(f"  sendB sr: 0x{dma_b.mmio.read(4):x}")

# Wait for SA done
while not (mmio.read(0x00) & 2):
    time.sleep(0.001)
print("\nSA done!")

print(f"\nFinal DMASR:")
print(f"  sendA sr: 0x{dma_a.mmio.read(4):x}")
print(f"  recv  sr: 0x{dma_a.mmio.read(0x34):x}")
print(f"  sendB sr: 0x{dma_b.mmio.read(4):x}")

# Read result
print("\nResult buffer out:")
for i in range(n * out_words):
    print(f"  out[{i}] = 0x{out[i]:08x} ({out[i]})")

result = np.zeros((n, size), dtype=np.uint32)
for i in range(n):
    for j in range(size):
        result[i, j] = out[i * out_words + j]
print("\nResult matrix:")
print(result)

in_a.freebuffer()
in_b.freebuffer()
out.freebuffer()
