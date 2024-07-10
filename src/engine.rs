extern crate core;
use std::ffi::{c_void, c_char};


pub mod api {
    extern "C" {
        pub fn window_init() -> super::c_char;
        pub fn window_destroy() -> super::c_void;
        pub fn window_update() -> super::c_void;
    }
}