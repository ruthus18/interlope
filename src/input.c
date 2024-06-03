#include <math.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "input.h"

#include "config.h"
#include "render.h"


double mouse_x, mouse_y, mouse_last_x, mouse_last_y, mouse_delta_x, mouse_delta_y;

void init_input() {
    /* Mouse setup */
    mouse_x = 0.0;
    mouse_y = 0.0;
    mouse_last_x = 0.0;
    mouse_last_y = 0.0;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void update_input() {
    /* Mouse */
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    
    mouse_delta_x = (mouse_x - mouse_last_x) / SCREEN_WIDTH;
    mouse_delta_y = (mouse_y - mouse_last_y) / SCREEN_HEIGHT;
    mouse_last_x = mouse_x;
    mouse_last_y = mouse_y;

    /* Keyboard */
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        render_stop();
}
