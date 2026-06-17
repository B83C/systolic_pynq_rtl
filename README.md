# Systolic Array (SA) + AXI-Stream Wrapper

## Files

| File | Description |
|---|---|---|
| `SA.sv` | Core systolic array (user-owned, do not modify) |
| `pe.sv` | Multiply-accumulate PE: `sum <= a*b + c_in` on `tick` |
| `sa_wrapper_axi_ctrl_sv.sv` | AXI-Stream + AXI-Lite wrapper (main RTL) |
| `counter.sv` | Up-counter with dynamic max, used for FB_CNT accumulation |
| `systolic.py` | PYNQ Python driver |
| `tb/` | Split testbench includes (tb_common, tb_test_01 … tb_test_13) |
| `justfile` | `just test-ctrl` builds + runs Verilator |

## Register map (AXI-Lite, 6-bit addresses)

| Addr | Name | Access | Description |
|------|------|--------|-------------|
| 0x00 | STATE | RO | `[1:0]` state (0=IDLE, 1=LOAD_A, 2=LOAD_B, 3=LOAD_C) |
| 0x04 | STATUS | RO | `[0]` b_underflow (clear-on-read) |
| 0x08 | C_LOAD | WO | write to trigger C ring loading |
| 0x0C | FB_CNT | RW | `[7:0]` accumulation group size (0=off) |
| 0x10 | A_LOAD | WO | write to trigger A ring loading |
| 0x14 | ACC_OUT | RW | `[0]` enable output during accumulation |
| 0x18 | A_LOOP_START | RW | A ring start index |
| 0x1C | A_LOOP_END | RW | A ring end index |
| 0x20 | C_LOOP_START | RW | C ring start index |
| 0x24 | C_LOOP_END | RW | C ring end index |

Writes are only accepted when state is IDLE or LOAD_B. The `idle` output port signals state == IDLE.

## Control flow

```
1. Configure registers (FB_CNT, ACC_OUT, ring bounds).  Must be IDLE.
2. Trigger A_LOAD (0x10).  State → LOAD_A.  Stream A rows on s_axis_B.
3. Optional: trigger C_LOAD (0x08).  State → LOAD_C.  Stream C values.
4. Stream B rows.  State → LOAD_B.  Computation starts on first B row.
5. Output appears on m_axis.  Wait for tlast or idle.
```

State machine:
- IDLE → LOAD_C (c_load_pending, priority) → IDLE
- IDLE → LOAD_A (a_load_pending) → IDLE
- IDLE → LOAD_B (default)
- LOAD_B → LOAD_C (c_load_pending && !operate)
- LOAD_B → LOAD_A (a_load_pending && !operate)
- LOAD_B → IDLE (s_axis_B_tlast)

## Running Verilator tests

```sh
just test-ctrl
```

Tests 1-13 cover: single multiply, back-to-back, accumulation, eye, consecutive groups, undersized, output back-pressure, continuous streaming, random back-pressure, A ring buffer, A+C ring accumulation, C matrix.

## PYNQ driver

**Note: `systolic.py`'s `compute()` / `compute_acc()` methods are currently broken**
(duplicate `_dma_send`/`_dma_recv` override + `self.dma` uninitialized).
Use the low-level MMIO+DMA flow in `test_systolic_driver.ipynb` instead.

### Sourcing the environment

When running scripts via SSH (outside Jupyter), source these files first:

```sh
source /etc/profile.d/pynq_venv.sh
source /etc/profile.d/xrt_setup.sh
```

Then run with sudo:
```sh
echo xilinx | sudo -S python3 your_script.py
```

Or as a one-liner:
```sh
ssh pynq 'cd /path/to/scripts && echo xilinx | sudo -S bash -c \
  "source /etc/profile.d/pynq_venv.sh && source /etc/profile.d/xrt_setup.sh && python3 bench_sa.py"'
```

### Low-level usage (MMIO + DMA — works)

