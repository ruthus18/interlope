#include <math.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "input.h"

#include "../config.h"
#include "../system/logging.h"

#include "./render.h"


double mouse_x, mouse_y, mouse_last_x, mouse_last_y, mouse_delta_x, mouse_delta_y;

void input_init() {
    /* Mouse setup */
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    mouse_last_x = mouse_x;
    mouse_last_y = mouse_y;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


bool input_is_kb_key_pressed(int key) {
    return (glfwGetKey(window, key) == GLFW_PRESS);
}


void input_update_keyboard() {
    glfwPollEvents();

    if (input_is_kb_key_pressed(GLFW_KEY_ESCAPE))
        render_stop();
}


void input_update_mouse() {
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    
    mouse_delta_x = (mouse_x - mouse_last_x) / SCREEN_WIDTH;
    mouse_delta_y = -(mouse_y - mouse_last_y) / SCREEN_HEIGHT;
    mouse_last_x = mouse_x;
    mouse_last_y = mouse_y;

    // info_log(
    //     "t=%f\tΔmouse_x=%f\tΔmouse_y=%f", time, mouse_delta_x, mouse_delta_y
    // );
}
