#include <time.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <cglm/cglm.h>

#include "config.h"
#include "system/logging.h"
#include "engine/input.h"
#include "engine/render.h"
#include "engine/time.h"
#include "engine/camera.h"
#include "engine/mesh.h"

#define NUM_VAO 1    // max num of Vertex Array Objects
#define NUM_VBO 3    // max num of Vertex Buffer Objects

static uint32_t vao[NUM_VAO];  // Vertex Array Objects
static uint32_t vbo[NUM_VBO];  // Vertex Buffer Objects

void main_init_meshes();
void main_update();
void main_draw_meshes();

void calc_cube_matrix(mat4 out);
void calc_pyramid_matrix(mat4 out);
void calc_floaty_animation_matrix(mat4 out);


int main() {
    render_init();
    print_engine_info();

    render_compile_shader("cube.vert", GL_VERTEX_SHADER);
    render_compile_shader("cube.frag", GL_FRAGMENT_SHADER);
    render_link_program();

    input_init();
    main_init_meshes();

    mesh_t* mesh_obj = mesh_create("monkey.gltf");

    while (!render_check_stop()) {
        render_update(&main_update);
    }
    mesh_destroy(mesh_obj);

    render_destroy();
    return EXIT_SUCCESS;
}


void main_update() {
    input_update_keyboard();
    input_update_mouse();

    camera_update_flycam();
    main_draw_meshes();
}


void main_init_meshes() {
    float cube_verts[] = {
        -1.0,  1.0, -1.0,    -1.0, -1.0, -1.0,     1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,     1.0,  1.0, -1.0,    -1.0,  1.0, -1.0,
         1.0, -1.0, -1.0,     1.0, -1.0,  1.0,     1.0,  1.0, -1.0,
         1.0, -1.0,  1.0,     1.0,  1.0,  1.0,     1.0,  1.0, -1.0,
         1.0, -1.0,  1.0,    -1.0, -1.0,  1.0,     1.0,  1.0,  1.0,
        -1.0, -1.0,  1.0,    -1.0,  1.0,  1.0,     1.0,  1.0,  1.0,
        -1.0, -1.0,  1.0,    -1.0, -1.0, -1.0,    -1.0,  1.0,  1.0,
        -1.0, -1.0, -1.0,    -1.0,  1.0, -1.0,    -1.0,  1.0,  1.0,
        -1.0, -1.0,  1.0,     1.0, -1.0,  1.0,     1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,    -1.0, -1.0, -1.0,    -1.0, -1.0,  1.0,
        -1.0,  1.0, -1.0,     1.0,  1.0, -1.0,     1.0,  1.0,  1.0,
         1.0,  1.0,  1.0,    -1.0,  1.0,  1.0,    -1.0,  1.0, -1.0
    };
    // float pyramid_verts[] = {
    //     -1.0, -1.0,  1.0,     1.0, -1.0,  1.0,     0.0,  1.0,  0.0,
    //      1.0, -1.0,  1.0,     1.0, -1.0, -1.0,     0.0,  1.0,  0.0,
    //      1.0, -1.0, -1.0,    -1.0, -1.0, -1.0,     0.0,  1.0,  0.0,
    //     -1.0, -1.0, -1.0,    -1.0, -1.0,  1.0,     0.0,  1.0,  0.0,
    //     -1.0, -1.0, -1.0,     1.0, -1.0,  1.0,    -1.0, -1.0,  1.0,
    //      1.0, -1.0,  1.0,    -1.0, -1.0, -1.0,     1.0, -1.0, -1.0
    // };

    float grid[] = {
        -10.0, 0.0, 0.0,    10.0, 0.0, 0.0
    };

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(NUM_VBO, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW
    );

    // glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    // glBufferData(
    //     GL_ARRAY_BUFFER, sizeof(pyramid_verts), pyramid_verts, GL_STATIC_DRAW
    // );

    // glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    // glBufferData(
    //     GL_ARRAY_BUFFER, sizeof(grid), grid, GL_LINE
    // );
}


vec3 v_cube_pos = {2.0, 0.0, 0.0};
vec3 v_pyr_pos = {-2.0, 0.0, 0.0};


mat4 m_model, m_modelview;
uint32_t gm_modelview, gm_persp;


void main_draw_meshes() {
    // camera_get_view_matrix();

    /* Cube draw  ( buffer 0 ) */
    calc_floaty_animation_matrix(m_model);
    // calc_cube_matrix(m_model);
    glm_mat4_mul(m_view, m_model, m_modelview);

    gm_modelview = render_get_uniform_var("gm_modelview");
    gm_persp = render_get_uniform_var("gm_persp");

    glUniformMatrix4fv(gm_modelview, 1, GL_FALSE, (float*)m_modelview);
    glUniformMatrix4fv(gm_persp, 1, GL_FALSE, (float*)m_persp);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CW);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    /* Pyramid draw  ( buffer[1] ) */
    // calc_pyramid_matrix(m_model);
    // glm_mat4_mul(m_view, m_model, m_modelview);

    // glUniformMatrix4fv(gm_modelview, 1, GL_FALSE, (float*)m_modelview);
    // glUniformMatrix4fv(gm_persp, 1, GL_FALSE, (float*)m_persp);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(0);

    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    // glFrontFace(GL_CCW);
    // glDrawArrays(GL_TRIANGLES, 0, 18);
}


void calc_cube_matrix(mat4 out) {
    vec3 model_vec = {v_cube_pos[0], v_cube_pos[1], v_cube_pos[2]};
    glm_mat4_identity(out);
    glm_translate(out, model_vec);
}


void calc_pyramid_matrix(mat4 out) {
    vec3 model_vec = {v_pyr_pos[0], v_pyr_pos[1], v_pyr_pos[2]};
    glm_mat4_identity(out);
    glm_translate(out, model_vec);
}


mat4 _m_ftrn, _m_frot;

void calc_floaty_animation_matrix(mat4 out) {
    float rot_angle = 0.4 * (float) current_time;

    glm_mat4_identity(out);
    glm_mat4_identity(_m_ftrn);
    glm_mat4_identity(_m_frot);

    vec3 v_trans = {
        sin(0.35 * current_time) * 2.0,
        cos(0.52 * current_time) * 2.0,
        sin(0.7 * current_time) * 2.0
    };
    glm_translate(_m_ftrn, v_trans);

    glm_rotate_x(_m_frot, rot_angle, _m_frot);
    glm_rotate_y(_m_frot, rot_angle, _m_frot);
    glm_rotate_z(_m_frot, rot_angle, _m_frot);

    glm_mat4_mul(_m_ftrn, _m_frot, out);
}
