#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "./render.h"
#include "./config.h"
#include "./logging.h"
#include "./file.h"
#include "./time.h"


bool should_stop = false;
window_t window;
uint32_t program;


void render_init() {
    setbuf(stdout, NULL);
    
    if (!glfwInit()) {
        log_error("Could not initialize GLFW!");
        exit(EXIT_FAILURE);
    }
    /* Set GL context options */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, NULL, NULL
    );
    glfwMakeContextCurrent(window);

    int glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        log_error("Could not initialize GLEW, status=%s", glew_status);
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);  // VSync
    program = glCreateProgram();
}


void render_destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


void render_update() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    glEnable(GL_CULL_FACE);

    time_update();
}


void render_post_update() {
    if (__DEBUG__SHOW_FPS_IN_TITLE && second_passed) {
        char title_buf[128];
        sprintf(title_buf, "%s [%i FPS]", SCREEN_TITLE, fps);
        glfwSetWindowTitle(window, title_buf);
    }
    glfwSwapBuffers(window);  // Draw content, VSync
}


bool render_check_stop() {
    return glfwWindowShouldClose(window) || should_stop;
}


void render_stop() {
    should_stop = true;
}


void render_link_program() {
    int link_ok;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (link_ok != 1) {
        log_error("GL program linking error");
        log_glprogram(program);
    }
}


uint32_t render_compile_shader(const char* file_path, int shader_type) {
    uint32_t shader = glCreateShader(shader_type);
    int compile_ok;

    const char* path = shader_path(file_path);
    const char* file_buffer = load_file(path);

    glShaderSource(shader, 1, &file_buffer, NULL);
    free((void*) path);
    free((void*) file_buffer);

    glCompileShader(shader);
    render_check_error();
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);

    if (compile_ok == 1) {
        log_success("Shader compiled: %s", file_path);
        glAttachShader(program, shader);
    }
    else {
        log_error("Shader compilation error; file=%s", file_path);
        log_glshader(shader);
    }

    return shader;
}


bool render_check_error() {
    bool is_err = false;
    int gl_err = glGetError();

    while (gl_err != GL_NO_ERROR) {
        log_error("OpenGL Error: %s", gl_err);
        is_err = true;
    }
    return is_err;
}


uint32_t render_get_uniform_var(const char* var_name) {
    return glGetUniformLocation(program, var_name);
}
