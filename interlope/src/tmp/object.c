#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <cglm/cglm.h>

#include "object.h"

#include "../system/logging.h"


static vec3 V_POS_DEFAULT = {0.0, 0.0, 0.0};
static vec3 V_ROT_DEFAULT = {0.0, 0.0, 0.0};
static vec3 V_SCALE_DEFAULT = {1.0, 1.0, 1.0};


object_t* object_create() {
    object_t* obj = malloc(sizeof(object_t));

    memcpy(obj->v_pos, V_POS_DEFAULT, sizeof(vec3));
    memcpy(obj->v_rot, V_ROT_DEFAULT, sizeof(vec3));
    memcpy(obj->v_scale, V_SCALE_DEFAULT, sizeof(vec3));
    obj->id = 0;

    return obj;
}


void object_destroy(object_t* obj) {
    free(obj);
}


void object_log(object_t* obj) {
    log_info("id: %i", obj->id);
    log_info("P: (%f  %f  %f)", obj->v_pos[0], obj->v_pos[1], obj->v_pos[2]);
    log_info("R: (%f  %f  %f)", obj->v_rot[0], obj->v_rot[1], obj->v_rot[2]);
    log_info("S: (%f  %f  %f)", obj->v_scale[0], obj->v_scale[1], obj->v_scale[2]);
}
