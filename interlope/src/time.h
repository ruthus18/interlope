#pragma once
#include <stdbool.h>


extern double current_time;
extern double dt;           // time delta 
extern int fps;             // current FPS
extern bool second_passed;  // simple timer --
                            //    true if 1 second passed, false otherwise

void time_update();         // call on every frame to fill vars above
