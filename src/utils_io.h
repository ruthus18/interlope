#pragma once

// Terminal colors
#define TERM_GREEN   "\033[0;32m"
#define TERM_RED_BG   "\033[0;41m"
#define TERM_CYAN_BG  "\033[0;104m"
#define TERM_RESET    "\033[0;0m"
// More: https://en.wikipedia.org/wiki/ANSI_escape_code#In_C


void greeting_log(const char* msg, ...);
void info_log(const char* msg, ...);
void success_log(const char* msg, ...);
void error_log(const char* msg, ...);
void print_engine_info();

char* shader_path(const char* file_path);
char* load_file(const char* path);
