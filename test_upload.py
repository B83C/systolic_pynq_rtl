import sys, time
sys.path.insert(0, "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual")
from systolic import SystolicArray
import numpy as np
from pynq import allocate

sa = SystolicArray(
    "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit",
    ctrl_base_addr=0x40000000
)

n = sa.size
A = np.ones((n, n), dtype=np.uint8)
B = np.eye(n, dtype=np.uint8) * 2

# Use driver's pack_rows
in_a = sa.pack_rows(A, sa._input_words_per_beat, 1)
in_b = sa.pack_rows(B, sa._input_words_per_beat, 1)
out = allocate(shape=(n * sa._output_words_per_beat,), dtype=np.uint32)

sa.set_mode(False, False)
sa.set_loop_len(n)

# Manual transfer like the old test_full
sa.dma_a.recvchannel.transfer(out)
sa.dma_a.sendchannel.transfer(in_a)
sa.dma_b.sendchannel.transfer(in_b)

sa.start()

for ch, name in [(sa.dma_a.sendchannel, "sendA"),
                  (sa.dma_b.sendchannel, "sendB"),
                  (sa.dma_a.recvchannel, "recv")]:
    off = ch._offset
    for _ in range(500):
        sr = ch._mmio.read(off + 4)
        if sr & 0x70:
            print(f"{name} error sr=0x{sr:x}")
            break
        if sr & 2:
            print(f"{name} done (sr=0x{sr:x})")
            break
        time.sleep(0.001)
    else:
        print(f"{name} timeout")

sa.wait_done()

result = sa.unpack_rows(out, n, sa.size, sa._output_words_per_beat)
print("Result:")
print(result)

expected = A @ B
print("Expected (A@B):")
print(expected)

if np.array_equal(result, expected):
    print("PASS")
else:
    print("FAIL")

in_a.freebuffer()
in_b.freebuffer()
out.freebuffer()
