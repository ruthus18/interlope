#include <GL/glew.h>
#include <cglm/cglm.h>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

#include "mesh.h"

#include "../system/logging.h"
#include "../system/file.h"
#include "../config.h"


mesh_t* mesh_create(char* mesh_path) {
    mesh_t* mesh_obj = malloc(sizeof(mesh_t));
    if (mesh_obj == NULL) {
        log_error("Unable to allocate memory for mesh");
        return NULL;
    }
    mesh_obj->indices = NULL;
    mesh_obj->vertices = NULL;
    mesh_obj->textures = NULL;

    mesh_import(mesh_obj, mesh_path);
    return mesh_obj;
}


cgltf_options options = {0};
cgltf_data* data = NULL;
cgltf_result result;


void mesh_import(mesh_t* mesh_obj, char* mesh_path) {
    glGenVertexArrays(1, &mesh_obj->VAO);
    glGenBuffers(1, &mesh_obj->VBO);
    glGenBuffers(1, &mesh_obj->EBO);

    glBindVertexArray(mesh_obj->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_obj->VBO);
    
    const char* abs_path = asset_path(mesh_path);
    result = cgltf_parse_file(&options, abs_path, &data);
    cgltf_load_buffers(&options, data, abs_path);

    free((void*) abs_path);

    if (result != cgltf_result_success) {
        log_error("Unable to open asset: %s", mesh_path);
    }
    else {
        log_success("Asset loaded: %s", mesh_path);
    }

    // TODO: process all nodes
    if (data -> nodes_count != 1) {
        log_error("Nodes count for model > 1: %s", mesh_path);
        return;
    }

    data->scene->nodes;

}


void mesh_destroy(mesh_t* mesh_obj) {
    cgltf_free(data);
    free(mesh_obj);
}
