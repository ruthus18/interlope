extern crate core;
use core::ffi::c_int;

extern "C" {
    fn multiply(a: c_int, b: c_int) -> c_int;
}


fn main() {
    println!("[R] Start Rust App!");
    unsafe { 
        println!("[R] Result: {}", multiply(14, 14));
    }
}
