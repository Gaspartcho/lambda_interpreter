#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../error/error.h"
#include "utils.h"


struct token_t* get_token_by_ref(struct array_t* array, struct node_t* ref) {
	ITERATE_ARRAY(array, i) if (((struct token_t*)array->content[i])->ref == ref) return array->content[i];
	return NULL;
}


struct token_t* get_token_by_name(struct array_t* array, name_t name) {
	ITERATE_ARRAY(array, i) if (!strcmp(((struct token_t*)array->content[i])->name, name)) return array->content[i];
	return NULL;
}


void free_mac_token(void* token) {
	free_node(((struct token_t*)token)->ref);
	free(token);
}


char get_dire_symbol(directive_t dire) {
	return (unsigned long)dire(NULL, NULL, NULL, NULL) % CHAR_MAX;
}


void get_file_path_from_relative_path(char** buffer, char* path, char* relative_path) {

	char* pos = strrchr(path, FILE_SEPARATOR);
	if (pos == NULL) error_s(E, path);

	*pos = '\0';

	*buffer = malloc(sizeof(**buffer) * (strlen(path) + strlen(relative_path) + 2));

	if (relative_path[0] == '/') strcpy(*buffer, relative_path);
	else sprintf(*buffer, "%s%c%s", path, FILE_SEPARATOR, relative_path);

	*pos = FILE_SEPARATOR;
}
