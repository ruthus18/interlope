#pragma once
#include <stdbool.h>


extern int fps;               // current FPS
extern bool second_passed;    // simple timer --
                              //    true if 1 second passed, false otherwise

double update_time();         // call on every frame to fill vars above