```python
from pynq import Overlay, allocate, MMIO
import numpy as np

ol = Overlay("systolic.bit"); ol.download()
dma = ol.axi_dma_0

ip_base = ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr']
mmio = MMIO(ip_base, 0x1000)
mmio_ch_t = MMIO(ol.ip_dict['chlast_to_tiled_gath_0']['phys_addr'], 0x1000)
mmio_quant = MMIO(ol.ip_dict['quantizer_wrapper_0']['phys_addr'], 0x1000)
mmio_t_ch = MMIO(ol.ip_dict['tiled_to_chlast_gath_0']['phys_addr'], 0x1000)
```

See `test_systolic_driver.ipynb` for the full data movement flow (block-major A,
channel-last B, quantizer configuration).

## Benchmark

Files on the PYNQ board (`~/jupyter_notebooks/test_systolic_vivado_manual/`):

| File | What |
|------|------|
| `bench_sa.py` | SA vs numpy throughput (Ci,Co ≤ 64, N arbitrary, all sizes pass correctness) |
| `dma_bench` | C++ binary — raw DMA latency via `/dev/mem` register access |
| `run_dma_bench.py` | Python wrapper comparing C++ vs Python DMA timing |

### Python benchmark (SA vs numpy)

```sh
ssh pynq 'cd /home/xilinx/jupyter_notebooks/test_systolic_vivado_manual && \
  echo xilinx | sudo -S bash -c "source /etc/profile.d/pynq_venv.sh && \
  source /etc/profile.d/xrt_setup.sh && python3 bench_sa.py"'
```

### C++ DMA latency

```sh
ssh pynq 'cd /home/xilinx/jupyter_notebooks/test_systolic_vivado_manual && \
  echo xilinx | sudo -S bash -c "source /etc/profile.d/pynq_venv.sh && \
  source /etc/profile.d/xrt_setup.sh && python3 run_dma_bench.py"'
```

### Results summary

| Co | Ci | N | SA vs numpy | Bandwidth |
|----|----|---|-------------|-----------|
| 64 | 64 | 1024 | **77× faster** | ~200 MB/s |
| 64 | 64 | 512 | **35× faster** | ~200 MB/s |
| 64 | 64 | 256 | **19× faster** | ~200 MB/s |
| 64 | 64 | 128 | **10× faster** | ~200 MB/s |
| 64 | 64 | 64 | **5.7× faster** | ~200 MB/s |

SA throughput is **~0.4 GOPS** (Python) / **~200 MB/s** DMA-limited. C++ direct register access eliminates Python's ~500 µs fixed overhead, yielding up to **166× improvement for small matrices** (N=64). For large streaming workloads (N≥4096), C++ gains are ~10% since bandwidth dominates.

## Deployment (g7-station → PYNQ)

### Power-cycle the board

The board is on a networked ESPHome switch:

```sh
# Power off
curl -s -X POST -H "Content-Type: application/json" -d '{}' \
  "https://radxa-cubie-a5e.shark-anoles.ts.net/switch/Misc%232/turn_off"
sleep 8
# Power on
curl -s -X POST -H "Content-Type: application/json" -d '{}' \
  "https://radxa-cubie-a5e.shark-anoles.ts.net/switch/Misc%232/turn_on"
```

Wait ~80s for the board to fully boot (SSH port 2222).

### Vivado rebuild (g7-station)

```sh
cd ~/git/systolic && git pull
cd ~/vivado/test_systolic
vivado -mode batch -source /dev/stdin <<'EOF'
open_project test_systolic.xpr
reset_run synth_1
reset_run impl_1
launch_runs synth_1 -jobs 8
wait_on_run synth_1
launch_runs impl_1 -to_step write_bitstream -jobs 8
wait_on_run impl_1
exit
EOF
cd ~/git/systolic && just upload 3
```

### Deploy to PYNQ

```sh
ssh pynq 'echo xilinx | sudo -S mkdir -p /home/xilinx/jupyter_notebooks/sa_test_axis'
scp systolic.bit systolic.hwh systolic.py \
  pynq:/home/xilinx/jupyter_notebooks/sa_test_axis/
```

### Run self-test

```sh
ssh pynq 'cd /home/xilinx/jupyter_notebooks/sa_test_axis && \
  sudo -i python3 -c "from systolic import SystolicArray; SystolicArray(None, 0x43C00000).test_all()"'
```

**Note:** The bitstream must match the RTL in this repo.  After RTL port/register
changes, rebuild on g7-station before deploying.
