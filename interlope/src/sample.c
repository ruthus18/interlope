#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./sample.h"

#include "./camera.h"
#include "./render.h"
#include "./time.h"


#define NUM_VAO 1    // max num of Vertex Array Objects
#define NUM_VBO 3    // max num of Vertex Buffer Objects

static uint32_t vao[NUM_VAO];  // Vertex Array Objects
static uint32_t vbo[NUM_VBO];  // Vertex Buffer Objects

vec3 v_cube_pos = {2.0, 0.0, 0.0};

mat4 m_model, m_modelview;
uint32_t gm_modelview, gm_persp;


void _calc_cube_matrix(mat4 out);
void _calc_floaty_animation_matrix(mat4 out);


void sample_init_cube() {
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
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(NUM_VBO, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW
    );
}


void sample_draw_cube() {
    /* Cube draw  ( buffer 0 ) */
    _calc_floaty_animation_matrix(m_model);
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
}


void _calc_cube_matrix(mat4 out) {
    vec3 model_vec = {v_cube_pos[0], v_cube_pos[1], v_cube_pos[2]};
    glm_mat4_identity(out);
    glm_translate(out, model_vec);
}


mat4 _m_ftrn, _m_frot;

void _calc_floaty_animation_matrix(mat4 out) {
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
