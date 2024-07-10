#include <stdint.h>

#include "./object.h"


struct scene_manager {
    object_t** objects;
    uint32_t size;
};
typedef struct scene_manager scene_manager_t;


scene_manager_t* scene_manager_init();
void scene_manager_destory(scene_manager_t* manager);
