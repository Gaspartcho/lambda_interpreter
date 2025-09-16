#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../error/error.h"
#include "../utils/utils.h"
#include "runtime.h"


bool get_parser_next_char(struct parser_t* parser) {

	bool new_tok = false;

	parser->next_char = getc(parser->file);

	while (isspace(parser->next_char)) {
		parser->next_char = getc(parser->file);
		new_tok			  = true;
	}

	if (parser->next_char == COMMENT_OPEN) {
		for (parser->next_char = getc(parser->file); parser->next_char != COMMENT_CLOSE;
			 parser->next_char = getc(parser->file)) {
			if (parser->next_char == EOF) error(E_EOF);
		}

		get_parser_next_char(parser);
		new_tok = true;
	}

	return new_tok;
}


void parse_name(struct parser_t* parser, name_t name, bool (*pattern_func)(char)) {

	u_short i;
	bool	same_tok = true;

	for (i = 0; i < NAME_MAX && pattern_func(parser->next_char) && same_tok; i++) {
		name[i]	 = parser->next_char;
		same_tok = !get_parser_next_char(parser);
	}
	name[i] = EMPTY;

	return;
}


bool pattern_char_name(char c) {
	return islower(c) || c == '_';
}


bool pattern_macro_name(char c) {
	return isupper(c) || c == '_';
}

bool pattern_directive_name(char c) {
	return c == DISPLAY || c == TREE || c == EVAL || c == EVAL_ONCE || c == MACRO;
}


struct node_t* parse_var(struct parser_t* parser, struct array_t* var_array) {

	name_t name;
	parse_name(parser, name, pattern_char_name);
	if (strlen(name) == 0) error(E_EPT_TOK_NAME);

	struct node_t* ref = get_token_by_name(var_array, name)->ref;
	if (ref == NULL) error_s(E_UNK_TOK_NAME, name);

	return create_variable(NULL, ref);
}


struct node_t* parse_function(struct parser_t* parser, struct array_t* var_array, struct array_t* mac_array) {

	size_t		   init_array_size = var_array->size;
	struct node_t* r_func		   = create_function(NULL, NULL);
	struct node_t* b_func		   = r_func;

	if (parser->next_char != FUNC_DEF) error_c(E_INV_TOK, parser->next_char);

	get_parser_next_char(parser);


	do {
		struct token_t* token = malloc(sizeof(*token));
		token->ref			  = b_func;
		parse_name(parser, token->name, pattern_char_name);

		if (strlen(token->name) == 0) error(E_EPT_TOK_NAME);
		add_array_elem(var_array, token);

		b_func->body = create_function(b_func, NULL);
		b_func		 = b_func->body;

		if (parser->next_char == COMMA) get_parser_next_char(parser);
	} while (parser->next_char != DOT);

	get_parser_next_char(parser);

	b_func = b_func->parent;
	free_node(b_func->body);
	b_func->body = parse_next_node(parser, var_array, mac_array);

	while (var_array->size > init_array_size)
		free(pop_array_elem(var_array));

	return r_func;
}


struct node_t* parse_application(struct parser_t* parser, struct array_t* var_array, struct array_t* mac_array) {

	if (parser->next_char != APPLY_OPEN) error_c(E_INV_TOK, parser->next_char);

	get_parser_next_char(parser);
	struct node_t* n_apply = create_application(NULL, parse_next_node(parser, var_array, mac_array), NULL);

	while (parser->next_char != APPLY_CLOSE) {
		if (parser->next_char == COMMA) get_parser_next_char(parser);
		n_apply->arg = parse_next_node(parser, var_array, mac_array);
		n_apply		 = create_application(NULL, n_apply, NULL);
	}

	struct node_t* r_apply = n_apply->func;
	n_apply->func		   = NULL;
	free_node(n_apply);

	get_parser_next_char(parser);

	return r_apply;
}


struct node_t* parse_macro(struct parser_t* parser, struct array_t* mac_array) {
	name_t name;
	parse_name(parser, name, pattern_macro_name);
	if (strlen(name) == 0) error(E_EPT_TOK_NAME);

	struct token_t* token = get_token_by_name(mac_array, name);

	if (token == NULL) {
		token = malloc(sizeof(*token));
		token->ref = NULL;
		strcpy(token->name, name);
		add_array_elem(mac_array, token);
	}

	return create_macro(NULL, token);
}


struct node_t* parse_directive(struct parser_t* parser) {

	name_t syms;
	parse_name(parser, syms, pattern_directive_name);

	if (!(strlen(syms) == 1 || strlen(syms) == 2)) error_s(E_INV_TOK_NAME, syms);

	switch (syms[0]) {
		default:
			error_c(E_INV_TOK, syms[0]);
			return NULL;
			break;

		case MACRO: return create_directive(NULL, macro, d_Macro); break;

		case DISPLAY:
			switch (syms[1]) {
				default:
					error_c(E_INV_TOK, syms[1]);
					return NULL;
					break;
				case EMPTY:		return create_directive(NULL, display, d_Display); break;
				case EVAL:		return create_directive(NULL, display_eval, d_Display_E); break;
				case EVAL_ONCE: return create_directive(NULL, display_eval_once, d_Display_EO); break;
			}
			break;

		case TREE:
			switch (syms[1]) {
				default:
					error_c(E_INV_TOK, syms[1]);
					return NULL;
					break;
				case EMPTY:		return create_directive(NULL, tree, d_Tree); break;
				case EVAL:		return create_directive(NULL, tree_eval, d_Tree_E); break;
				case EVAL_ONCE: return create_directive(NULL, tree_eval_once, d_Tree_EO); break;
			}
			break;
	}
}


struct node_t* parse_next_node(struct parser_t* parser, struct array_t* var_array, struct array_t* mac_array) {

	if (pattern_char_name(parser->next_char)) return parse_var(parser, var_array);
	if (parser->next_char == FUNC_DEF) return parse_function(parser, var_array, mac_array);
	if (parser->next_char == APPLY_OPEN) return parse_application(parser, var_array, mac_array);
	if (pattern_macro_name(parser->next_char)) return parse_macro(parser, mac_array);
	if (pattern_directive_name(parser->next_char)) return parse_directive(parser);

	error_c(E_INV_TOK, parser->next_char);

	return NULL;
}
