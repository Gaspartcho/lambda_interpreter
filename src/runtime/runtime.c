#include <stdbool.h>
#include <stdlib.h>


#include "../error/error.h"
#include "runtime.h"


void run_file(char filename[], struct array_t* mac_array, struct array_t* str_array) {

	char* last_filename		 = ARRAY_ELEM(str_array, 0);
	ARRAY_ELEM(str_array, 0) = filename;

	struct parser_t parser	  = {.file = fopen(filename, "r"), .next_char = BLANK};
	struct array_t* var_array = init_array(DEFAULT_ARRAY_LENGTH, free);

	if (parser.file == NULL) error_s(E_F, filename);

	get_parser_next_char(&parser);

	while (parser.next_char != EOF) {
		struct node_t* node = update_node_parent(parse_next_node(&parser, var_array, mac_array, str_array), NULL);

		// bool _unused;
		// d_display(node, &_unused, mac_array);

		for (bool changed = true; changed;) {
			changed = false;
			node	= update_node_parent(beta_reduce(node, &changed, mac_array, str_array), NULL);
		}

		free_node(node);
	}

	free_array(var_array);
	fclose(parser.file);

	ARRAY_ELEM(str_array, 0) = last_filename;

	return;
}
