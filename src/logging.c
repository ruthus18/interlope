#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "logging.h"


/* =====  Output  ===== */

void greeting_log(const char* msg, ...) {
    fprintf(stdout, "%s", TERM_CYAN_BG);

    va_list argp;
    va_start(argp, msg);
    vfprintf(stdout, msg, argp);
    va_end(argp);

    fprintf(stdout, "%s\n", TERM_RESET);
}


void info_log(const char* msg, ...) {
    va_list argp;
    va_start(argp, msg);
    vfprintf(stdout, msg, argp);
    va_end(argp);
    fprintf(stdout, "\n");
}


void success_log(const char* msg, ...) {
    fprintf(stdout, "%s[+] ", TERM_GREEN);

    va_list argp;
    va_start(argp, msg);
    vfprintf(stdout, msg, argp);
    va_end(argp);

    fprintf(stdout, "%s\n", TERM_RESET);
}


void error_log(const char* msg, ...) {
    fprintf(stderr, "%s[X] ", TERM_RED);

    va_list argp;
    va_start(argp, msg);
    vfprintf(stderr, msg, argp);
    va_end(argp);

    fprintf(stderr, "%s\n", TERM_RESET);
}


void print_engine_info() {
    greeting_log("======  Interlope Engine  ======");
    info_log("ENGINE VERSION: %s", ENGINE_VERSION);
    info_log("OPENGL VERSION: %s", glGetString(GL_VERSION));
    info_log("GLEW VERSION: %s", glewGetString(GLEW_VERSION));
    info_log("GLFW VERSION: %u.%u.%u", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR);
    info_log("VIDEO DEVICE: %s (%s)", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    info_log("SHADERS DIR: %s", SHADERS_DIR);
    info_log("RESOLUTION: %i x %i", SCREEN_WIDTH, SCREEN_HEIGHT);
    info_log("------");
}


void shader_log(uint32_t shader) {
    int log_len = 0;
    int ch = 0;
    char *log;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
    if (log_len > 0) {
        log = (char*) malloc(log_len);

        glGetShaderInfoLog(shader, log_len, &ch, log);
        info_log("Shader log: %s", log);
        free(log);
    }
}


void program_log(uint32_t program) {
    int log_len = 0;
    int ch = 0;
    char *log;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
    if (log_len > 0) {
        log = (char*) malloc(log_len);

        glGetProgramInfoLog(program, log_len, &ch, log);
        info_log("Program log: %s", log);
        free(log);
    }
}
