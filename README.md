# Systolic Array (SA) + AXI-Stream Wrapper

## Files

| File | Description |
|---|---|
| `SA.sv` | Core systolic array (user-owned, do not modify) |
| `pe.sv` | Multiply-accumulate PE: `sum <= a*b + c_in` on `tick` |
| `sa_wrapper_axi_test.sv` | AXI-Stream wrapper around SA |
| `sa_wrapper_axi_test_tb.sv` | Self-checking testbench (single stream) |
| `sa_wrapper_axi_test_multi_tb.sv` | Multi-stream + sync testbench |
| `justfile` | `just test <tb>` builds + runs with Verilator |

## SA Architecture

SIZE×SIZE grid of PEs. Each PE reads:

```
a = a_inner_loop[i][i]   ← diagonal of circular shift register (same for all PEs in row i)
b = b_inner[i][j]        ← column j of stationary B row i
c_in = partial_sum[i][j] ← accumulated sum from PE[i-1][j]
sum = a * b + c_in
```

### Key detail: `a_inner_loop` is a circular shift register

On every `valid` posedge:
1. All elements shift right (with wrap): `a_inner_loop[i][(j+1)%SIZE] <= a_inner_loop[i][j]`
2. If `load_a`: `a_inner_loop[i][0] <= a_row[i]`
3. If `load_b && current_row[i]`: `b_inner[i] <= b_row`

Because `a = a_inner_loop[i][i]` (diagonal), PE row `i` sees the value that was loaded **i cycles ago**. The pipeline fills after **SIZE+1** valid cycles (not SIZE), because the bottom‑row diagonal element trails by one cycle.

### What it computes

With A columns fed as `a_row[k]` at cycle k: `result_row[j] = Σᵢ A[i][t−i] · B[i][j]` (skewed anti‑diagonal accumulation, **not** standard A×B).

## Wrapper: `sa_wrapper_axi_test`

### Parameters (current test config)

| Parameter | Value |
|---|---|
| `SIZE` | 4 |
| `DATA_WIDTH_IN` | 8 bits |
| `DATA_WIDTH_OUT` | 32 bits (4× input width) |
| AXI input width | `SIZE × DATA_WIDTH_IN = 32` |
| AXI output width | `SIZE × DATA_WIDTH_OUT = 128` |

### Ports

| Port | Dir | Description |
|---|---|---|
| `s_axis_A_tdata/tvalid/tready/tlast` | in/out | A‑stream (A matrix rows) |
| `s_axis_B_tdata/tvalid/tready/tlast` | in/out | B‑stream (B matrix rows) |
| `m_axis_tdata/tvalid/tready/tlast` | out/in | Result stream |

### Protocol

1. **Synchronized inputs**: Both A and B streams must be valid **on the same cycle** for a handshake to occur. If only one is valid, `tready` stays low.

2. **B loading**: During the first `SIZE` handshakes, `load_b` is high and `current_row` cycles through one‑hot positions, loading B rows into the SA.

3. **A streaming**: Height of A is unbounded — keep sending A rows. After `SIZE` input beats the pipeline fills and results appear.

4. **Drain**: Assert `tlast` on the last A beat (B should match). The wrapper internally injects `SIZE+1` drain cycles of zeros to flush the pipeline, then outputs `tlast` on the final result.

5. **Output width**: `DATA_WIDTH_OUT = 4 × DATA_WIDTH_IN` so each output beat carries a full result row without packing.

### Internal control signals

```
can_output  = !m_axis_tvalid || m_axis_tready
consume     = both_valid && !draining && can_output
drain_active = draining && (drain_cnt > 0)
drain_valid = drain_active && can_output
valid       = consume || drain_valid
```

When the output channel stalls (`m_axis_tvalid=1, m_axis_tready=0`), `can_output=0` → `valid=0` → the SA freezes, preserving pipeline state.

### Important quirk: `first_drain`

Because the SA's diagonal shift register fills one cycle late, the first drain cycle's pre‑NBA data on `result_row` is stale (zeros). The wrapper:
- Suppresses the first drain sample (`first_drain` flag)
- Runs `SIZE+1` drain cycles instead of `SIZE-1`

Without this, the first output beat is `[0,0,0,0]`.

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

## Notes for agent use

- **Do not modify `SA.sv`** — it is user‑owned
- The wrapper (`sa_wrapper_axi_test.sv`) is the file to modify for protocol changes
- Adding self‑checking to a new testbench: unpack `m_axis_tdata` into `result_row[0..SIZE-1]` and compare with expected values computed by tracing the SA's diagonal‑shift‑register behavior
- For back‑to‑back streaming without reset, BE AWARE that `beat_cnt` does not reset and `load_b` stays low after the first SIZE beats, so new B rows are NOT loaded — the old B matrix persists
