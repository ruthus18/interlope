#pragma once
#include <stdbool.h>
#include <GLFW/glfw3.h>


// #define DISPLAY_CALLBACK_T void (*display_callback)()
// typedef DISPLAY_CALLBACK_T;

typedef GLFWwindow* window_t;


extern window_t window;

void render_init();
void render_update();
void render_post_update();
bool render_check_stop();
void render_stop();
void render_destroy();
void render_link_program();
uint32_t render_compile_shader(const char* path, int shader_type);
bool render_check_error();

uint32_t render_get_uniform_var(const char* var_name);
