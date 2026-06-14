"""SA driver abstraction — same API for sim (cocotb) and hardware (PYNQ)."""

import numpy as np

# ---------------------------------------------------------------------------
#  Register map (must match defs.svh)
# ---------------------------------------------------------------------------
REG_STATE        = 0x00
REG_STATUS       = 0x04
REG_C_LOAD       = 0x08
REG_FB_CNT       = 0x0C
REG_A_LOAD       = 0x10
REG_ACC_CNT      = 0x14
REG_A_LOOP_START = 0x18
REG_A_LOOP_END   = 0x1C
REG_C_LOOP_START = 0x20
REG_C_LOOP_END   = 0x24
REG_SIZE         = 0x28
REG_RST_INDEX    = 0x2C
REG_MUL_Q        = 0x30
REG_SHIFT        = 0x34
REG_ZP_OUT       = 0x38
REG_ZP_IN        = 0x3C

STATE_IDLE  = 0
STATE_LOAD_A = 1
STATE_LOAD_B = 2
STATE_LOAD_C = 3


class SADriver:
    """Abstract SA driver. Subclass for sim or hardware."""

    def __init__(self, size=4, dwi=8, dwo=8, a_depth=8, c_depth=4):
        self.size = size
        self.dwi = dwi
        self.dwo = dwo
        self.a_depth = a_depth
        self.c_depth = c_depth
        self._input_words = max(1, size * dwi // 32)
        self._output_words = max(1, size * dwo // 32)

    # -- subclasses must implement these -----------------------------------
    def reg_write(self, addr, value): raise NotImplementedError
    def reg_read(self, addr): raise NotImplementedError
    def stream_send(self, data): raise NotImplementedError
    def stream_recv(self, n_words): raise NotImplementedError

    # -- high-level helpers ------------------------------------------------
    @property
    def state(self):
        return self.reg_read(REG_STATE) & 0x3

    @property
    def idle(self):
        return self.state == STATE_IDLE

    def soft_reset(self):
        self.reg_write(REG_RST_INDEX, 0)

    def configure(self, fb_cnt=0, a_loop_start=0, a_loop_end=None,
                  c_loop_start=0, c_loop_end=None):
        self.reg_write(REG_FB_CNT, fb_cnt & 0xFF)
        self.reg_write(REG_A_LOOP_START, a_loop_start)
        if a_loop_end is None:
            a_loop_end = self.a_depth - 1
        self.reg_write(REG_A_LOOP_END, a_loop_end)
        self.reg_write(REG_C_LOOP_START, c_loop_start)
        if c_loop_end is None:
            c_loop_end = self.c_depth - 1
        self.reg_write(REG_C_LOOP_END, c_loop_end)

    # -- pack / unpack helpers --------------------------------------------
    @staticmethod
    def pack_rows(rows, words_per_beat):
        n, s = rows.shape
        buf = np.zeros(n * words_per_beat, dtype=np.uint32)
        for i in range(0, s, 4):
            col = np.uint32(np.int32(rows[:, i].astype(np.int8)) & 0xFF)
            for j in range(1, min(4, s - i)):
                col |= np.uint32(np.int32(rows[:, i + j].astype(np.int8)) & 0xFF) << (8 * j)
            beat = i // 4
            buf[beat::words_per_beat] = col
        return buf

    @staticmethod
    def unpack_rows(buf, n_rows, size, words_per_beat, dtype=np.int32):
        n_cols = min(size, words_per_beat * 4)
        out = np.zeros((n_rows, n_cols), dtype=dtype)
        for beat in range(words_per_beat):
            col = buf[beat::words_per_beat]
            base = beat * 4
            for j in range(4):
                if base + j < size:
                    out[:, base + j] = (col >> (8 * j)) & 0xFF
        # sign extend from int8
        out = np.where(out > 127, out - 256, out).astype(dtype)
        return out

    # -- quant helpers -----------------------------------------------------
    def set_quant(self, mul_q, shift, zp_out, zp_in=0):
        self.reg_write(REG_MUL_Q, mul_q)
        self.reg_write(REG_SHIFT, shift)
        self.reg_write(REG_ZP_OUT, np.int8(zp_out) & 0xFF)
        self.reg_write(REG_ZP_IN, np.int8(zp_in) & 0xFF)

    # -- compute ----------------------------------------------------------
    def compute(self, A, B, C=None, fb_cnt=0):
        n = A.shape[0]
        assert A.shape == B.shape == (n, self.size)
        self.soft_reset()
        self.configure(fb_cnt=fb_cnt)

        in_a = self.pack_rows(A, self._input_words)
        in_b = self.pack_rows(B, self._input_words)

        if C is not None:
            in_c = np.array(C, dtype=np.uint32)
            self.reg_write(REG_C_LOAD, 0)
            self.stream_send(in_c)

        self.reg_write(REG_A_LOAD, 0)
        self.stream_send(in_a)

        out = self.stream_recv(n * self._output_words)
        result = self.unpack_rows(out, n, self.size, self._output_words)
        return result
