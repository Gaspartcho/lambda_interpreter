#pragma once


#include <limits.h>


#define MAX_DEPTH USHRT_MAX


typedef unsigned short depth_t;


enum n_type_t {
	Variable,
	Function,
	Application,
	Root
};

struct func_t {
	struct node_t* body;
	depth_t		   depth;
};

struct apply_t {
	struct node_t* func;
	struct node_t* arg;
};

struct node_t {

	struct node_t* parent;
	enum n_type_t  type;

	union {
		depth_t*	   var;
		struct func_t  func;
		struct apply_t apply;
		struct node_t* child;
	};
};


extern void update_node_var(struct node_t* node, depth_t* old_var, depth_t* new_var);
extern void update_node_parent(struct node_t* node, struct node_t* parent);
extern void update_node_depth(struct node_t* node);

extern struct node_t* create_variable(struct node_t* parent, depth_t* var);
extern struct node_t* create_function(struct node_t* parent, struct node_t* body);
extern struct node_t* create_application(struct node_t* parent, struct node_t* func, struct node_t* arg);
extern struct node_t* create_root(struct node_t* parent, struct node_t* child);

extern struct node_t* copy_node(struct node_t* node);
extern void			  free_node(struct node_t* node);
