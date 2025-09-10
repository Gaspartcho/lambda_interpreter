#pragma once


#include <stdio.h>


#include "../core/core.h"


#define SPACE		" "
#define FUNC_DEF	"\\"
#define FUNC_DOT	"."
#define APPLY_DOT	","
#define APPLY_OPEN	"("
#define APPLY_CLOSE ")"

#define BACKSPACE "\b"

#define DISPLAY		   "&"
#define TREE		   "#"
#define EVAL		   "$"
#define DEF_MACRO	   "%"
#define DEF_CMACRO	   "@"
#define COMMENT		   "//"
#define OPEN_BCOMMENT  "/*"
#define CLOSE_BCOMMENT "*/"
#define OPEN_FRAME	   "{"
#define CLOSE_FRAME	   "}"
#define OPEN_MACRO	   "["
#define CLOSE_MACRO	   "]"
#define OPEN_STRING	   "\""
#define CLOSE_STRING   "\""


#define NB_LETTERS		   26
#define DEFAULT_FRAME_SIZE 8


enum e_type_t {
	Node,
	String,
	Display,
	Tree,
	Evaluate,
	Macro,
	Macro_Def,
	CMacro_Def
};


struct parser_t {
	FILE* file;
	char  next_char;
};


struct r_elem_t {
	enum e_type_t type;
	char		  name[NAME_MAX];

	union {
		struct r_elem_t* elem;
		struct node_t*	 node;
	};
};


extern char get_parser_next_char(struct parser_t* parser);

extern struct r_elem_t* parse_next_elem(struct parser_t* parser);


extern void get_node_str(char buffer[], struct node_t* node);
extern void display_node(struct node_t* node);


// @TEST \x. \y.x

// 	%../

// 	% D_E_D [
// 	&#$& #
// ]
