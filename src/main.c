#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "logging.h"


#define NUM_VAO 1
GLuint rendering_program;
GLuint vao[NUM_VAO];


void print_engine_info();
void init(GLFWwindow* window);
void display(GLFWwindow* window, double current_time);
GLuint create_shader_program();


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
        display(window, glfwGetTime());
        glfwSwapBuffers(window);  // Draw content, VSync
        glfwPollEvents();         // Handle window-related events (kb, mouse...) 
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
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


void init(GLFWwindow* window) {
    rendering_program = create_shader_program();

    glGenVertexArrays(NUM_VAO, vao);
    glBindVertexArray(vao[0]);
}


void display(GLFWwindow* window, double current_time) {
    glUseProgram(rendering_program);
    glDrawArrays(GL_POINTS, 0, 1);
}


GLuint create_shader_program() {
    const char* v_shader_src =
        "#version 460  \n"
        "void main()  \n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    const char* f_shader_src = 
        "#version 460  \n"
        "out vec4 color;  \n"
        "void main()  \n"
        "{ color = vec4(1.0, 0.0, 5.0, 1.0); }";

    GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(v_shader, 1, &v_shader_src, NULL);
    glShaderSource(f_shader, 1, &f_shader_src, NULL);
    glCompileShader(v_shader);
    glCompileShader(f_shader);

    GLuint vf_program = glCreateProgram();
    glAttachShader(vf_program, v_shader);
    glAttachShader(vf_program, f_shader);
    glLinkProgram(vf_program);

    return vf_program;
}