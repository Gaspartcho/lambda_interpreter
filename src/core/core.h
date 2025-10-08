#pragma once


#include <stdbool.h>


#include "../utils/array.h"


#define NAME_MAX 255


enum type_t : char {
	Variable,
	Function,
	Application,
	Macro,
	Directive,
	String,
};

enum status_t : char {
	Idle,
	LoopBegin,
	LoopEnd,
	LoopBreak,
	LoopContinue,
};


typedef unsigned char  u_char;
typedef unsigned short u_short;
typedef unsigned char  depth_t;

typedef struct node_t* (*directive_t)(struct node_t*, enum status_t*, struct array_t*, struct array_t*);
typedef char name_t[NAME_MAX + 1];


struct token_t {
	name_t		   name;
	struct node_t* ref;
};

struct node_t {

	struct node_t* parent;
	enum type_t	   type;

	union {
		struct node_t*	ref;   // Variable
		struct node_t*	body;  // Function
		struct node_t*	func;  // Application
		struct token_t* token; // Macro
		struct node_t*	next;  // Directive
		char*			str;   // String
	};

	union {
		depth_t		   depth; // Function
		struct node_t* arg;	  // Application
		directive_t	   dire;  // Directive
	};
};


extern struct node_t* create_variable(struct node_t* parent, struct node_t* ref);
extern struct node_t* create_function(struct node_t* parent, struct node_t* body);
extern struct node_t* create_application(struct node_t* parent, struct node_t* func, struct node_t* arg);
extern struct node_t* create_macro(struct node_t* parent, struct token_t* token);
extern struct node_t* create_directive(struct node_t* parent, struct node_t* next, directive_t dire);
extern struct node_t* create_string(struct node_t* parent, char* data);

extern struct node_t* copy_node(struct node_t* node);
extern void			  free_node(struct node_t* node);

extern struct node_t* update_node_ref(struct node_t* node, struct node_t* old_ref, struct node_t* new_ref);
extern struct node_t* update_node_parent(struct node_t* node, struct node_t* parent);
extern struct node_t* update_node_depth(struct node_t* node);

extern struct node_t* apply_directive(struct node_t* node, struct node_t* directive_node, enum status_t* status, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* beta_reduce(struct node_t* node, bool* changed, struct array_t* mac_array, struct array_t* str_array);
extern struct node_t* mu_factorize(struct node_t* node, bool* changed, struct array_t* mac_array);
extern struct node_t* mu_expand(struct node_t* node, bool* changed, struct array_t* mac_array);
extern bool			  is_node_self_contained(struct node_t* node, struct array_t* var_array);
extern bool			  is_beta_normal(struct node_t* node);
extern bool			  are_node_equal(struct node_t* node_1, struct node_t* node_2);
