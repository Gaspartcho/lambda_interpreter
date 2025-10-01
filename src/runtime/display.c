#include <stdlib.h>
#include <string.h>


#include "../utils/utils.h"
#include "runtime.h"


size_t get_var_name(name_t name, depth_t var) {

	size_t i = 0;

	do {
		name[i] = 'a' + var % NB_LETTERS;
		var /= NB_LETTERS;
		i++;
	} while (var >= NB_LETTERS);

	name[i] = '\0';

	return i;
}


size_t get_node_str(char** buffer, struct node_t* node) {

	if (node == NULL) {
		return 0;
	}

	size_t length;

	switch (node->type) {
		case Variable: {
			name_t var_name;

			length	= get_var_name(var_name, node->ref->depth) + 1;
			*buffer = malloc(sizeof(**buffer) * length);

			sprintf(*buffer, "%s", var_name);
			break;
		}

		case Function: {
			name_t var_name;
			char*  body_str;

			length	= get_var_name(var_name, node->depth) + get_node_str(&body_str, node->body) + 3;
			*buffer = malloc(sizeof(**buffer) * length);

			sprintf(*buffer, "%c%s%c%c%s", FUNC_DEF, var_name, DOT, SPACE, body_str);
			free(body_str);
			break;
		}

		case Application: {
			char* func_str;
			char* arg_str;

			length	= get_node_str(&func_str, node->func) + get_node_str(&arg_str, node->arg) + 4;
			*buffer = malloc(sizeof(**buffer) * length);

			sprintf(*buffer, "%c%s%c%c%s%c", APPLY_OPEN, func_str, COMMA, SPACE, arg_str, APPLY_CLOSE);
			free(func_str);
			free(arg_str);
			break;
		}

		case Macro:
			length	= strlen(node->token->name) + 1;
			*buffer = malloc(sizeof(**buffer) * length);
			sprintf(*buffer, "%s", node->token->name);
			break;


		case Directive: {
			char* next_str = "";

			length	= get_node_str(&next_str, node->next) + 2;
			*buffer = malloc(sizeof(**buffer) * length);

			sprintf(*buffer, "%c%s", get_dire_symbol(node->dire), next_str);
			if (length != 2) free(next_str);
			break;
		}

		case String:
			length	= strlen(node->str) + 3;
			*buffer = malloc(sizeof(**buffer) * length);
			sprintf(*buffer, "%c%s%c", STRING_OPEN, node->str, STRING_CLOSE);
			break;
	}

	return length;
}
