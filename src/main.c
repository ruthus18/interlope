#include <stdlib.h>

#include <GL/glew.h>
#include <cglm/cglm.h>

#include "config.h"
#include "render.h"
#include "types.h"
#include "utils/io.h"


GLuint vao[NUM_VAO];
GLuint vbo[NUM_VBO];

void init_cube_mesh();
void frame(double delta);
void calc_persp_matrix(mat4 out);
void calc_view_matrix(mat4 out);
void calc_model_matrix(mat4 out);
void calc_floaty_animation_matrix(double time, mat4 out);


int main() {
    render_init();
    print_engine_info();

    load_shader("cube_vert.glsl", GL_VERTEX_SHADER);
    load_shader("cube_frag.glsl", GL_FRAGMENT_SHADER);
    link_gl_program();

    init_cube_mesh();
    render_run(&frame);

    render_close();
    return EXIT_SUCCESS;
}


void init_cube_mesh() {
    float cube_vertices[108] = {
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
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(NUM_VBO, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW
    );
}


/* Runtime variables */
GLVar gm_modelview, gm_persp;
mat4 m_persp, m_view, m_model, m_modelview;

float camera_x = 0.0;
float camera_y = 0.0;
float camera_z = 8.0;

float cube_loc_x = 0.0;
float cube_loc_y = -2.0;
float cube_loc_z = 0.0;


void frame(double time) {
    gm_modelview = get_uniform_var("gm_modelview");
    gm_persp = get_uniform_var("gm_persp");

    calc_persp_matrix(m_persp);
    calc_view_matrix(m_view);
    // calc_model_matrix();  // Animation instead

    calc_floaty_animation_matrix(time, m_model);
    
    // model-view matrix
    glm_mat4_mul(m_view, m_model, m_modelview);

    // copy Persp. and MV matrices to shader vars
    glUniformMatrix4fv(gm_modelview, 1, GL_FALSE, (float*)m_modelview);
    glUniformMatrix4fv(gm_persp, 1, GL_FALSE, (float*)m_persp);

    // associate VBO with vertex attr in v-shader
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // adjust OpenGL settings and draw model
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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


void calc_model_matrix(mat4 out) {
    vec3 model_vec = {cube_loc_x, cube_loc_y, cube_loc_z};
    glm_mat4_identity(out);
    glm_translate(out, model_vec);
}


mat4 _m_ftrn, _m_frot;

void calc_floaty_animation_matrix(double time, mat4 out) {
    float rot_angle = 0.4 * (float) time;

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
