#include <cglm/cglm.h>


struct mesh_vertex {
    vec3 position;
    vec3 normal;
    vec2 texture;
};
typedef struct mesh_vertex mesh_vertex_t;

struct mesh_texture {
    uint32_t id;
    char* type;
};
typedef struct mesh_texture mesh_texture_t;

struct mesh {
    uint32_t** indices;
    mesh_vertex_t** vertices;
    mesh_texture_t** textures;

    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;
};
typedef struct mesh mesh_t;


mesh_t* mesh_create(char* mesh_path);
void mesh_import(mesh_t* mesh_obj, char* mesh_path);
void mesh_destroy(mesh_t* mesh_obj);
