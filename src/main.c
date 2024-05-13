#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "types.h"
#include "utils/io.h"
#include "utils/time.h"

#define __DEBUG__SHOW_FPS_IN_TITLE true


GLuint vao[NUM_VAO];
ProgramID program;

void init(GLFWwindow* window);
void process(GLFWwindow* window, double delta);

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


void init(GLFWwindow* window) {
    program = create_shader_program();

    glGenVertexArrays(NUM_VAO, vao);
    glBindVertexArray(vao[0]);
}


float x = 0.0;
float y = 0.5;
float inc_x = 0.008;
float inc_y = 0.005;


void process(GLFWwindow* window, double delta) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    x += inc_x;
    if (x > 1.0)    inc_x = -0.008;
    if (x < -1.0)   inc_x =  0.008;

    y += inc_y;
    if (y > 1.0)    inc_y = -0.005;
    if (y < -1.0)   inc_y =  0.005;

    GLuint s_offset_x = glGetUniformLocation(program, "offset_x");
    GLuint s_offset_y = glGetUniformLocation(program, "offset_y");
    glProgramUniform1f(program, s_offset_x, x);
    glProgramUniform1f(program, s_offset_y, y);

    // glPointSize(10.0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


ProgramID create_shader_program() {
    ProgramID _program = glCreateProgram();
    int link_ok;

    ShaderID v_shader = load_shader("tri.v.glsl", GL_VERTEX_SHADER);
    ShaderID f_shader = load_shader("tri.f.glsl", GL_FRAGMENT_SHADER);
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
