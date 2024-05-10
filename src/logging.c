#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

#include "logging.h"


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
    fprintf(stderr, "%s[X] ", TERM_RED_BG);

    va_list argp;
    va_start(argp, msg);
    vfprintf(stderr, msg, argp);
    va_end(argp);

    fprintf(stderr, "%s\n", TERM_RESET);
}

/* Print compilation errors from the OpenGL shader compiler */
//
void opengl_error_log(GLuint object) {
    char* log_msg;
    int log_len;
    bool is_shader = glIsShader(object);
    bool is_program = glIsProgram(object);

    if (is_shader)
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_len);

    else if (is_program)
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_len);

    else {
        error_log("[opengl_error_log] object is not a shader or program");
        return;
    }
    log_msg = (char*) malloc(log_len);

    if (is_shader)
        glGetShaderInfoLog(object, log_len, NULL, log_msg);

    else if (is_program)
        glGetProgramInfoLog(object, log_len, NULL, log_msg);

    error_log(log_msg);
    free(log_msg);
}