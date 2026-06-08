# Systolic Array (SA) + AXI-Stream Wrapper

## Files

| File | Description |
|---|---|---|
| `SA.sv` | Core systolic array (user-owned, do not modify) |
| `pe.sv` | Multiply-accumulate PE: `sum <= a*b + c_in` on `tick` |
| `sa_wrapper_axi_ctrl_sv.sv` | AXI-Stream + AXI-Lite wrapper (SystemVerilog impl, do not use in Vivado BD) |
| `sa_wrapper_axi_ctrl.v` | Pure-Verilog BD wrapper for Vivado block design (instantiates `_sv`) |
| `sa_wrapper_axi_test.sv` | Older AXI-Stream wrapper (no AXI-Lite) |
| `sa_wrapper_axi_test_tb.sv` | Self-checking testbench (single stream) |
| `sa_wrapper_axi_test_multi_tb.sv` | Multi-stream + sync testbench |
| `systolic.py` | PYNQ Python driver (load from Jupyter notebook) |
| `justfile` | `just test <tb>` builds + runs with Verilator |

## SA Architecture

SIZE×SIZE grid of PEs. Each PE reads:

```
a = a_inner_loop[i][i]   ← diagonal of circular shift register (same for all PEs in row i)
b = b_inner[i][j]        ← column j of stationary B row i
c_in = partial_sum[i][j] ← accumulated sum from PE[i-1][j]
sum = a * b + c_in
```

### `a_inner_loop` — programmable recirculation

On every `valid` posedge:
1. Position 0 loads new data or recirculates: `a_inner_loop[i][0] <= load_a ? a_row[i] : a_inner_loop[i][loop_len_a - 1]`
2. Positions 1…MAX_LOOP-1 shift right: `a_inner_loop[i][j+1] <= a_inner_loop[i][j]`
3. If `load_b && current_row[i]`: `b_inner[i] <= b_row`

The A recirculation depth is **programmable** via `loop_len_a` (default = SIZE, max = 3×SIZE). An A element returns to the same PE after `loop_len_a` cycles.

Because `a = a_inner_loop[i][i]` (diagonal), PE row `i` sees the value that was loaded **i cycles ago**. The pipeline fills after **SIZE+1** valid cycles (not SIZE), because the bottom‑row diagonal element trails by one cycle.

### `loop_len_a` usage scenarios

| `loop_len_a` | Behaviour |
|---|---|
| `SIZE` (k=1) | Original — A recirculates every SIZE cycles |
| `2×SIZE` (k=2) | Double-length recirculation |
| `3×SIZE` (k=3) | Triple-length recirculation (maximum) |

Set via AXI-Lite register 0x08 before starting.

### What it computes

With A columns fed as `a_row[k]` at cycle k: `result_row[j] = Σᵢ A[i][t−i] · B[i][j]` (skewed anti‑diagonal accumulation, **not** standard A×B).

## Wrapper: `sa_wrapper_axi_ctrl` (AXI-Lite + AXI-Stream)

Recommended top-level for Vivado integration. Includes all features of
`sa_wrapper_axi_test` plus a programmable AXI4-Lite control interface.

### Block design usage

1. Add `sa_wrapper_axi_ctrl.v` to your Vivado block design as a module.
2. Add `sa_wrapper_axi_ctrl_sv.sv`, `SA.sv`, `pe.sv` as design sources.
3. Connect:
   - `s_axis_A`, `s_axis_B` → AXI DMA MM2S channels
   - `m_axis`            → AXI DMA S2MM channel
   - `s_axil_*`          → Zynq PS M_AXI_GP via AXI Interconnect

### Register map (AXI-Lite, 4‑byte aligned)

| Addr | Name   | Access | Description |
|------|--------|--------|-------------|
| 0x00 | CTRL   | R/W    | `[0]` start (SW write 1), `[1]` done (RO), `[2]` running (RO) |
| 0x04 | MODE   | R/W    | `[0]` a_reuse, `[2]` b_reuse (1 = reuse, 0 = stream) |
| 0x08 | LOOP   | R/W    | `[7:0]` loop_len_a (1 … 3×SIZE, default = SIZE) |

### Control flow

```
1. Write 0x08  ← loop_len_a          (optional, default = SIZE)
2. Write 0x04  ← mode                 (optional, 0 = normal streaming)
3. Start DMA:  A stream, B stream
4. Write 0x00  ← 1                    (start bit)
5. Wait for SA done (poll 0x00[1])    (or wait for DMA S2MM completion)
6. Read DMA output buffer
```

The TLAST handshake also works: assert `tlast` on the last A (or B) beat;
the wrapper drains automatically when `tlast` is seen.

### Parameters

| Parameter | Default | Description |
|---|---|---|
| `SIZE` | 4 | Array dimension |
| `DATA_WIDTH_IN` | 8 | Input element width (bits) |
| `DATA_WIDTH_OUT` | 32 | Output element width (bits) |
| `MAX_LOOP` | 3×SIZE | Internal A recirculation buffer depth |

### Timing (SIZE=4, both streaming, no reuse)

