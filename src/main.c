#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "config.h"
#include "types.h"
#include "utils/io.h"
#include "utils/time.h"

#define __DEBUG__SHOW_FPS_IN_TITLE true


void init(GLFWwindow* window);
void init_cube_mesh();

void process(GLFWwindow* window, double delta);

MatrixID calc_cube_animation_matrix(double time, mat4 out);
ProgramID create_shader_program();
ShaderID load_shader(const char* path, int shader_type);
bool check_opengl_error();


int main() {
    if (!glfwInit()) {
        error_log("Could not initialize GLFW!");
        return EXIT_FAILURE;
    }
    /* Set GL context options */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    GLFWwindow* window = glfwCreateWindow(
        SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, NULL, NULL
    );
    glfwMakeContextCurrent(window);

    int glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        error_log("Could not initialize GLEW, status=%s", glew_status);
        return EXIT_FAILURE;
    }
    glfwSwapInterval(1);  // VSync

    print_engine_info();
    init(window);

    double delta;
    while (!glfwWindowShouldClose(window)) {
        delta = update_time();
        process(window, delta);

        if (__DEBUG__SHOW_FPS_IN_TITLE && second_passed()) {
            char title_buf[128];
            sprintf(title_buf, "%s [%i FPS]", SCREEN_TITLE, get_fps());
            glfwSetWindowTitle(window, title_buf);
        }

        glfwSwapBuffers(window);  // Draw content, VSync
        glfwPollEvents();         // Handle window-related events (kb, mouse...) 
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}


GLuint vao[NUM_VAO];
GLuint vbo[NUM_VBO];

/* Allocated during `init` */
ProgramID program;
float camera_x, camera_y, camera_z;
float cube_loc_x, cube_loc_y, cube_loc_z;


void init(GLFWwindow* window) {
    program = create_shader_program();
    camera_x = 0.0;
    camera_y = 0.0;
    camera_z = 8.0;

    cube_loc_x = 0.0;
    cube_loc_y = -2.0;
    cube_loc_z = 0.0;

    init_cube_mesh();
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


/* Allocated during `process` */
MatrixID gm_modelview, gm_persp;
int width, height;
float aspect;

mat4 m_persp, m_view, m_model, m_modelview;


void process(GLFWwindow* window, double delta) {
    glClear(GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.9, 0.9, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    gm_modelview = glGetUniformLocation(program, "gm_modelview");
    gm_persp = glGetUniformLocation(program, "gm_persp");

    // Build perspective matrix
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    glm_perspective(1.0472, aspect, 0.1, 1000.0, m_persp);  // 1.0472 radians = 60 deg

    // View matrix
    vec3 v_view = {-camera_x, -camera_y, -camera_z};
    glm_mat4_identity(m_view);
    glm_translate(m_view, v_view);

    // Model matrix
    calc_cube_animation_matrix(glfwGetTime(), m_model);

    // vec3 v_model = {cube_loc_x, cube_loc_y, cube_loc_z};
    // glm_mat4_identity(m_model);
    // glm_translate(m_model, v_model);

    // Model-View matrix
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


MatrixID calc_cube_animation_matrix(double time, mat4 out) {
    mat4 mat_trans, mat_rot;
    float rot_angle = 0.4 * (float) time;

    glm_mat4_identity(out);
    glm_mat4_identity(mat_trans);
    glm_mat4_identity(mat_rot);

    vec3 v_trans = {
        sin(0.35 * time) * 2.0,
        cos(0.52 * time) * 2.0,
        sin(0.7 * time) * 2.0
    };
    glm_translate(mat_trans, v_trans);

    // vec3 rotate_x_vec = {1.0, 0.0, 0.0};
    // glm_rotate(mat_rot, 0.25 * (float)time, rotate_x_vec);

    glm_rotate_x(mat_rot, rot_angle, mat_rot);
    glm_rotate_y(mat_rot, rot_angle, mat_rot);
    glm_rotate_z(mat_rot, rot_angle, mat_rot);

    glm_mat4_mul(mat_trans, mat_rot, out);
}


ProgramID create_shader_program() {
    ProgramID _program = glCreateProgram();
    int link_ok;

    ShaderID v_shader = load_shader("cube_vert.glsl", GL_VERTEX_SHADER);
    ShaderID f_shader = load_shader("cube_frag.glsl", GL_FRAGMENT_SHADER);
    glAttachShader(_program, v_shader);
    glAttachShader(_program, f_shader);

    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &link_ok);
    if (link_ok != 1) {
        error_log("GL program linking error");
        program_log(_program);
    }

    return _program;
}


ShaderID load_shader(const char* file_path, int shader_type) {
    ShaderID shader = glCreateShader(shader_type);
    int compile_ok;

    const char* path = shader_path(file_path);
    const char* file_buffer = load_file(path);

    glShaderSource(shader, 1, &file_buffer, NULL);
    free((void*) path);
    free((void*) file_buffer);

    glCompileShader(shader);
    check_opengl_error();

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok != 1) {
        error_log("Shader compilation error; file=%s", file_path);
        shader_log(shader);
    }
    return shader;
}


bool check_opengl_error() {
    bool is_err = false;
    int gl_err = glGetError();

    while (gl_err != GL_NO_ERROR) {
        error_log("OpenGL Error: %s", gl_err);
        is_err = true;
    }
    return is_err;
}
