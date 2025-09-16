#include <stdio.h>
#include <stdlib.h>


#include "runtime.h"


void run_file(const char filename[], struct array_t* mac_array) {

	struct parser_t parser	  = {.file = fopen(filename, "r"), .next_char = '\0'};
	struct array_t* var_array = init_array(DEFAULT_ARRAY_LENGTH, free);

	get_parser_next_char(&parser);


	while (parser.next_char != EOF) {
		struct node_t* node = parse_next_node(&parser, var_array, mac_array);
		update_node_parent(node, NULL);
		free_node(evaluate(node, mac_array));
	}


	free_array(var_array);
	fclose(parser.file);

	return;
}
