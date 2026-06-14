#!/usr/bin/env python3
"""Build SA RTL with Verilator + Cocotb and run tests.

Usage (from tb/cocotb/):
    uv run python3 run.py                   # build + run all
    uv run python3 run.py --test negative   # single test
"""

import sys, os, subprocess

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = [os.path.join(REPO, f) for f in [
    "sa_wrapper_axi_ctrl_sv.sv", "SA.sv", "pe.sv", "counter.sv", "defs.svh"]]
TOP = "sa_wrapper_axi_ctrl_sv"
OBJ = os.path.join(REPO, "obj_cocotb")


def build():
    share = subprocess.run(["cocotb-config", "--share"],
                           capture_output=True, text=True).stdout.strip()
    lib_dir = subprocess.run(["cocotb-config", "--lib-dir"],
                              capture_output=True, text=True).stdout.strip()
    lib_path = subprocess.run(["cocotb-config", "--lib-name-path", "vpi", "verilator"],
                              capture_output=True, text=True).stdout.strip()
    vpi_src = os.path.join(share, "lib", "verilator", "verilator.cpp")
    inc = os.path.join(share, "include")
    os.makedirs(OBJ, exist_ok=True)

    verilator = os.environ.get("VERILATOR_ROOT", "verilator")
    cmd = [verilator, "--cc", TOP, "--build", "--exe",
           "--top-module", TOP,
           "--Mdir", OBJ, "-j", "8",
           "--timing", "--hierarchical", "--threads", "2",
           "-Wno-WIDTHTRUNC", "-Wno-WIDTHEXPAND", "-Wno-UNOPTFLAT",
           "-CFLAGS", f"-I{inc}",
           "-LDFLAGS", f"-L{lib_dir} -lcocotb",
           ] + SRC + [vpi_src]
    print(" ".join(str(c) for c in cmd))
    subprocess.run(cmd, check=True, cwd=REPO)


def run(test_filter=""):
    sim = os.path.join(OBJ, f"V{TOP}")
    if not os.path.exists(sim):
        build()

    lib = subprocess.run(["cocotb-config", "--lib-name-path", "verilator"],
                         capture_output=True, text=True).stdout.strip()
    venv = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    env = os.environ.copy()
    env["TOPLEVEL"] = TOP
    env["MODULE"] = "tb.cocotb.test_sa"
    env["COCOTB_HDL_TIMEUNIT"] = "1ns"
    env["COCOTB_HDL_TIMEPRECISION"] = "1ps"
    env["PYTHONPATH"] = REPO + ":" + env.get("PYTHONPATH", "")
    if test_filter:
        env["TESTCASE"] = test_filter

    subprocess.run([sim], env=env, cwd=REPO, check=True)


if __name__ == "__main__":
    flt = ""
    if "--test" in sys.argv:
        flt = sys.argv[sys.argv.index("--test") + 1]
    build()
    run(flt)
