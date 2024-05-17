#pragma once

// Terminal colors
#define TERM_RED   "\033[0;31m"
#define TERM_GREEN   "\033[0;32m"
#define TERM_CYAN_BG  "\033[0;104m"
#define TERM_RESET    "\033[0;0m"
// More: https://en.wikipedia.org/wiki/ANSI_escape_code#In_C


void greeting_log(const char* msg, ...);
void info_log(const char* msg, ...);
void success_log(const char* msg, ...);
void error_log(const char* msg, ...);
void print_engine_info();

void shader_log(uint32_t shader);
void program_log(uint32_t program);

char* shader_path(const char* file_path);
char* load_file(const char* path);
