#![no_std]
#![no_main]

use core::cmp::{max, min};

fn clamp(v: i32) -> u8 {
    min(max(v, 0), 255) as u8
}

#[panic_handler]
fn panic(_: &core::panic::PanicInfo) -> ! {
    loop {}
}

#[unsafe(no_mangle)]
pub extern "C" fn depthwise3x3(
    inp: *const u8,
    w: *const i8,
    out: *mut u8,
    c: i32,
    h: i32,
    w_: i32,
    shift: i32,
) {
    let c = c as usize;
    let h = h as usize;
    let w_ = w_ as usize;
    let rounding = 1i32 << (shift - 1).max(0);
    for ch in 0..c {
        let base = ch * h * w_;
        for y in 1..h - 1 {
            for x in 1..w_ - 1 {
                let mut acc: i32 = 0;
                for ky in 0..3 {
                    for kx in 0..3 {
                        let py = y - 1 + ky;
                        let px = x - 1 + kx;
                        unsafe {
                            let p = *inp.add(base + py * w_ + px) as i32;
                            let wt = *w.add(ch * 9 + ky * 3 + kx) as i32;
                            acc += p * wt;
                        }
                    }
                }
                let v = (acc + rounding) >> shift;
                unsafe {
                    *out.add(base + y * w_ + x) = clamp(v);
                }
            }
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn conv3x3_reg(
    inp: *const u8,
    w: *const i8,
    out: *mut u8,
    c_in: i32,
    c_out: i32,
    h: i32,
    w_: i32,
    shift: i32,
    pad: i32,
) {
    let c_in = c_in as usize;
    let c_out = c_out as usize;
    let h = h as usize;
    let w_ = w_ as usize;
    let pad = pad as usize;
    let h_out = h + 2 * pad - 2;
    let w_out = w_ + 2 * pad - 2;
    let rounding = 1i32 << (shift - 1).max(0);
    for o in 0..c_out {
        let w_base = o * c_in * 9;
        for y in 0..h_out {
            for x in 0..w_out {
                let mut acc: i32 = 0;
                for ci in 0..c_in {
                    for ky in 0..3 {
                        for kx in 0..3 {
                            let iy = y as i32 + ky as i32 - pad as i32;
                            let ix = x as i32 + kx as i32 - pad as i32;
                            if iy >= 0 && iy < h as i32 && ix >= 0 && ix < w_ as i32 {
                                unsafe {
                                    let p = *inp.add(ci * h * w_ + iy as usize * w_ + ix as usize)
                                        as i32;
                                    let wt = *w.add(w_base + ci * 9 + ky * 3 + kx) as i32;
                                    acc += p * wt;
                                }
                            }
                        }
                    }
                }
                let v = (acc + rounding) >> shift;
                unsafe {
                    *out.add(o * h_out * w_out + y * w_out + x) = clamp(v);
                }
            }
        }
    }
}
