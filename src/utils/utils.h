#pragma once


#include "../core/core.h"


extern struct token_t* get_token_by_ref(struct array_t* array, struct node_t* ref);
extern struct token_t* get_token_by_name(struct array_t* array, name_t name);

extern void free_mac_token(void* token);
