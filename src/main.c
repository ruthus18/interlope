#include <time.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <cglm/cglm.h>

#include "config.h"
#include "system/logging.h"
#include "engine/input.h"
#include "engine/render.h"
#include "engine/time.h"

#define NUM_VAO 1    // max num of Vertex Array Objects
#define NUM_VBO 3    // max num of Vertex Buffer Objects

static uint32_t vao[NUM_VAO];  // Vertex Array Objects
static uint32_t vbo[NUM_VBO];  // Vertex Buffer Objects

void update();

void init_meshes();
void draw_meshes();
void handle_fly_camera();

void calc_persp_matrix(mat4 out);
void calc_view_matrix(mat4 out);
void calc_cube_matrix(mat4 out);
void calc_pyramid_matrix(mat4 out);
void calc_floaty_animation_matrix(mat4 out);


int main() {
    render_init();
    print_engine_info();

    render_load_shader("cube.vert", GL_VERTEX_SHADER);
    render_load_shader("cube.frag", GL_FRAGMENT_SHADER);
    render_link_program();

    input_init();
    init_meshes();
    srand(time(NULL));  // set random generator

    while (!render_check_stop()) {
        render_update(&update);
    }
    render_destroy();
    return EXIT_SUCCESS;
}


void update() {
    input_update_keyboard();
    input_update_mouse();

    handle_fly_camera();
    draw_meshes();
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

    float grid[6] = {
        -10.0, 0.0, 0.0,    10.0, 0.0, 0.0
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

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(grid), grid, GL_LINE
    );
}


uint32_t gm_modelview, gm_persp;
mat4 m_persp, m_view, m_model, m_modelview;

vec3 v_camera_pos = {0.0, 0.0, 3.0};
vec3 v_camera_front = {0.0, 0.0, -1.0};
vec3 v_camera_up = {0.0, 1.0, 0.0};
vec3 v_camera_center;

vec3 v_camera_pos_dx;
vec3 v_camera_pos_dy;
float camera_speed = 2.5;

double yaw = -90.0, pitch = 0.0;
double yaw_delta, pitch_delta;

#define MOUSE_SENS 150
#define radian(x) (x / (180.0 / GLM_PI))


void handle_fly_camera() {
    /* Horizontal camera movement delta */
    glm_cross(v_camera_front, v_camera_up, v_camera_pos_dx);
    glm_normalize(v_camera_pos_dx);
    glm_vec3_scale(v_camera_pos_dx, camera_speed * dt, v_camera_pos_dx);

    /* Vertiical camera movement delta */
    glm_vec3_scale(v_camera_front, camera_speed * dt, v_camera_pos_dy);

    if (input_is_kb_key_pressed(GLFW_KEY_W)) {
        glm_vec3_add(v_camera_pos, v_camera_pos_dy, v_camera_pos);
    }

    if (input_is_kb_key_pressed(GLFW_KEY_S)) {
        glm_vec3_sub(v_camera_pos, v_camera_pos_dy, v_camera_pos);
    }

    if (input_is_kb_key_pressed(GLFW_KEY_A)) {
        glm_vec3_sub(v_camera_pos, v_camera_pos_dx, v_camera_pos);
    }

    if (input_is_kb_key_pressed(GLFW_KEY_D)) {
        glm_vec3_add(v_camera_pos, v_camera_pos_dx, v_camera_pos);
    }

    /* Mouse */
    yaw_delta = mouse_delta_x * MOUSE_SENS;
    pitch_delta = mouse_delta_y * MOUSE_SENS;

    yaw += yaw_delta;
    pitch += pitch_delta;

    if (pitch >  89.0) pitch = 89.0;
    if (pitch < -89.0) pitch = -89.0;

    // info_log("yaw: %f\tpitch: %f", yaw, pitch);
    v_camera_front[0] = cos(radian(yaw)) * cos(radian(pitch));
    v_camera_front[1] = sin(radian(pitch));
    v_camera_front[2] = sin(radian(yaw)) * cos(radian(pitch));

    glm_normalize(v_camera_front);
    // info_log("%f\t%f\t%f", v_camera_front[0], v_camera_front[1], v_camera_front[2]);
}


vec3 v_cube_pos = {2.0, 0.0, 0.0};
vec3 v_pyr_pos = {-2.0, 0.0, 0.0};


void draw_meshes() {
    calc_persp_matrix(m_persp);  // <- camera FOV
    calc_view_matrix(m_view);    // <- camera position

    glm_vec3_add(v_camera_pos, v_camera_front, v_camera_center); 
    glm_lookat(v_camera_pos, v_camera_center, v_camera_up, m_view);

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
    calc_pyramid_matrix(m_model);
    glm_mat4_mul(m_view, m_model, m_modelview);

    glUniformMatrix4fv(gm_modelview, 1, GL_FALSE, (float*)m_modelview);
    glUniformMatrix4fv(gm_persp, 1, GL_FALSE, (float*)m_persp);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CCW);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    /* Grid (buffer[2]) */
    
}


void calc_persp_matrix(mat4 out) {
    glm_perspective(radian(CAMERA_FOV), SCREEN_ASPECT, 0.1, 1000.0, out);
}


void calc_view_matrix(mat4 out) {
    vec3 view_vec = {-v_camera_pos[0], -v_camera_pos[1], -v_camera_pos[2]};
    glm_mat4_identity(out);
    glm_translate(out, view_vec);
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
    // float randnum = (float)rand() / (float)RAND_MAX;
    // info_log("%f", randnum);

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
