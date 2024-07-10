#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "time.h"

#include "./config.h"
#include "./logging.h"


double current_time, last_time, dt, timer_sec;

double current_time = 0.0;       // GetTime value from GLFW
int nbframes = 0;                // num of frames after recent timer reset
int fps = 0;                     // last record of nbframes
bool second_passed = false;      // mark true on every 1 sec frame (timer)


void time_update() {
    last_time = current_time;
    current_time = glfwGetTime();

    dt = current_time - last_time;
    nbframes++;

    timer_sec += dt;

    /* Update vars and reset after 1.0 sec */
    if (timer_sec >= 1.0) {
        second_passed = true; 
        fps = nbframes;
        nbframes = 0;
        timer_sec = 0.0;
    }
    else if (second_passed) second_passed = false;

    if (__DEBUG__PRINT_TIME_UPDATE)  log_info(
        "t_now=%f\tt_last=%f\tt_delta=%f\tt_timer[1s]=%f",
        current_time, last_time, dt, timer_sec
    );
}