| Phase | Cycles | Activity |
|---|---|---|
| Fill   | SIZE+1 | load_a, load_b, current_row rotates |
| Compute | N−1  | A streaming; B frozen |
| Drain  | SIZE+1 | zeros injected, pipeline flushed |
| Total  | N + 2×SIZE + 1 | |

## Running

```sh
# Single stream test
just test sa_wrapper_axi_test_tb

# Multi-stream + synchronization test
just test sa_wrapper_axi_test_multi_tb
```

Expected single‑stream output:
```
OUT[23] = [160, 153, 146, 139]
OUT[25] = [173, 164, 155, 146]
OUT[27] = [186, 175, 164, 153]
OUT[29] = [199, 186, 173, 160] LAST
PASS: all 4 outputs match
```

## Testbench structure

The `send` task drives both A and B streams simultaneously with the same `tlast`. For synchronization testing, the multi‑stream testbench drives them independently to verify that:
- Ready stays low when only one stream is valid
- Data is only consumed when both are valid
- Backpressure stalls the SA when the output channel is blocked
- Multiple sequential streams work (with or without reset between them)

## PYNQ control (Jupyter notebook)

```python
from systolic import SystolicArray
import numpy as np

sa = SystolicArray('systolic.bit', ctrl_base_addr=0x43C00000)

A = np.array([[1, 2, 3, 4],
              [5, 6, 7, 8],
              [9, 0, 1, 2],
              [3, 4, 5, 6]], dtype=np.uint8)

B = np.array([[1, 0, 0, 0],
              [0, 1, 0, 0],
              [0, 0, 1, 0],
              [0, 0, 0, 1]], dtype=np.uint8)

result = sa.compute(A, B)   # normal streaming
result_with_reuse = sa.compute(A, B, a_reuse=True, loop_len=12)
```

### Vivado BD requirements

| IP | Role | Connection |
|---|---|---|
| `sa_wrapper_axi_ctrl.v` | SA top (add as BD module) | — |
| `axi_dma_0` | A stream MM2S + result S2MM | `s_axis_A`, `m_axis` |
| `axi_dma_1` | B stream MM2S | `s_axis_B` |
| `axi_interconnect` | PS ↔ AXI-Lite / DMA | M_AXI_GP0 → control + DMA regs |
| `clk_wiz` (optional) | Clock generation | — |

### Register quick reference

```python
from pynq import MMIO

mmio = MMIO(0x43C00000, 0x1000)

mmio.write(0x08, 12)   # loop_len_a = 12
mmio.write(0x04, 0x01) # a_reuse = 1
mmio.write(0x00, 0x01) # start

# poll until done
while not (mmio.read(0x00) & 0x2):
    pass
```

## PYNQ-Z2 Deployment (Vivado flow)

### Prerequisites

- Vivado 2025.2 (or compatible)
- PYNQ-Z2 board with PYNQ v3.1.1 image
- `ssh` access configured (SSH port 2222 on the board)

### Build bitstream

```sh
cd build/scripts
source /path/to/Vivado/2025.2/settings64.sh
vivado -mode batch -source build.tcl 2>&1 | tee build.log
```

This generates `systolic.bit` and `systolic.hwh` in the project root.

### Deploy to PYNQ

```sh
### Power-cycling the PYNQ board

The board is on a networked ESPHome switch at `qswitch_test.local`. Use `curl`:

```sh
# Power off
curl -s -X POST -H "Content-Type: application/json" -d '{}' \
  "http://qswitch_test.local/switch/Misc%232/turn_off"

# Wait 8 seconds, then power on
sleep 8
curl -s -X POST -H "Content-Type: application/json" -d '{}' \
  "http://qswitch_test.local/switch/Misc%232/turn_on"
```

Wait ~80s for the board to fully boot (SSH port 2222).

### Deploy to PYNQ

```sh
ssh pynq 'echo xilinx | sudo -S mkdir -p /home/xilinx/jupyter_notebooks/sa_test_axis'
scp systolic.bit systolic.hwh systolic.py test_systolic.ipynb \
  pynq:/home/xilinx/jupyter_notebooks/sa_test_axis/
```

### Run on PYNQ (SSH)

```sh
ssh pynq
sudo -i
source /etc/profile.d/pynq_venv.sh
source /etc/profile.d/xrt_setup.sh
cd /home/xilinx/jupyter_notebooks/sa_test_axis
python3 -c "from systolic import SystolicArray; SystolicArray().test_all()"
```

### Run from Jupyter

Open `http://<pynq-ip>:9090` in a browser, navigate to `sa_test_axis/test_systolic.ipynb`, and run all cells.
## Notes for agent use

- The systolic array core is `SA.sv` — the wrapper and BD files are for integration
- **Do not modify `SA.sv`** — it is user‑owned
- The wrapper (`sa_wrapper_axi_ctrl_sv.sv`) is the file to modify for protocol changes
- Adding self‑checking to a new testbench: unpack `m_axis_tdata` into `result_row[0..SIZE-1]` and compare with expected values computed by tracing the SA's diagonal‑shift‑register behavior
- For back‑to‑back streaming without reset, BE AWARE that `beat_cnt` does not reset and `load_b` stays low after the first SIZE beats, so new B rows are NOT loaded — the old B matrix persists
