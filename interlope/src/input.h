#include <stdbool.h>

extern double mouse_x, mouse_y, mouse_delta_x, mouse_delta_y;  // readonly

void input_init();
// void input_set_key_callback();
bool input_is_kb_key_pressed(int key);

void input_update_keyboard();
void input_update_mouse();

