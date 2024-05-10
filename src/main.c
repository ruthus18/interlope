#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "logging.h"

#define VERSION "0.0.0dev"
#define SHADERS_DIR "shaders/"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080



bool init_resources();
void free_resources();
char* file_read(const char* filename);
GLuint create_shader(const char* filename, GLenum shader_type);
void render(SDL_Window* window);
void main_loop(SDL_Window* window);
void update_window();

/* Rendering */
GLuint program;
GLuint vertex_buffer;
GLint attribute_coord2d, attribute_v_color;
GLint uniform_fade;


int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_GLContext context;
    GLenum glew_status;

    greeting_log("======  Interlope Engine  ======");
    info_log("VERSION: %s", VERSION);
    info_log("SHADERS DIR: %s", VERSION);
    info_log("RESOLUTION: %i x %i", SCREEN_WIDTH, SCREEN_HEIGHT);
    info_log("------");

    // SDL init
    SDL_Init(SDL_INIT_VIDEO); 

    window = SDL_CreateWindow(
        "Interlope",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_OPENGL
    );
    if (window == NULL) {
        error_log("Unable to create SDL window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Context vars for SDL & OpenGL
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);

    context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        error_log("Unable to create SDL context: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    success_log("SDL Initialized");

    // Extension wrangler init
    glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        error_log("Error on init GLEW: %i", glew_status);
        return EXIT_FAILURE;
    }

    if (!init_resources()) {
        error_log("Unable to init resources");
        return EXIT_FAILURE;
    }
    success_log("Resouces initialized");

    // Enable alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    success_log("Entering main loop");
    main_loop(window);

    info_log("Shutdown engine");
    free_resources();

    SDL_Quit();
    return EXIT_SUCCESS;
}


void main_loop(SDL_Window* window) {
    while (true) {
        SDL_Event event;
        while (true) {
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) {
                return;
            }
            glUseProgram(program);
            update_window();
            render(window);
        }
    }
}


void update_window() {
    uint32_t delta = SDL_GetTicks();
    float cur_fade = sinf(delta / 1000.0 * (2*3.14) / 5) / 2 + 0.5;

    glUniform1f(uniform_fade, cur_fade);
}


/* ====== Rendering ====== */

bool init_resources() {
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
    GLuint v_shader, f_shader;

    float vertex_data[] = {
        -0.5,  0.5,     1.0, 0.0, 0.0,
        -0.5, -0.5,     1.0, 1.0, 0.0,
         0.5,  0.5,     1.0, 0.0, 0.0,
         0.5,  0.5,     1.0, 0.0, 0.0,
        -0.5, -0.5,     1.0, 1.0, 0.0,
         0.5, -0.5,     1.0, 1.0, 0.0
    };

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    /* Init shaders */
    v_shader = create_shader("triangle.v.glsl", GL_VERTEX_SHADER);
    f_shader = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER);

    /* GLSL Compiling Shaders */
    program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        error_log("Error in glLinkProgram");
        opengl_error_log(program);
        return false;
    }
    // ------

    const char* attribute_name;

    // Bind vertex coords
    attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord2d == -1) {
        error_log("Could not bind GLSL attribute: %s", attribute_name);
    }

    // Bind color coords
    attribute_name = "v_color";
    attribute_v_color = glGetAttribLocation(program, attribute_name);
    if (attribute_v_color == -1) {
        error_log("Could not bind GLSL attribute: %s", attribute_name);
    }

    // Bind fading
    const char* uniform_name = "fade";
    uniform_fade = glGetUniformLocation(program, uniform_name);
    if (uniform_fade == -1) {
        error_log("Could not bind uniform: %s", uniform_name);
    }

    return true;
}


void free_resources() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vertex_buffer);
}


char* file_read(const char* filename) {
    SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
	if (rw == NULL) return NULL;
	
	Sint64 res_size = SDL_RWsize(rw);
	char* res = (char*)malloc(res_size + 1);

	Sint64 nb_read_total = 0, nb_read = 1;
	char* buf = res;
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
		free(res);
		return NULL;
	}
	
	res[nb_read_total] = '\0';
	return res;
}


GLuint create_shader(const char* filename, GLenum shader_type) {
    const char* shader_source;
    GLuint shader;
    GLint compile_ok;

    char* full_path = malloc(strlen(SHADERS_DIR) + strlen(filename) + 1);
    strcpy(full_path, SHADERS_DIR);
    strcat(full_path, filename);

    shader_source = file_read(full_path);
    free(full_path);

    if (shader_source == NULL) {
        error_log("Error on file read: %s", SDL_GetError());
        return 0;
    }

    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    free((void*)shader_source);

    glCompileShader(shader);
    
    compile_ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        error_log("Error on loading shader: %s", filename);
        opengl_error_log(shader);
        return 0;
    }

    info_log("Shader loaded: %s", filename);
    return shader;
}


void render(SDL_Window* window) {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // color bg in white
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glEnableVertexAttribArray(attribute_coord2d);
    glEnableVertexAttribArray(attribute_v_color);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    int attr_len = 5 * sizeof(float);
    glVertexAttribPointer(
        attribute_coord2d,  // attribute
        2,                  // num of elements per vertex (x,y)
        GL_FLOAT,           // the type of each value
        GL_FALSE,           // take our values as-is
        attr_len,           // no extra data between each position
        0                   // offset of first element
    );
    glVertexAttribPointer(
        attribute_v_color,
        3,                  
        GL_FLOAT,           
        GL_FALSE,           
        attr_len,           
        (void*) (2* sizeof(float))
    );
    
    // Push each element in buffer_vertices to the vertex shader
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(attribute_coord2d);
    glDisableVertexAttribArray(attribute_v_color);

    SDL_GL_SwapWindow(window);
}
