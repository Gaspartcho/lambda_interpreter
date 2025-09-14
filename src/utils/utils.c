#include <stdlib.h>
#include <string.h>


#include "utils.h"


struct token_t* get_token_by_ref(struct array_t* array, struct node_t* ref) {
	for (size_t i = 0; i < array->size; i++) {
		if (((struct token_t*)array->content[i])->ref == ref) return array->content[i];
	}

	return NULL;
}

struct token_t* get_token_by_name(struct array_t* array, name_t name) {
	for (size_t i = 0; i < array->size; i++) {
		if (!strcmp(((struct token_t*)array->content[i])->name, name)) return array->content[i];
	}

	return NULL;
}


void free_mac_token(void* token) {
	free_node(((struct token_t*)token)->ref);
	free(token);
}
