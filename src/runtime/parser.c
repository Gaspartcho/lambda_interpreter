#include <linux/limits.h>
#include <stdio.h>
#include <stdtype.h>
#include <stdbool.h>


#include "runtime.h"


char get_parser_next_char(struct parser_t* parser) {

	parser->next_char = getc(parser->file);
	return parser->next_char;
}


struct node_t* parse_var(struct parser_t* parser) {
	char node_name[NAME_MAX] = "";

	if (parser->next_char < )

	node_name[0] = parser->next_char;

	for(int i = 1; i < NAME_MAX - 1; i ++) {

	}
}


struct node_t* parse_function(struct parser_t* parser) {

}
