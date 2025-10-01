#include <stdlib.h>


#include "../error/error.h"
#include "runtime.h"


void run_file(char filename[], struct array_t* mac_array, struct array_t* str_array) {

	char* last_filename = get_array_elem(str_array, 0);
	set_array_elem(str_array, filename, 0);

	struct parser_t parser	  = {.file = fopen(filename, "r"), .next_char = EMPTY};
	struct array_t* var_array = init_array(DEFAULT_ARRAY_LENGTH, free);

	if (parser.file == NULL) error_s(E_F, filename);

	get_parser_next_char(&parser);

	while (parser.next_char != EOF) {
		struct node_t* node = parse_next_node(&parser, var_array, mac_array, str_array);
		update_node_parent(node, NULL);

		// bool _unused;
		// d_display(node, &_unused, mac_array);

		while (beta_reduce(&node, mac_array, str_array)) {
			update_node_parent(node, NULL);
		}

		free_node(node);
	}

	free_array(var_array);
	fclose(parser.file);

	set_array_elem(str_array, last_filename, 0);

	return;
}
