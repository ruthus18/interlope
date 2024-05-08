#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>

using namespace std;


GLuint program;
GLint attribute_coord2d;


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
        "    gl_FragColor[0] = 0.0;"
        "    gl_FragColor[1] = 0.0;"
        "    gl_FragColor[2] = 1.0;"
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
        0.0, 0.8,
        -0.8, -0.8,
        0.8, 0.8,
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
    glDrawArrays(GL_TRIANGLES, 0, 3);

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


int main(int argc, char* argv[]) {
    // SDL init
    SDL_Init(SDL_INIT_VIDEO); 

    SDL_Window* window = SDL_CreateWindow(
        "Triangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1920, 1080,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
    );
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // Extension wrangler init
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        cerr << "Error: glewInit: " << glew_status << endl;
        return EXIT_FAILURE;
    }

    if (!init_resources())
        return EXIT_FAILURE;

    main_loop(window);
    free_resources();

    return EXIT_SUCCESS;
}
