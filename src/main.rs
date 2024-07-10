mod engine;
use engine::{_api, input, render, time, ShaderType, Key};


fn main() {
    render::init();
    render::compile_shader("cube.vert", ShaderType::VERTEX);
    render::compile_shader("cube.frag", ShaderType::FRAGMENT);
    render::link_program();

    input::init();
    unsafe { _api::sample_init_cube(); }

    while render::check_stop() == false {
        input::update();
        render::update(on_update);
    }

    render::destroy();
}


fn on_update() {
    unsafe {
        _api::camera_update_flycam();
        _api::sample_draw_cube();

        if input::is_kb_key_pressed(Key::ENTER) {
            println!("Key pressed: ENTER");
        }
        
        if time::get_timer() {
            println!("Current Time: {}", time::current_time);
        }
    }

}
