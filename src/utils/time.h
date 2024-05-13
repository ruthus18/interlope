#pragma once
#include <stdbool.h>


double update_time();    // call on every frame
int get_fps();           // get FPS count for last second
bool second_passed();    // true if 1 second passed, false otherwise
