#include <string.h>


#include "runtime.h"


void get_var_name(char buffer[], depth_t var) {

	char letter[2] = "a\0";

	do {
		letter[0] = 'a' + var % NB_LETTERS;
		strcat(buffer, letter);
		var /= NB_LETTERS;
	} while (var >= NB_LETTERS);

	return;
}


void get_node_str(char buffer[], struct node_t* node) {

	if (node == NULL) {
		return;
	}

	switch (node->type) {
		case Variable:
			get_var_name(buffer, *node->var);
			strcat(buffer, SPACE);
			break;

		case Function:
			strcat(buffer, FUNCTION_SYMBOL);
			get_var_name(buffer, node->func.depth);
			strcat(buffer, DOT);
			strcat(buffer, SPACE);
			get_node_str(buffer, node->func.body);
			break;

		case Application:
			strcat(buffer, OPEN_PARENTHESIS);
			get_node_str(buffer, node->apply.func);
			strcat(buffer, BACKSPACE);
			strcat(buffer, COMMA);
			strcat(buffer, SPACE);
			get_node_str(buffer, node->apply.arg);
			strcat(buffer, BACKSPACE);
			strcat(buffer, CLOSE_PARENTHESIS);
			strcat(buffer, SPACE);


			break;

		case Root: get_node_str(buffer, node->child); break;
	}

	return;
}


void display_node(struct node_t* node) {
	char name[NAME_MAX] = "";
	get_node_str(name, node);
	printf("%s\n", name);
}
