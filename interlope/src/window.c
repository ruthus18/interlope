#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define SCREEN_WIDTH   1366
#define SCREEN_HEIGHT  768
#define SCREEN_ASPECT  (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT
#define SCREEN_TITLE   "Interlope Engine"


typedef char result_t;
typedef GLFWwindow* window_t;


bool should_stop = false;
window_t window;
uint32_t program;


result_t window_init() {
    setbuf(stdout, NULL);
    if (!glfwInit()) {
        printf("[C] Could not initialize GLFW!\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, NULL, NULL
    );
    glfwMakeContextCurrent(window);

    int glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        printf("[C] Could not initialize GLEW, status=%s\n", glew_status);
        return EXIT_FAILURE;
    }
    glfwSwapInterval(1);  // VSync
    program = glCreateProgram();

    printf("[C] Window initialized\n");
    return EXIT_SUCCESS;
}


void window_destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


void window_update() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);

    glEnable(GL_CULL_FACE);

    // time_update();
    // (*display_callback)();

    // if (__DEBUG__SHOW_FPS_IN_TITLE && second_passed) {
    //     char title_buf[128];
    //     sprintf(title_buf, "%s [%i FPS]", SCREEN_TITLE, fps);
    //     glfwSetWindowTitle(window, title_buf);
    // }
    glfwSwapBuffers(window);  // Draw content, VSync
}

// void window_init() {
//     vec3 v_test = {3.0, 5.0, 9.0};
//     mat4 m_test;
//     glm_mat4_identity(m_test);
//     glm_translate(m_test, v_test);

//     printf("[C] Hello from C!\n");
//     printf("[C] |%.2f\t%.2f\t%.2f\t%.2f |\n", m_test[0][0], m_test[0][1], m_test[0][2], m_test[0][3]);
//     printf("    |%.2f\t%.2f\t%.2f\t%.2f |\n", m_test[1][0], m_test[1][1], m_test[1][2], m_test[1][3]);
//     printf("    |%.2f\t%.2f\t%.2f\t%.2f |\n", m_test[2][0], m_test[2][1], m_test[2][2], m_test[2][3]);
//     printf("    |%.2f\t%.2f\t%.2f\t%.2f |\n", m_test[3][0], m_test[3][1], m_test[3][2], m_test[3][3]);
// }
