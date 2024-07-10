#include <time.h>
#include <stdlib.h>

#include <cglm/cglm.h>

#include "./config.h"
#include "./logging.h"
#include "./input.h"
#include "./render.h"
#include "./time.h"
#include "./camera.h"
#include "./sample.h"


void main_update();
void main_draw_meshes();


int main() {
    render_init();
    log_engine_info();

    render_compile_shader("cube.vert", GL_VERTEX_SHADER);
    render_compile_shader("cube.frag", GL_FRAGMENT_SHADER);
    render_link_program();

    input_init();
    sample_init_cube();

    // mesh_t* mesh_obj = mesh_create("monkey.gltf");

    while (!render_check_stop()) {
        render_update(&main_update);
    }
    // mesh_destroy(mesh_obj);

    render_destroy();
    return EXIT_SUCCESS;
}


void main_update() {
    input_update_keyboard();
    input_update_mouse();

    camera_update_flycam();
    main_draw_meshes();
}
