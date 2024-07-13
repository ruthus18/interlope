mod interlope_ffi;
mod scene;

use interlope_ffi::{_api, input, render, time, ShaderType, Key};
use cgmath::{Point3, Vector3};


fn main() {
    render::init();
    render::compile_shader("render.vert", ShaderType::VERTEX);
    render::compile_shader("render.frag", ShaderType::FRAGMENT);
    render::link_program();

    input::init();
    unsafe { _api::sample_init_cube(); }

    app_init();

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
    }

    if input::is_kb_key_pressed(Key::ENTER) {
        println!("Key pressed: ENTER");
    }
    if input::is_mouse_key_pressed(Key::LMB) {
        println!("Key pressed: LMB");
    }
    
    if time::get_timer() {
        println!("Current Time: {}", unsafe { time::current_time });
    }
}


struct Mesh;

#[derive(Debug)]
struct Object {
    position: Point3<f32>,
    rotation: Point3<f32>,
    scale: Point3<f32>,
}


impl Object {
    fn new() -> Self {
        Self {
            position: (0.0, 0.0, 0.0).into(),
            rotation: (0.0, 0.0, 0.0).into(),
            scale: (1.0, 1.0, 1.0).into(),
        }
    }
}


// struct Scene {
//     objects: Vec<Object>,
// }


fn app_init() {
    let mut scene: Vec<Object> = Vec::new();
    scene.push(Object::new());

    println!("{scene:#?}");
}
