#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "utils_io.h"

/* GLuint is a cross-platform type, so making more readable macros aliases */
#define ProgramID GLuint
#define ShaderID GLuint


#define NUM_VAO 1

GLuint vao[NUM_VAO];  // Vertex Array Objects
ProgramID rendering_program;


void init(GLFWwindow* window);
void process(GLFWwindow* window, double current_time);

ProgramID create_shader_program();
ShaderID load_shader(const char* path, int shader_type);


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

    while (!glfwWindowShouldClose(window)) {
        process(window, glfwGetTime());

        glfwSwapBuffers(window);  // Draw content, VSync
        glfwPollEvents();         // Handle window-related events (kb, mouse...) 
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}


void init(GLFWwindow* window) {
    rendering_program = create_shader_program();

    glGenVertexArrays(NUM_VAO, vao);
    glBindVertexArray(vao[0]);
}


void process(GLFWwindow* window, double current_time) {
    glUseProgram(rendering_program);

    glPointSize(10.0);
    glDrawArrays(GL_POINTS, 0, 1);
}


ProgramID create_shader_program() {
    ProgramID _program = glCreateProgram();

    ShaderID v_shader = load_shader("point.v.glsl", GL_VERTEX_SHADER);
    ShaderID f_shader = load_shader("point.f.glsl", GL_FRAGMENT_SHADER);

    glAttachShader(_program, v_shader);
    glAttachShader(_program, f_shader);
    glLinkProgram(_program);
    return _program;
}


ShaderID load_shader(const char* file_path, int shader_type) {
    ShaderID shader = glCreateShader(shader_type);

    const char* path = shader_path(file_path);
    const char* file_buffer = load_file(path);

    glShaderSource(shader, 1, &file_buffer, NULL);
    free((void*) path);
    free((void*) file_buffer);

    glCompileShader(shader);
    return shader;
}
