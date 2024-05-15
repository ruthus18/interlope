#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../config.h"
#include "./time.h"
#include "./io.h"


double current_time, last_time, delta, timer;

bool sec_passed = false;   // mark true on every 1 sec frame
int nbframes = 0;          // num of frames after recent timer reset
int fps = 0;               // last record of nbframes


double update_time() {
    last_time = current_time;
    current_time = glfwGetTime();

    delta = current_time - last_time;
    nbframes++;

    timer += delta;

    /* Update vars and reset after 1.0 sec */
    if (timer >= 1.0) {
        sec_passed = true; 
        fps = nbframes;
        nbframes = 0;
        timer = 0.0;
    }
    else sec_passed = false;

    if (__DEBUG__PRINT_TIME_UPDATE)  info_log(
        "t_now=%f  t_last=%f  t_delta=%f  timer=%f",
        current_time, last_time, delta, timer
    );
    return current_time;
}

int get_fps() { return fps; }

bool second_passed() { return sec_passed; }
