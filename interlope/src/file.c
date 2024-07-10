#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

/* read file by relative app `path` */
//
char* load_file(const char* path) {
    char* buffer = 0;
    long len;
    FILE * file = fopen(path, "rb");

    if (file) {
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        fseek(file, 0, SEEK_SET);

        buffer = (char*) malloc((len+1) * sizeof(char));
        if (buffer)
            fread(buffer, sizeof(char), len, file);
        
        fclose(file);
    }
    buffer[len] = '\0';
    return buffer;
}


/*  concatenate shader filename with base shader dir     */
/*                                                       */
/*  example:  shader_path("f.vert") -> "shaders/f.vert"  */
//
char* shader_path(const char* file_path) {
    char* path = malloc(strlen(SHADERS_DIR) + strlen(file_path) + 1);
    strcpy(path, SHADERS_DIR);
    strcat(path, file_path);
    return path;
}


char* asset_path(const char* rel_path) {
    char* path = malloc(strlen(ASSETS_DIR) + strlen(rel_path) + 1);
    strcpy(path, ASSETS_DIR);
    strcat(path, rel_path);
    return path;
}
