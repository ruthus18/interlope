#include "scene.h"

#include "../system/logging.h"


#define SCENE_SIZE_DEFAULT 100


scene_manager_t* scene_manager_init() {
    scene_manager_t* manager = malloc(sizeof(scene_manager_t));

    if (manager == NULL) {
        log_error("Unable to allocate memory for scene manager");
        return NULL;
    }
    manager->size = SCENE_SIZE_DEFAULT;    
    manager->objects = malloc(sizeof(object_t*) * SCENE_SIZE_DEFAULT);

    if (manager->objects == NULL) {
        log_error("Unable to allocate memory for scene manager objects");
        free(manager);
        return NULL;
    }
    return manager;
}


void scene_manager_destory(scene_manager_t* manager) {
    free(manager->objects);
    free(manager);
}
