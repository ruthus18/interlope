#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>

// Terminal colors
#define TERM_GREEN   "\033[0;32m"
#define TERM_RED_BG   "\033[0;41m"
#define TERM_CYAN_BG  "\033[0;104m"
#define TERM_RESET    "\033[0;0m"
// More: https://en.wikipedia.org/wiki/ANSI_escape_code#In_C

#define VERSION "0.0.0dev"
#define SHADERS_DIR "shaders/"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080


void greeting_log(const char* msg, ...);
void info_log(const char* msg, ...);
void success_log(const char* msg, ...);
void error_log(const char* msg, ...);
void opengl_error_log(GLuint object);
char* file_read(const char* filename);
GLuint create_shader(const char* filename, GLenum shader_type);

bool init_resources();
void free_resources();
void main_loop(SDL_Window* window);
void render(SDL_Window* window);


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
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
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

    return EXIT_SUCCESS;
}


/* ====== Logging ====== */

void greeting_log(const char* msg, ...) {
    fprintf(stdout, "%s", TERM_CYAN_BG);

    va_list argp;
    va_start(argp, msg);
    vfprintf(stdout, msg, argp);
    va_end(argp);

    fprintf(stdout, "%s\n", TERM_RESET);
}

void info_log(const char* msg, ...) {
    va_list argp;
    va_start(argp, msg);
    vfprintf(stdout, msg, argp);
    va_end(argp);
    fprintf(stdout, "\n");
}

void success_log(const char* msg, ...) {
    fprintf(stdout, "%s[+] ", TERM_GREEN);

    va_list argp;
    va_start(argp, msg);
    vfprintf(stdout, msg, argp);
    va_end(argp);

    fprintf(stdout, "%s\n", TERM_RESET);
}

void error_log(const char* msg, ...) {
    fprintf(stderr, "%s[X] ", TERM_RED_BG);

    va_list argp;
    va_start(argp, msg);
    vfprintf(stderr, msg, argp);
    va_end(argp);

    fprintf(stderr, "%s\n", TERM_RESET);
}

/* Print compilation errors from the OpenGL shader compiler */
//
void opengl_error_log(GLuint object) {
    char* log_msg;
    int log_len;
    bool is_shader = glIsShader(object);
    bool is_program = glIsProgram(object);

    if (is_shader)
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_len);

    else if (is_program)
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_len);

    else {
        error_log("[opengl_error_log] object is not a shader or program");
        return;
    }
    log_msg = (char*) malloc(log_len);

    if (is_shader)
        glGetShaderInfoLog(object, log_len, NULL, log_msg);

    else if (is_program)
        glGetProgramInfoLog(object, log_len, NULL, log_msg);

    error_log(log_msg);
    free(log_msg);
}


/* ====== Resource Loading, Shaders ====== */

GLuint program;
GLint attribute_coord2d;


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

bool init_resources() {
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
    GLuint v_shader, f_shader;

    v_shader = create_shader("triangle.v.glsl", GL_VERTEX_SHADER);
    f_shader = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER);

    // GLSL Program
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

    // Bind vertex coords
    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord2d == -1) {
        error_log("Could not bind attribute ");
    }

    return true;
}

void free_resources() {
    glDeleteProgram(program);
}

void main_loop(SDL_Window* window) {
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return;
            }
            render(window);
        }
    }
}

void render(SDL_Window* window) {
    glClearColor(1.0, 1.0, 1.0, 1.0);  // color bg in white
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord2d);

    // Describe mesh
    GLfloat triangle_vertices[] = {
        -0.5, 0.5,
        -0.5, -0.5,
        0.5, 0.5,
        0.5, 0.5,
        -0.5, -0.5,
        0.5, -0.5
    };
    glVertexAttribPointer(
        attribute_coord2d,  // attribute
        2,                  // num of elements per vertex (x,y)
        GL_FLOAT,           // the type of each value
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        triangle_vertices   // pointer to the C array
    );
    
    // Push each element in buffer_vertices to the vertex shader
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(attribute_coord2d);

    SDL_GL_SwapWindow(window);
}
