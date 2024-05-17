#pragma once
#include <stdbool.h>
#include <GLFW/glfw3.h>


#define DISPLAY_CALLBACK_T void (*display_callback)(double)
typedef DISPLAY_CALLBACK_T;

typedef GLFWwindow* window_t;


extern window_t window;

void render_init();
void render_display(DISPLAY_CALLBACK_T);
void render_close();

void link_gl_program();
uint32_t load_shader(const char* path, int shader_type);
bool check_opengl_error();

uint32_t get_uniform_var(const char* var_name);
