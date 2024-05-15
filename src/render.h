#include <stdbool.h>

#include "types.h"

#define frame_callback_t void (*frame_callback)(double)
typedef frame_callback_t;


void render_init();
void render_run(frame_callback_t);
void render_close();

void link_gl_program();
ShaderID load_shader(const char* path, int shader_type);
bool check_opengl_error();

GLVar get_uniform_var(const char* var_name);
