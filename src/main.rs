mod engine;
use engine::api::{window_init, window_update};


fn main() {
    println!("[R] Start Rust App!");
    unsafe { window_init() };
    loop {
        unsafe { window_update() };
    }
    // unsafe { engine::_api::window_destroy() };
}
