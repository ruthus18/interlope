#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "logging.h"


void print_engine_info();
void init(GLFWwindow* window);
void display(GLFWwindow* window, double current_time);


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


/* App Initialization */
//
void init(GLFWwindow* window) {}


/* Rendering Loop */
//
void display(GLFWwindow* window, double current_time) {

    // Set color to be applied when clearing the color buffer
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
