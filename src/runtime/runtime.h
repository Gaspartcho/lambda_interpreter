#pragma once


#include <stdio.h>


#include "../core/core.h"


#define SPACE		' '
#define FUNC_DEF	'\\'
#define DOT			'.'
#define COMMA		','
#define APPLY_OPEN	'('
#define APPLY_CLOSE ')'
#define BACKSPACE	'\b'

#define DISPLAY		  '&'
#define TREE		  '#'
#define EVAL		  '$'
#define EVAL_ONCE	  '!'
#define MACRO		  '@'
#define COMMENT_OPEN  '%'
#define COMMENT_CLOSE '%'
#define OPEN_STRING	  '\"'
#define CLOSE_STRING  '\"'


#define NB_LETTERS		   26
#define DEFAULT_FRAME_SIZE 8


typedef char name_t[NAME_MAX + 1];


struct parser_t {
	FILE* file;
	char  next_char;
};


extern char			  get_parser_next_char(struct parser_t* parser);
extern struct node_t* parse_next_node(struct parser_t* parser, struct array_t* var_array, struct array_t* mac_array);

extern void	  run_file(const char filename[], struct array_t* mac_array);
extern size_t get_node_str(char** buffer, struct node_t* node);

extern struct node_t* display(struct node_t* node, struct array_t* mac_array);
extern struct node_t* tree(struct node_t* node, struct array_t* mac_array);
extern struct node_t* evaluate(struct node_t* node, struct array_t* mac_array);
extern struct node_t* eval_once(struct node_t* node, struct array_t* mac_array);
extern struct node_t* macro(struct node_t* node, struct array_t* mac_array);
