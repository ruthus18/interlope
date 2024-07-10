#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#include "./camera.h"

#include "./config.h"
#include "./logging.h"
#include "./input.h"
#include "./time.h"


mat4 m_persp, m_view;

vec3 v_camera_pos = {0.0, 0.0, 3.0};
vec3 v_camera_front = {-1.0, 0.0, -0.5};
vec3 v_camera_up = {0.0, 1.0, 0.0};
vec3 v_camera_center;

vec3 v_camera_pos_dx;
vec3 v_camera_pos_dy;
float camera_speed = 2.5;

double yaw = 18.0, pitch = 0.0;
double yaw_delta, pitch_delta;

#define radian(x) (x / (180.0 / GLM_PI))


void _camera_update();
void _camera_calc_persp_matrix(mat4 out);
void _camera_calc_view_matrix(mat4 out);


void _camera_update() {
    _camera_calc_persp_matrix(m_persp);  // <- camera FOV
    _camera_calc_view_matrix(m_view);    // <- camera position

    glm_vec3_add(v_camera_pos, v_camera_front, v_camera_center); 
    glm_lookat(v_camera_pos, v_camera_center, v_camera_up, m_view);
}


void _camera_calc_persp_matrix(mat4 out) {
    glm_perspective(radian(CAMERA_FOV), SCREEN_ASPECT, 0.1, 1000.0, out);
}


void _camera_calc_view_matrix(mat4 out) {
    vec3 view_vec = {-v_camera_pos[0], -v_camera_pos[1], -v_camera_pos[2]};
    glm_mat4_identity(out);
    glm_translate(out, view_vec);
}


void camera_update_flycam() {
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

    // log_info("yaw: %f\tpitch: %f", yaw, pitch);
    v_camera_front[0] = cos(radian(yaw)) * cos(radian(pitch));
    v_camera_front[1] = sin(radian(pitch));
    v_camera_front[2] = sin(radian(yaw)) * cos(radian(pitch));

    glm_normalize(v_camera_front);
    // log_info("%f\t%f\t%f", v_camera_front[0], v_camera_front[1], v_camera_front[2]);

    _camera_update();
}
