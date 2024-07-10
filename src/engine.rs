extern crate core;
use std::ffi::{c_int, c_char, c_double};


pub mod _api {
    use super::{c_int, c_char};

    extern "C" {

        // CAMERA
        //
        pub fn camera_update_flycam();

        // INPUT
        //
        pub fn input_init();
        pub fn input_update_keyboard();
        pub fn input_update_mouse();
        pub fn input_is_kb_key_pressed(key: c_int) -> bool;

        // LOGGING
        //
        pub fn log_engine_info();

        // RENDER
        // TODO: combine `update` and `post_update`, trace callback
        //
        pub fn render_init();
        pub fn render_update();
        pub fn render_post_update();
        pub fn render_check_stop() -> bool;
        pub fn render_stop();
        pub fn render_destroy();
        pub fn render_link_program();
        pub fn render_compile_shader(path: *const c_char, shader_type: c_int) -> c_int;

        // SAMPLES
        //
        pub fn sample_init_cube();
        pub fn sample_draw_cube();
    }
}


pub enum ShaderType {
    VERTEX = 0x8B31,
    FRAGMENT = 0x8B30,
}

#[allow(dead_code)]
pub enum Key {
    ESC = 256,
    ENTER = 257,
    RIGHT = 262,
    LEFT = 263,
    DOWN = 264,
    UP = 265,
}


pub mod input {
    use super::{Key, _api, c_int};

    pub fn init() {
        unsafe { _api::input_init(); }
    }

    pub fn update() {
        unsafe {
            _api::input_update_keyboard();
            _api::input_update_mouse();
        }
    }

    pub fn is_kb_key_pressed(key: Key) -> bool {
        unsafe { return _api::input_is_kb_key_pressed(key as c_int); }
    }
}


pub mod render {
    use super::{ShaderType, _api};
    use std::ffi::{CString,c_int};

    pub fn init() {
        unsafe {
            _api::render_init();
            _api::log_engine_info();
        }
    }

    pub fn destroy() {
        unsafe { _api::render_destroy(); }
    }

    pub fn compile_shader(shader_path: &str, shader_type: ShaderType) {
        let shader_path = CString::new(shader_path).unwrap();
        unsafe {
            _api::render_compile_shader(
                shader_path.as_ptr(),
                shader_type as c_int
            );
        }
    }

    pub fn link_program() {
        unsafe { _api::render_link_program(); }
    }

    pub fn check_stop() -> bool {
        unsafe { return _api::render_check_stop(); }
    }

    #[allow(dead_code)]
    pub fn stop() {
        unsafe { _api::render_stop(); }
    }

    pub fn update(func: fn()) {
        unsafe { _api::render_update(); }
        func();
        unsafe { _api::render_post_update();}
    }

}


pub mod time {
    use super::c_double;

    extern "C" {
        pub static current_time: c_double;
        pub static second_passed: bool;
    }

    pub fn get_timer() -> bool {
        unsafe{ return second_passed; }
    }
}
