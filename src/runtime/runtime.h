#pragma once


#include <stddef.h>
#include <stdio.h>


#include "../core/core.h"


#define SPACE		' '
#define FUNC_DEF	'\\'
#define DOT			'.'
#define COMMA		','
#define APPLY_OPEN	'('
#define APPLY_CLOSE ')'
#define BACKSPACE	'\b'
#define BLANK		'\0'
#define UNDERSCORE	'_'

#define DISPLAY		  '&'
#define TREE		  '#'
#define EVAL		  '$'
#define EVAL_ONCE	  '!'
#define EVAL_EACH	  '?'
#define MACRO		  '@'
#define FACTORIZE	  '-'
#define EXPAND		  '+'
#define INCLUDE		  '*'
#define COMMENT_OPEN  '%'
#define COMMENT_CLOSE '%'
#define STRING_OPEN	  '\"'
#define STRING_CLOSE  '\"'

#define NB_LETTERS 26


typedef char name_t[NAME_MAX + 1];


struct parser_t {
	FILE* file;
	char  next_char;
};


extern bool			  get_parser_next_char(struct parser_t* parser);
extern struct node_t* parse_next_node(struct parser_t* parser, struct array_t* var_array, struct array_t* mac_array, struct array_t* str_array);

void		 run_file(char filename[], struct array_t* mac_array, struct array_t* str_array);
extern char* get_node_str(struct node_t* node);
extern char* generate_tree(struct node_t* node);

extern struct node_t* d_identity(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* d_macro(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* d_factorize(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* d_expand(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* d_display(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* d_tree(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* d_evaluate(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* d_eval_once(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* d_eval_each(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* d_include(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array);
