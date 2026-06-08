from pynq import Overlay, allocate, MMIO
import numpy as np
import time


class SystolicArray:
    """PYNQ driver for the systolic-array AXI wrapper.

    Expects a block design containing:
      - ``sa_wrapper_axi_ctrl`` (AXI-Lite control)
      - ``axi_dma_0``   MM2S →   s_axis_A,  S2MM ← m_axis (result)
      - ``axi_dma_1``   MM2S →   s_axis_B

    Adjust DMA names and base address to match your BD.
    """

    def __init__(self, bitstream, ctrl_base_addr=0x43C00000):
        self.ol = Overlay(bitstream)
        self.mmio = MMIO(ctrl_base_addr, 0x1000)
        self.dma_a = self.ol.axi_dma_0
        self.dma_b = self.ol.axi_dma_1

        # Read params from hardware (SIZE=8, DWI=8, DWO=32)
        self.size = 8
        self.dwi = 8
        self.dwo = 32
        self._input_words_per_beat = self.size * self.dwi // 32    # 2
        self._output_words_per_beat = self.size * self.dwo // 32   # 8

    # -----------------------------------------------------------------
    #  Low-level register access
    # -----------------------------------------------------------------

    REG_CTRL = 0x00
    REG_MODE = 0x04
    REG_LOOP = 0x08

    _MODE_A_REUSE = 0x01
    _MODE_B_REUSE = 0x04

    def reg_write(self, addr, value):
        self.mmio.write(addr, value)

    def reg_read(self, addr):
        return self.mmio.read(addr)

    # -----------------------------------------------------------------
    #  Mode / control helpers
    # -----------------------------------------------------------------

    @property
    def running(self):
        return bool(self.reg_read(self.REG_CTRL) & 0x4)

    @property
    def done(self):
        return bool(self.reg_read(self.REG_CTRL) & 0x2)

    def start(self):
        self.reg_write(self.REG_CTRL, 1)

    def set_mode(self, a_reuse=False, b_reuse=False):
        v = 0
        if a_reuse:
            v |= self._MODE_A_REUSE
        if b_reuse:
            v |= self._MODE_B_REUSE
        self.reg_write(self.REG_MODE, v)

    def set_loop_len(self, n):
        """Set A recirculation length ``n`` (1 … 3*SIZE)."""
        self.reg_write(self.REG_LOOP, n & 0xFF)

    def wait_done(self, poll_us=100):
        import time
        while not self.done:
            time.sleep(poll_us / 1e6)

    # -----------------------------------------------------------------
    #  Data packing  (uint8 row ↔ packed AXI beats)
    # -----------------------------------------------------------------

    @staticmethod
    def pack_rows(rows, words_per_beat, elem_bytes):
        """Pack ``rows`` (N, SIZE) into a 1-D uint32 DMA buffer.

        *words_per_beat* = SIZE * elem_bytes / 4  (1 for 32-bit stream,
        4 for 128-bit stream, etc.).
        """
        n, s = rows.shape
        buf = allocate(shape=(n * words_per_beat,), dtype=np.uint32)
        if elem_bytes == 1:          # uint8  → pack 4 per uint32
            for i in range(0, s, 4):
                col = np.uint32(rows[:, i])
                for j in range(1, min(4, s - i)):
                    col |= np.uint32(rows[:, i + j]) << (8 * j)
                beat = i // 4          # which uint32 word within the beat
                buf[beat::words_per_beat] = col
        elif elem_bytes == 4:        # already uint32
            for i in range(s):
                buf[i::words_per_beat] = np.uint32(rows[:, i])
        return buf

    @staticmethod
    def unpack_rows(buf, n_rows, size, words_per_beat, dtype=np.uint32):
        """Reverse of pack_rows."""
        out = np.zeros((n_rows, size), dtype=dtype)
        if dtype == np.uint32:
            for i in range(size):
                out[:, i] = buf[i::words_per_beat]
        return out

    # -----------------------------------------------------------------
    #  Full compute
    # -----------------------------------------------------------------

    def compute(self, A, B, a_reuse=False, b_reuse=False, loop_len=None):
        """Run a complete SA transaction.

        Parameters
        ----------
        A, B : ndarray (N, SIZE)  uint8
            Input matrices (N rows of SIZE elements each).
        a_reuse, b_reuse : bool
        loop_len : int or None
            A recirculation length (default: ``A.shape[0]``).

        Returns
        -------
        result : ndarray (N, SIZE)  uint32
        """
        n = A.shape[0]
        assert A.shape == B.shape == (n, self.size)
        if loop_len is None:
            loop_len = n

        # Allocate DMA buffers
        in_a = self.pack_rows(A, self._input_words_per_beat, 1)
        in_b = self.pack_rows(B, self._input_words_per_beat, 1)
        out = allocate(shape=(n * self._output_words_per_beat,), dtype=np.uint32)

        # Configure SA
        self.set_mode(a_reuse, b_reuse)
        self.set_loop_len(loop_len)

        # Start DMA transfers (recv first so S2MM is ready for SA output)
        self.dma_a.recvchannel.transfer(out)
        self.dma_a.sendchannel.transfer(in_a)
        self.dma_b.sendchannel.transfer(in_b)

        # Start computation
        self.start()

        # Wait for DMA completion
        for ch in [self.dma_a.sendchannel, self.dma_b.sendchannel, self.dma_a.recvchannel]:
            off = ch._offset
            for _ in range(500):
                sr = ch._mmio.read(off + 4)
                if sr & 0x70:
                    raise RuntimeError(f"DMA error sr=0x{sr:x}")
                if sr & 2:
                    break
                time.sleep(0.001)
            else:
                raise RuntimeError("DMA timeout")
            if not ch._flush_before:
                ch._active_buffer.invalidate()
            ch._active_buffer = None
        self.wait_done()

        # Unpack result
        result = self.unpack_rows(out, n, self.size, self._output_words_per_beat)
        in_a.freebuffer()
        in_b.freebuffer()
        out.freebuffer()
        return result
