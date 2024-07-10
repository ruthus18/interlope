#include <cglm/cglm.h>
#include <stdint.h>

struct object {
    vec3 v_pos;
    vec3 v_rot;
    vec3 v_scale;

    uint32_t id;
};
typedef struct object object_t;


object_t* object_create();
void object_destroy(object_t* obj);

void object_log(object_t* obj);
