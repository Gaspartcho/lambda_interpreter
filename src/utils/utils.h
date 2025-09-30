#pragma once


#include "../core/core.h"
#include "array.h"


#ifdef linux
#define FILE_SEPARATOR '/'
#endif

#ifdef _WIN32
#define FILE_SEPARATOR '\\'
#endif

#define XOR(a, b) ((a || b) && !(a && b))


extern char get_dire_symbol(directive_t dire);

extern struct token_t* get_token_by_ref(struct array_t* array, struct node_t* ref);
extern struct token_t* get_token_by_name(struct array_t* array, name_t name);

extern void free_mac_token(void* token);

extern void get_file_path_from_relative_path(char** buffer, char* path, char* relative_path);
