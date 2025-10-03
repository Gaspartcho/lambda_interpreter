#include <stdlib.h>
#include <string.h>


#include "../utils/utils.h"
#include "runtime.h"


void get_var_name(name_t name, depth_t var) {

	size_t i = 0;

	do {
		name[i] = 'a' + var % NB_LETTERS;
		var /= NB_LETTERS;
		i++;
	} while (var >= NB_LETTERS);

	name[i] = BLANK;
}


size_t get_var_size(depth_t var) {
	size_t size = 1;
	while (var >= NB_LETTERS) {
		var /= NB_LETTERS;
		size++;
	}
	return size;
}


size_t get_node_size(struct node_t* node) {

	if (node == NULL) return 0;

	switch (node->type) {
		case Variable:	  return get_var_size(node->ref->depth);
		case Function:	  return get_node_size(node->body) + get_var_size(node->depth) + 3;
		case Application: return get_node_size(node->func) + get_node_size(node->arg) + 4;
		case Macro:		  return strlen(node->token->name);
		case Directive:	  return get_node_size(node->next) + 1;
		case String:	  return strlen(node->str) + 2;
	}

	return 0;
}


void write_node(char* buffer, struct node_t* node) {

	if (node == NULL) return;

	switch (node->type) {
		case Variable: get_var_name(buffer, node->ref->depth); break;
		case Function: {
			size_t name_size = get_var_size(node->depth);

			buffer[0] = FUNC_DEF;
			get_var_name(buffer + 1, node->depth);
			buffer[name_size + 1] = DOT;
			buffer[name_size + 2] = SPACE;
			write_node(buffer + name_size + 3, node->body);

			break;
		}

		case Application: {
			size_t func_size = get_node_size(node->func);
			size_t arg_size	 = get_node_size(node->arg);

			buffer[0] = APPLY_OPEN;
			write_node(buffer + 1, node->func);
			buffer[func_size + 1] = COMMA;
			buffer[func_size + 2] = SPACE;
			write_node(buffer + func_size + 3, node->arg);
			buffer[func_size + arg_size + 3] = APPLY_CLOSE;

			break;
		}

		case Macro: strcpy(buffer, node->token->name); break;

		case Directive: {
			buffer[0] = get_dire_symbol(node->dire);
			write_node(buffer + 1, node->next);
			break;
		}

		case String:
			buffer[0] = STRING_OPEN;
			strcpy(buffer + 1, node->str);
			buffer[strlen(node->str) + 1] = STRING_CLOSE;
	}
}


char* get_node_str(struct node_t* node) {

	size_t size	  = get_node_size(node);
	char*  buffer = malloc(sizeof(*buffer) * (size + 1));
	buffer[0]	  = BLANK;
	write_node(buffer, node);
	buffer[size] = BLANK;
	return buffer;
}
