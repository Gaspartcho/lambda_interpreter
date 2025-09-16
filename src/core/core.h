#pragma once


#include <limits.h>
#include <stdbool.h>


#include "../utils/array.h"


#define MAX_DEPTH USHRT_MAX

typedef unsigned char  u_char;
typedef unsigned short u_short;
typedef unsigned char  depth_t;

typedef struct node_t* (*directive_t)(struct node_t*, struct array_t* mac_array);
typedef char name_t[NAME_MAX + 1];


enum type_t {
	Variable,
	Function,
	Application,
	Macro,
	Directive,
};

enum dire_type_t {
	d_Macro,
	d_Display,
	d_Tree,
	d_Display_E,
	d_Display_EO,
	d_Tree_E,
	d_Tree_EO,
};


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
		directive_t		dire;  // Directive
	};

	union {
		depth_t			 depth;		// Function
		struct node_t*	 arg;		// Application
		enum dire_type_t dire_type; // Directive
	};
};


extern void update_node_ref(struct node_t* node, struct node_t* old_ref, struct node_t* new_ref);
extern void update_node_parent(struct node_t* node, struct node_t* parent);
extern void update_node_depth(struct node_t* node);


extern struct node_t* create_variable(struct node_t* parent, struct node_t* ref);
extern struct node_t* create_function(struct node_t* parent, struct node_t* body);
extern struct node_t* create_application(struct node_t* parent, struct node_t* func, struct node_t* arg);
extern struct node_t* create_macro(struct node_t* parent, struct token_t* token);
extern struct node_t* create_directive(struct node_t* parent, directive_t dire, enum dire_type_t dire_type);

extern struct node_t* reset_node(struct node_t* node);
extern struct node_t* copy_node(struct node_t* node);
extern void			  free_node(struct node_t* node);

extern bool beta_reduce(struct node_t** node, struct array_t* mac_array);
extern bool is_node_self_contained(struct node_t* node, struct array_t* var_array);
