from pynq import Overlay, allocate, MMIO
import numpy as np
import time


class SystolicArray:
    """PYNQ driver for the systolic-array AXI wrapper.

    Block design:
      - ``sa_wrapper_axi_ctrl``  (AXI-Lite control + AXI-Stream data)
      - ``axi_dma_0``   MM2S → s_axis_B (A/C/B),  S2MM ← m_axis (result)
      - ``axi_interconnect``  PS → AXI-Lite / DMA regs
      - ``idle`` GPIO / interrupt (optional)

    All input (A, C, B) flows through ``s_axis_B_tdata``.  The wrapper
    routes it based on the internal state (LOAD_A / LOAD_C / LOAD_B).
    """

    # ------------------------------------------------------------------
    #  Register map (AXI-Lite, 4-byte aligned, 6-bit addresses)
    # ------------------------------------------------------------------
    REG_STATE = 0x00  # RO  [1:0] state (0=IDLE, 1=LOAD_A, 2=LOAD_B, 3=LOAD_C)
    REG_STATUS = 0x04  # RO  [0] b_underflow (clear-on-read)
    REG_C_LOAD = 0x08  # WO  write to trigger C_LOAD
    REG_FB_CNT = 0x0C  # RW  accumulation group size (0=off)
    REG_A_LOAD = 0x10  # WO  write to trigger A_LOAD
    REG_ACC_OUT = 0x14  # RW  [0] enable output during accumulation
    REG_A_LOOP_START = 0x18  # RW  A ring start index
    REG_A_LOOP_END = 0x1C  # RW  A ring end index
    REG_C_LOOP_START = 0x20  # RW  C ring start index
    REG_C_LOOP_END = 0x24  # RW  C ring end index
    REG_RST_INDEX = 0x2C  # WO  soft-reset indices and state

    # State enum (from defs.svh)
    STATE_IDLE = 0
    STATE_LOAD_A = 1
    STATE_LOAD_B = 2
    STATE_LOAD_C = 3

    def __init__(
        self,
        bitstream=None,
        ctrl_base_addr=0x40000000,
        size=8,
        dwi=8,
        dwo=32,
        a_depth=8,
        c_depth=8,
    ):
        self.size = size
        self.dwi = dwi
        self.dwo = dwo
        self.a_depth = a_depth
        self.c_depth = c_depth
        self._input_words_per_beat = self.size * self.dwi // 32
        self._output_words_per_beat = self.size * self.dwo // 32

        if bitstream is not None:
            self.ol = Overlay(bitstream, download=False)
            self.dma = self.ol.axi_dma_0
        else:
            self.ol = None
            self.dma = None

        self.mmio = MMIO(ctrl_base_addr, 0x1000)

    # ------------------------------------------------------------------
    #  Low-level register I/O
    # ------------------------------------------------------------------
    def reg_write(self, addr, value):
        self.mmio.write(addr, value)

    def reg_read(self, addr):
        return self.mmio.read(addr)

    @property
    def state(self):
        return self.reg_read(self.REG_STATE) & 0x3

    @property
    def idle(self):
        return self.state == self.STATE_IDLE

    @property
    def b_underflow(self):
        return bool(self.reg_read(self.REG_STATUS) & 0x1)

    def wait_idle(self, poll_us=100):
        while not self.idle:
            time.sleep(poll_us / 1e6)

    def soft_reset(self):
        """Reset internal state (ring pointers, pending flags, pipeline)
        without touching configuration registers."""
        self.reg_write(self.REG_RST_INDEX, 0)

    # ------------------------------------------------------------------
    #  Configuration
    # ------------------------------------------------------------------
    def configure(
        self,
        fb_cnt=0,
        acc_out=True,
        a_loop_start=0,
        a_loop_end=None,
        c_loop_start=0,
        c_loop_end=None,
    ):
        """Write all config registers. Call while IDLE."""
        self.reg_write(self.REG_FB_CNT, fb_cnt & 0xFF)
        self.reg_write(self.REG_ACC_OUT, 1 if acc_out else 0)
        self.reg_write(self.REG_A_LOOP_START, a_loop_start)
        if a_loop_end is None:
            a_loop_end = self.a_depth - 1
        self.reg_write(self.REG_A_LOOP_END, a_loop_end)
        self.reg_write(self.REG_C_LOOP_START, c_loop_start)
        if c_loop_end is None:
            c_loop_end = self.c_depth - 1
        self.reg_write(self.REG_C_LOOP_END, c_loop_end)

    # ------------------------------------------------------------------
    #  Data packing — all input via single MM2S channel
    # ------------------------------------------------------------------
    @staticmethod
    def pack_rows(rows, words_per_beat, elem_bytes=1):
        """Pack ``rows`` (N×SIZE) into a 1-D uint32 DMA buffer."""
        n, s = rows.shape
        buf = allocate(shape=(n * words_per_beat,), dtype=np.uint32)
        if elem_bytes == 1:  # uint8 → pack 4 per uint32
            for i in range(0, s, 4):
                col = np.uint32(rows[:, i].astype(np.uint8))
                for j in range(1, min(4, s - i)):
                    col |= np.uint32(rows[:, i + j].astype(np.uint8)) << (8 * j)
                beat = i // 4
                buf[beat::words_per_beat] = col
        elif elem_bytes == 4:
            for i in range(s):
                buf[i::words_per_beat] = np.uint32(rows[:, i])
        return buf

    @staticmethod
    def pack_c_values(c_vals):
        """Pack C values (N,) → uint32 DMA buffer.
        Each C value broadcast to all columns of one output row."""
        n = len(c_vals)
        buf = allocate(shape=(n,), dtype=np.uint32)
        for i in range(n):
            buf[i] = np.uint32(c_vals[i])
        return buf

    @staticmethod
    def unpack_rows(buf, n_rows, size, words_per_beat, dtype=np.uint32):
        out = np.zeros((n_rows, size), dtype=dtype)
        if dtype == np.uint32:
            for i in range(size):
                out[:, i] = buf[i::words_per_beat]
        return out

    def _dma_send(self, buf):
        """Send one buffer through MM2S, wait for completion."""
        self.dma.sendchannel.start()
        self.dma.sendchannel.transfer(buf)
        self.dma.sendchannel.wait()

    def _dma_recv(self, buf):
        """Prepare S2MM receive buffer, wait for completion."""
        self.dma.recvchannel.start()
        self.dma.recvchannel.transfer(buf)
        self.dma.recvchannel.wait()

    # ------------------------------------------------------------------
    #  Compute  —  C + A × B
    # ------------------------------------------------------------------
    def compute(self, A, B, C=None, fb_cnt=0, acc_out=True):
        """Run one SA transaction:  C + A × B.

        Parameters
        ----------
        A : ndarray (N, SIZE)  uint8 (signed at PE)
        B : ndarray (N, SIZE)  uint8 (signed at PE)
        C : ndarray (N,) uint32 or None
            One 32-bit C per output row.  None → C = 0.
        fb_cnt : int   accumulation group size (0 = off)
        acc_out : bool  show outputs during accumulation

        Returns
        -------
        result : ndarray (N, SIZE) uint32
        """
        n = A.shape[0]
        assert A.shape == B.shape == (n, self.size)
        if C is not None:
            assert len(C) == n

        self.soft_reset()
        self.configure(fb_cnt=fb_cnt, acc_out=acc_out)

        in_a = self.pack_rows(A, self._input_words_per_beat, 1)
        in_b = self.pack_rows(B, self._input_words_per_beat, 1)
        out = allocate(shape=(n * self._output_words_per_beat,), dtype=np.uint32)

        if C is not None:
            in_c = self.pack_c_values(C)
            self.reg_write(self.REG_C_LOAD, 1)
            self._dma_send(in_c)
            in_c.freebuffer()

        # Load A ring
        self.reg_write(self.REG_A_LOAD, 1)
        self._dma_send(in_a)

        # Stream B (computation starts) + receive output
        self.dma.recvchannel.start()
        self.dma.recvchannel.transfer(out)
        self._dma_send(in_b)
        self.dma.recvchannel.wait()

        self.wait_idle()

        result = self.unpack_rows(out, n, self.size, self._output_words_per_beat)
        in_a.freebuffer()
        in_b.freebuffer()
        out.freebuffer()
        return result

    def compute_acc(self, A, B_matrices, C=None, fb_cnt=0):
        """Accumulate multiple B matrices against one A.

        Parameters
        ----------
        A : ndarray (SIZE, SIZE) uint8
        B_matrices : list of ndarray (SIZE, SIZE) uint8
        C : ndarray (SIZE,) uint32 or None
        fb_cnt : int  accumulation group size

        Returns
        -------
        result : ndarray (M, SIZE) uint32  where M = len(B_matrices) × SIZE
        """
        m = len(B_matrices)
        self.soft_reset()
        self.configure(fb_cnt=fb_cnt, acc_out=True)

        in_a = self.pack_rows(A, self._input_words_per_beat, 1)
        out = allocate(
            shape=(m * self.size * self._output_words_per_beat,), dtype=np.uint32
        )

        if C is not None:
            in_c = self.pack_c_values(C)
            self.reg_write(self.REG_C_LOAD, 0)
            self._dma_send(in_c)
            in_c.freebuffer()

        self.reg_write(self.REG_A_LOAD, 0)
        self._dma_send(in_a)

        self.dma.recvchannel.start()
        self.dma.recvchannel.transfer(out)

        for i, B in enumerate(B_matrices):
            in_b = self.pack_rows(B, self._input_words_per_beat, 1)
            self._dma_send(in_b)
            in_b.freebuffer()

        self.dma.recvchannel.wait()
        self.wait_idle()

        result = self.unpack_rows(
            out, m * self.size, self.size, self._output_words_per_beat
        )
        in_a.freebuffer()
        out.freebuffer()
        return result

    # ------------------------------------------------------------------
    #  On-board self-test
    # ------------------------------------------------------------------
    def test_all(self):
        """Quick sanity check — A×B1 with SIZE=4 (zero-padded to SIZE=8)."""
        A = np.array(
            [
                [10, 11, 12, 13, 0, 0, 0, 0],
                [11, 12, 13, 14, 0, 0, 0, 0],
                [12, 13, 14, 15, 0, 0, 0, 0],
                [13, 14, 15, 16, 0, 0, 0, 0],
            ],
            dtype=np.uint8,
        )
        B1 = np.array(
            [
                [1, 2, 3, 4, 0, 0, 0, 0],
                [5, 6, 7, 8, 0, 0, 0, 0],
                [9, 10, 11, 12, 0, 0, 0, 0],
                [13, 14, 15, 16, 0, 0, 0, 0],
            ],
            dtype=np.uint8,
        )

        if self.size == 8:
            A = np.pad(A, ((0, 4), (0, 0)), "constant")
            B1 = np.pad(B1, ((0, 4), (0, 0)), "constant")

        result = self.compute(A, B1)
        exp = np.array(
            [
                [342, 388, 434, 480, 0, 0, 0, 0],
                [370, 420, 470, 520, 0, 0, 0, 0],
                [398, 452, 506, 560, 0, 0, 0, 0],
                [426, 484, 542, 600, 0, 0, 0, 0],
            ],
            dtype=np.uint32,
        )
        ok = np.allclose(result[:4, :4], exp[:4, :4])
        print("TEST 1  A×B1:  " + ("PASS" if ok else "FAIL"))
        if not ok:
            print("  got:\n", result[:4, :4])
            print("  exp:\n", exp[:4, :4])
        return ok
