use std::path::Path;

use marlin::verilator::{
    verilator_version, AsDynamicVerilatedModel, VerilatorRuntime, VerilatorRuntimeOptions,
};
use snafu::Whatever;

use tb::SaWrapperTestTop;

fn make_runtime() -> Result<VerilatorRuntime, Whatever> {
    VerilatorRuntime::new(
        Path::new("target/marlin-cache/sa_wrapper"),
        &[
            Path::new("sa_wrapper_test_top.sv"),
            Path::new("../SA_wrapper.sv"),
            Path::new("../SA.sv"),
            Path::new("../pe.v"),
        ],
        &[Path::new("."), Path::new("..")],
        [],
        VerilatorRuntimeOptions::default()
            .allow_unsupported_verilator(Some(verilator_version!(5 031))),
    )
}

fn tick(dut: &mut SaWrapperTestTop) {
    dut.ACLK = 1;
    dut.eval();
    dut.ACLK = 0;
    dut.eval();
}

fn reset(dut: &mut SaWrapperTestTop) {
    dut.ARESETN = 0;
    tick(dut);
    dut.ARESETN = 1;
    tick(dut);
}

/// Helper: read RESULT_0..7 into an array
fn read_results(dut: &SaWrapperTestTop) -> [u32; 8] {
    [
        dut.RESULT_0,
        dut.RESULT_1,
        dut.RESULT_2,
        dut.RESULT_3,
        dut.RESULT_4,
        dut.RESULT_5,
        dut.RESULT_6,
        dut.RESULT_7,
    ]
}

/// Pack 8 bytes into a u64 (little-endian: byte 0 at bits 0-7)
fn pack_u64(data: &[u8; 8]) -> u64 {
    let mut v = 0u64;
    for i in 0..8 {
        v |= (data[i] as u64) << (i * 8);
    }
    v
}

/// Send one (A column, B row) pair on 64-bit channels.
/// Each tile is exactly one AXI transfer (ELEMS_PER_WORD = 8, Cols = 0).
fn send_tile_64(dut: &mut SaWrapperTestTop, a_word: u64, b_word: u64) {
    dut.S_AXIS_A_TDATA = a_word;
    dut.S_AXIS_A_TKEEP = 0xFF;
    dut.S_AXIS_A_TVALID = 1;
    dut.S_AXIS_B_TDATA = b_word;
    dut.S_AXIS_B_TKEEP = 0xFF;
    dut.S_AXIS_B_TVALID = 1;
    for _ in 0..8 {
        tick(dut);
        if dut.S_AXIS_A_TREADY != 0 && dut.S_AXIS_B_TREADY != 0 {
            break;
        }
    }
    dut.S_AXIS_A_TVALID = 0;
    dut.S_AXIS_B_TVALID = 0;
}

/// Send all 8 tiles. `a[j][t]` = column t, row j of A. `b[t][k]` = row t, column k of B.
fn send_all(dut: &mut SaWrapperTestTop, a: &[[u8; 8]; 8], b: &[[u8; 8]; 8]) {
    for t in 0..8 {
        let mut a_word = 0u64;
        for j in 0..8 {
            a_word |= (a[j][t] as u64) << (j * 8);
        }
        let mut b_word = 0u64;
        for k in 0..8 {
            b_word |= (b[t][k] as u64) << (k * 8);
        }
        send_tile_64(&mut *dut, a_word, b_word);
    }
}

// ========================================================================
// Tests
// ========================================================================

#[test]
#[snafu::report]
fn reset_clears_results() -> Result<(), Whatever> {
    let runtime = make_runtime()?;
    let mut dut = runtime.create_model_simple::<SaWrapperTestTop>()?;
    reset(&mut dut);

    let r = read_results(&dut);
    assert_eq!(r, [0u32; 8], "Results should be zero after reset");
    println!("=== reset clears results: OK ===");
    Ok(())
}

