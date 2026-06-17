"""ctypes wrapper for libhwynet.so — direct C++ calls, no subprocess overhead."""
import ctypes, numpy as np

_lib = None

def _get_lib():
    global _lib
    if _lib is None:
        _lib = ctypes.CDLL('./libhwynet.so')
        _lib.hw_init()
    return _lib

def hw_dw3x3(ib, wb, bb, ob, H, W, C, zi, zw, mq, sh, zo):
    """NEON depthwise via ctypes. ib,wb,bb,ob are allocated buffers."""
    lib = _get_lib()
    lib.hw_dw3x3(
        ctypes.c_uint64(ib.device_address), ctypes.c_uint64(wb.device_address),
        ctypes.c_uint64(bb.device_address), ctypes.c_uint64(ob.device_address),
        ctypes.c_int(H), ctypes.c_int(W), ctypes.c_int(C),
        ctypes.c_int(zi), ctypes.c_int(zw),
        ctypes.c_int(mq), ctypes.c_int(sh), ctypes.c_int(zo))

def hw_conv3x3(ib, wb, bb, ob, H, W, Ci, Co, zi, zw, mq, sh, zo):
    """NEON conv3x3 via ctypes."""
    lib = _get_lib()
    lib.hw_conv3x3(
        ctypes.c_uint64(ib.device_address), ctypes.c_uint64(wb.device_address),
        ctypes.c_uint64(bb.device_address), ctypes.c_uint64(ob.device_address),
        ctypes.c_int(H), ctypes.c_int(W),
        ctypes.c_int(Ci), ctypes.c_int(Co),
        ctypes.c_int(zi), ctypes.c_int(zw),
        ctypes.c_int(mq), ctypes.c_int(sh), ctypes.c_int(zo))

def hw_close():
    global _lib
    if _lib is not None:
        _lib.hw_close()
