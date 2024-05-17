#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "render.h"
#include "utils/io.h"
#include "utils/time.h"


GLFWwindow* window;
uint32_t program;

static double time_;


void render_init() {
    if (!glfwInit()) {
        error_log("Could not initialize GLFW!");
        exit(EXIT_FAILURE);
    }
    /* Set GL context options */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window = glfwCreateWindow(
        SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, NULL, NULL
    );
    glfwMakeContextCurrent(window);

    int glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        error_log("Could not initialize GLEW, status=%s", glew_status);
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);  // VSync
    program = glCreateProgram();
}


void render_display(DISPLAY_CALLBACK_T) {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_DEPTH_BUFFER_BIT);
        // glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);

        time_ = update_time();
        (*display_callback)(time_);

        if (__DEBUG__SHOW_FPS_IN_TITLE && second_passed) {
            char title_buf[128];
            sprintf(title_buf, "%s [%i FPS]", SCREEN_TITLE, fps);
            glfwSetWindowTitle(window, title_buf);
        }

        glfwSwapBuffers(window);  // Draw content, VSync
        glfwPollEvents();         // Handle window-related events (kb, mouse...) 
    }
}


void render_close() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


void link_gl_program() {
    int link_ok;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (link_ok != 1) {
        error_log("GL program linking error");
        program_log(program);
    }
}


uint32_t load_shader(const char* file_path, int shader_type) {
    uint32_t shader = glCreateShader(shader_type);
    int compile_ok;

    const char* path = shader_path(file_path);
    const char* file_buffer = load_file(path);

    glShaderSource(shader, 1, &file_buffer, NULL);
    free((void*) path);
    free((void*) file_buffer);

    glCompileShader(shader);
    check_opengl_error();
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);

    if (compile_ok == 1) {
        success_log("Shader loaded: %s", file_path);
        glAttachShader(program, shader);
    }
    else {
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


uint32_t get_uniform_var(const char* var_name) {
    return glGetUniformLocation(program, var_name);
}