#[test]
#[snafu::report]
fn idle_keeps_zero() -> Result<(), Whatever> {
    let runtime = make_runtime()?;
    let mut dut = runtime.create_model_simple::<SaWrapperTestTop>()?;
    reset(&mut dut);

    // Run a few cycles without driving data
    for _ in 0..20 {
        tick(&mut dut);
    }

    let r = read_results(&dut);
    assert_eq!(r, [0u32; 8], "Results should remain zero while idle");
    println!("=== idle keeps zero: OK ===");
    Ok(())
}

#[test]
#[snafu::report]
fn all_ones() -> Result<(), Whatever> {
    let runtime = make_runtime()?;
    let mut dut = runtime.create_model_simple::<SaWrapperTestTop>()?;
    reset(&mut dut);

    println!("=== All-ones ===");
    let a = [[1u8; 8]; 8];
    let b = [[1u8; 8]; 8];
    send_all(&mut dut, &a, &b);

    // Wait for pipeline: the last tick fires on cycle 7 (tile 7).
    // After the last tick (1 cycle), draining happens (1 cycle).
    // Wait ~10 cycles to be safe.
    for _ in 0..10 {
        tick(&mut dut);
    }

    let r = read_results(&dut);
    // With all-1 inputs, each PE accumulates 8 terms of 1*1 = 8.
    let expected = [8u32; 8];
    assert_eq!(r, expected, "All-ones should give 8 per element");
    println!("  result = {:?}", r);
    println!("=== all-ones: OK ===");
    Ok(())
}

#[test]
#[snafu::report]
fn all_zeros() -> Result<(), Whatever> {
    let runtime = make_runtime()?;
    let mut dut = runtime.create_model_simple::<SaWrapperTestTop>()?;
    reset(&mut dut);

    println!("=== All-zeros ===");
    let a = [[0u8; 8]; 8];
    let b = [[0u8; 8]; 8];
    send_all(&mut dut, &a, &b);

    for _ in 0..10 {
        tick(&mut dut);
    }

    let r = read_results(&dut);
    assert_eq!(r, [0u32; 8], "All-zeros should give 0");
    println!("=== all-zeros: OK ===");
    Ok(())
}

#[test]
#[snafu::report]
fn only_one_tile() -> Result<(), Whatever> {
    let runtime = make_runtime()?;
    let mut dut = runtime.create_model_simple::<SaWrapperTestTop>()?;
    reset(&mut dut);

    println!("=== Only one tile (tile 0) ===");
    // All tiles except tile 0 are zero
    let mut a = [[0u8; 8]; 8];
    let mut b = [[0u8; 8]; 8];
    for j in 0..8 {
        a[j][0] = 1; // column 0 has all ones
    }
    for k in 0..8 {
        b[0][k] = 1; // row 0 has all ones
    }
    send_all(&mut dut, &a, &b);

    for _ in 0..10 {
        tick(&mut dut);
    }

    let r = read_results(&dut);
    // Only one term propagates through the PE chain: 1*1 = 1.
    let expected = [1u32; 8];
    assert_eq!(r, expected, "Single tile should give 1 per element");
    println!("  result = {:?}", r);
    println!("=== only-one-tile: OK ===");
    Ok(())
}

#[test]
#[snafu::report]
fn repeatable() -> Result<(), Whatever> {
    let runtime = make_runtime()?;
    let mut dut = runtime.create_model_simple::<SaWrapperTestTop>()?;

    println!("=== Repeatable (3 runs) ===");
    let a = [[5u8; 8]; 8]; // a[j][t] = 5
    let b = [[3u8; 8]; 8]; // b[t][k] = 3

    let mut prev = [0u32; 8];
    for run in 0..3 {
        reset(&mut dut);
        send_all(&mut dut, &a, &b);

        for _ in 0..10 {
            tick(&mut dut);
        }

        let r = read_results(&dut);
        if run > 0 {
            assert_eq!(r, prev, "Run {} should match run 0", run);
        }
        prev = r;
        println!("  run {} result = {:?}", run, r);
    }
    // Expected: 8 tiles × (5*3) = 120
    assert_eq!(prev, [120u32; 8], "5*3*8 should give 120");
    println!("=== repeatable: OK ===");
    Ok(())
}
