#include <time.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <cglm/cglm.h>

#include "config.h"
#include "input.h"
#include "render.h"
#include "logging.h"

#define NUM_VAO 1    // max num of Vertex Array Objects
#define NUM_VBO 2    // max num of Vertex Buffer Objects

static uint32_t vao[NUM_VAO];  // Vertex Array Objects
static uint32_t vbo[NUM_VBO];  // Vertex Buffer Objects

void update(double dt);

void init_meshes();
void draw_meshes();

void calc_persp_matrix(mat4 out);
void calc_view_matrix(mat4 out);
void calc_cube_matrix(mat4 out);
void calc_pyramid_matrix(mat4 out);
void calc_floaty_animation_matrix(double time, mat4 out);


int main() {
    render_init();
    print_engine_info();

    render_load_shader("cube.vert", GL_VERTEX_SHADER);
    render_load_shader("cube.frag", GL_FRAGMENT_SHADER);
    render_link_program();

    init_input();
    init_meshes();
    srand(time(NULL));  // set random generator

    while (!render_check_stop()) {
        render_update(&update);
    }
    render_destroy();
    return EXIT_SUCCESS;
}


void update(double time) {
    update_input();
    draw_meshes();

    info_log(
        "t=%f\tΔmouse_x=%f\tΔmouse_y=%f", time, mouse_delta_x, mouse_delta_y
    );
}


void init_meshes() {
    float cube_verts[108] = {
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
    float pyramid_verts[54] = {
        -1.0, -1.0,  1.0,     1.0, -1.0,  1.0,     0.0,  1.0,  0.0,
         1.0, -1.0,  1.0,     1.0, -1.0, -1.0,     0.0,  1.0,  0.0,
         1.0, -1.0, -1.0,    -1.0, -1.0, -1.0,     0.0,  1.0,  0.0,
        -1.0, -1.0, -1.0,    -1.0, -1.0,  1.0,     0.0,  1.0,  0.0,
        -1.0, -1.0, -1.0,     1.0, -1.0,  1.0,    -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,    -1.0, -1.0, -1.0,     1.0, -1.0, -1.0
    };

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(NUM_VBO, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(pyramid_verts), pyramid_verts, GL_STATIC_DRAW
    );
}


uint32_t gm_modelview, gm_persp;
mat4 m_persp, m_view, m_model, m_modelview;

float camera_x = 0.0;
float camera_y = 0.0;
float camera_z = 8.0;

float cube_loc_x =  2.0;
float cube_loc_y =  0.0;
float cube_loc_z =  0.0;

float pyr_loc_x =  -2.0;
float pyr_loc_y =   0.0;
float pyr_loc_z =   0.0;


void draw_meshes() {
    gm_modelview = render_get_uniform_var("gm_modelview");
    gm_persp = render_get_uniform_var("gm_persp");

    calc_persp_matrix(m_persp);  // <- camera FOV
    calc_view_matrix(m_view);    // <- camera position

    /* Cube draw  ( buffer 0 ) */
    calc_cube_matrix(m_model);
    glm_mat4_mul(m_view, m_model, m_modelview);

    glUniformMatrix4fv(gm_modelview, 1, GL_FALSE, (float*)m_modelview);
    glUniformMatrix4fv(gm_persp, 1, GL_FALSE, (float*)m_persp);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    /* Pyramid draw  ( buffer[1] ) */
    calc_pyramid_matrix(m_model);
    glm_mat4_mul(m_view, m_model, m_modelview);

    glUniformMatrix4fv(gm_modelview, 1, GL_FALSE, (float*)m_modelview);
    glUniformMatrix4fv(gm_persp, 1, GL_FALSE, (float*)m_persp);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);
}


void calc_persp_matrix(mat4 out) {
    float fov_rad = CAMERA_FOV / (180.0 / GLM_PI);
    glm_perspective(fov_rad, SCREEN_ASPECT, 0.1, 1000.0, out);
}


void calc_view_matrix(mat4 out) {
    vec3 view_vec = {-camera_x, -camera_y, -camera_z};
    glm_mat4_identity(out);
    glm_translate(out, view_vec);
}


void calc_cube_matrix(mat4 out) {
    vec3 model_vec = {cube_loc_x, cube_loc_y, cube_loc_z};
    glm_mat4_identity(out);
    glm_translate(out, model_vec);
}


void calc_pyramid_matrix(mat4 out) {
    vec3 model_vec = {pyr_loc_x, pyr_loc_y, pyr_loc_z};
    glm_mat4_identity(out);
    glm_translate(out, model_vec);
}


mat4 _m_ftrn, _m_frot;

void calc_floaty_animation_matrix(double time, mat4 out) {
    float rot_angle = 0.4 * (float) time;
    // float randnum = (float)rand() / (float)RAND_MAX;
    // info_log("%f", randnum);

    glm_mat4_identity(out);
    glm_mat4_identity(_m_ftrn);
    glm_mat4_identity(_m_frot);

    vec3 v_trans = {
        sin(0.35 * time) * 2.0,
        cos(0.52 * time) * 2.0,
        sin(0.7 * time) * 2.0
    };
    glm_translate(_m_ftrn, v_trans);

    glm_rotate_x(_m_frot, rot_angle, _m_frot);
    glm_rotate_y(_m_frot, rot_angle, _m_frot);
    glm_rotate_z(_m_frot, rot_angle, _m_frot);

    glm_mat4_mul(_m_ftrn, _m_frot, out);
}
