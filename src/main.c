#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "logging.h"


void init(GLFWwindow* window) {}


void display(GLFWwindow* window, double current_time) {
    glClearColor(0.8, 0.8, 0.8, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}


int main() {
    if (!glfwInit()) {
        error_log("Could not initialize GLFW!");
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(
        SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, NULL, NULL
    );
    glfwMakeContextCurrent(window);

    int glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        error_log("Could not initialize GLEW, status=%s", glew_status);
        return EXIT_FAILURE;
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
