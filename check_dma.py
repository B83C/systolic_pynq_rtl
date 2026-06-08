import sys, time
sys.path.insert(0, "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual")
from systolic import SystolicArray
from pynq import allocate
import numpy as np

sa = SystolicArray(
    "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit",
    ctrl_base_addr=0x40000000
)

n = 4
wpbi = sa._input_words_per_beat
in_a = allocate(shape=(n * wpbi,), dtype=np.uint32)
in_b = allocate(shape=(n * wpbi,), dtype=np.uint32)
out = allocate(shape=(n * sa._output_words_per_beat,), dtype=np.uint32)

in_a[:] = 0x01010101
in_b[:] = 0x02000000

sa.set_mode(False, False)
sa.set_loop_len(n)

print("After init:")
print("  sendA sr:", hex(sa.dma_a.mmio.read(4)))
print("  recv sr:", hex(sa.dma_a.mmio.read(0x34)))
print("  sendB sr:", hex(sa.dma_b.mmio.read(4)))

sa.dma_a.recvchannel.transfer(out)
sa.dma_a.sendchannel.transfer(in_a)
sa.dma_b.sendchannel.transfer(in_b)

print("After transfer:")
print("  sendA sr:", hex(sa.dma_a.mmio.read(4)))
print("  recv sr:", hex(sa.dma_a.mmio.read(0x34)))
print("  sendB sr:", hex(sa.dma_b.mmio.read(4)))
print("  sendA sa_low:", hex(sa.dma_a.mmio.read(0x18)))
print("  sendA sa_hi:", hex(sa.dma_a.mmio.read(0x1C)))
print("  sendA len:", hex(sa.dma_a.mmio.read(0x28)))
print("  recv sa_low:", hex(sa.dma_a.mmio.read(0x48)))
print("  recv len:", hex(sa.dma_a.mmio.read(0x58)))

sa.start()
time.sleep(0.1)

print("After sa.start():")
print("  sendA sr:", hex(sa.dma_a.mmio.read(4)))
print("  recv sr:", hex(sa.dma_a.mmio.read(0x34)))
print("  sendB sr:", hex(sa.dma_b.mmio.read(4)))
print("  SA done:", sa.done())
print("  SA run:", sa.running())

# Poll DMASR over time
print("\nPolling sendA DMASR:")
for i in range(10):
    sr = sa.dma_a.mmio.read(4)
    print(f"  t={i*10}ms sr=0x{sr:x}")
    if sr & 2:
        print("  -> idle!")
        break
    time.sleep(0.01)
else:
    print("  -> never idle!")

print("\nPolling recv DMASR:")
for i in range(10):
    sr = sa.dma_a.mmio.read(0x34)
    print(f"  t={i*10}ms sr=0x{sr:x}")
    if sr & 2:
        print("  -> idle!")
        break
    time.sleep(0.01)
else:
    print("  -> never idle!")

in_a.freebuffer()
in_b.freebuffer()
out.freebuffer()
