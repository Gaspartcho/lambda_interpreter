#include <stdbool.h>
#include <stdlib.h>


#include "../error/error.h"
#include "runtime.h"


void run_file(char filename[], struct array_t* mac_array, struct array_t* str_array) {

	char* last_filename		 = ARRAY_ELEM(str_array, 0);
	ARRAY_ELEM(str_array, 0) = filename;

	FILE* file = fopen(filename, "r");
	if (file == NULL) error_s(E_F, filename);

	exec_file(file, false, mac_array, str_array);

	fclose(file);
	ARRAY_ELEM(str_array, 0) = last_filename;

	return;
}


void exec_file(FILE* file, bool interactive, struct array_t* mac_array, struct array_t* str_array) {

	struct parser_t parser	  = {.file = file, .next_char = BLANK};
	struct array_t* var_array = init_array(DEFAULT_ARRAY_LENGTH, free);

	if (interactive) printf("> ");

	get_parser_next_char(&parser);

	struct node_t* null_node = CREATE_NULL_NODE;

	while (parser.next_char != EOF) {
		struct node_t* node = update_node_parent(parse_next_node(&parser, var_array, mac_array, str_array), NULL);

		for (bool changed = true; changed;) {
			changed = false;
			node	= update_node_parent(beta_reduce(node, &changed, mac_array, str_array), NULL);
		}

		if (interactive && !are_node_equal(node, null_node)) {

			char* name = get_node_str(update_node_depth(node));
			printf("%s\n", name);
			free(name);
			printf("> ");
		}

		free_node(node);
	}

	free_node(null_node);
	free_array(var_array);

	return;
}
