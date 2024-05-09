#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>

// Terminal colors
#define TERM_RED_BG   "\033[0;41m"
#define TERM_CYAN_BG  "\033[0;104m"
#define TERM_RESET    "\033[0;0m"
// More: https://en.wikipedia.org/wiki/ANSI_escape_code#In_C

using namespace std;

void greeting_log();
void print_err(const string& msg);
void main_loop(SDL_Window* window);
bool init_resources();
void free_resources();
void main_loop(SDL_Window* window);


int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_GLContext gl_context;
    GLenum glew_status;

    greeting_log();
    // SDL init
    SDL_Init(SDL_INIT_VIDEO); 

    window = SDL_CreateWindow(
        "Interlope",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1920, 1080,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
    );
    gl_context = SDL_GL_CreateContext(window);
    cout << "SDL Initialized" << endl;

    // Extension wrangler init
    glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        print_err("Error: glewInit: " + to_string(glew_status));
        return EXIT_FAILURE;
    }

    if (!init_resources())
        return EXIT_FAILURE;

    main_loop(window);
    free_resources();

    return EXIT_SUCCESS;
}


/* ====== Logging ====== */

void greeting_log() {
    cout
        << TERM_CYAN_BG
        << "------ Interlope Engine ------"
        << TERM_RESET << endl;
}

void print_err(const string& msg) {
    cerr << TERM_RED_BG << "[!] " <<  msg << TERM_RESET << endl;
}

// Print compilation errors from the OpenGL shader compiler
//
// void print_shader_err(GLuint object) {
//     GLint log_len = 0;
//     if (glIsShader(object)) {
//         glGetShaderiv()
//     }
// }


/* ====== Resource Loading, Shaders ====== */

GLuint program;
GLint attribute_coord2d;


char* file_read(const char* filename) {
    SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
    if (rw == NULL) return NULL;

    Sint64 res_size = SDL_RWsize(rw);
    char* res = (char*) malloc(res_size + 1);
    char* buf = res;

    Sint64 nb_read_total = 0, nb_read = 1;
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

bool init_resources() {
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;

    // Vertex Shader
    GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* v_shader_source = 
        "#version 120\n"
        "attribute vec2 coord2d;"
        "void main(void) {"
        "    gl_Position = vec4(coord2d, 0.0, 1.0);"
        "}";

    glShaderSource(v_shader, 1, &v_shader_source, NULL);
    glCompileShader(v_shader);
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        cerr << "Error in vertex shader" << endl;
        // return false;
    }

    // Fragment Shader
    GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* f_shader_source = 
        "#version 120\n"
        "void main(void) {"
        "    gl_FragColor[0] = gl_FragCoord.x/1920.0;"
        "    gl_FragColor[1] = gl_FragCoord.y/1080.0;"
        "    gl_FragColor[2] = 1;"
        "}";

    glShaderSource(f_shader, 1, &f_shader_source, NULL);
    glCompileShader(f_shader);
    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
        cerr << "Error in fragment shader" << endl;
        // return false;
    }

    // GLSL Program
    program = glCreateProgram();
    glAttachShader(program, v_shader);
    glAttachShader(program, f_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        cerr << "Error in glLinkProgram" << endl;
    }

    // Bind vertex coords
    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord2d == -1) {
        cerr << "Could not bind attribute " << attribute_name << endl;
    }

    return true;
}

void free_resources() {
    glDeleteProgram(program);
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
