#include <stdlib.h>
#include <string.h>


#include "core.h"


struct node_t* create_variable(struct node_t* parent, depth_t* var) {

	struct node_t* n_var = malloc(sizeof(*n_var));

	n_var->parent = (struct node_t*)parent;
	n_var->type	  = Variable;
	n_var->var	  = var;

	return n_var;
}


struct node_t* create_function(struct node_t* parent, struct node_t* body) {

	struct node_t* n_func = malloc(sizeof(*n_func));

	n_func->parent	   = parent;
	n_func->type	   = Function;
	n_func->func.body  = body;
	n_func->func.depth = 0;

	return n_func;
}


struct node_t* create_application(struct node_t* parent, struct node_t* func, struct node_t* arg) {

	struct node_t* n_apply = malloc(sizeof(*n_apply));

	n_apply->parent		= parent;
	n_apply->type		= Application;
	n_apply->apply.func = func;
	n_apply->apply.arg	= arg;

	return n_apply;
}


struct node_t* create_root(struct node_t* parent, struct node_t* child) {

	struct node_t* n_root = malloc(sizeof(*n_root));

	n_root->parent = parent;
	n_root->type   = Root;
	n_root->child  = child;

	return n_root;
}


struct node_t* copy_node(struct node_t* node) {

	if (node == NULL) {
		return NULL;
	}

	struct node_t* n_node;

	switch (node->type) {
		case Variable:	  n_node = create_variable(NULL, node->var); break;
		case Application: n_node = create_application(NULL, copy_node(node->apply.func), copy_node(node->apply.arg)); break;
		case Root:		  n_node = create_root(NULL, copy_node(node->child)); break;
		case Function:
			n_node = create_function(NULL, copy_node(node->func.body));
			update_node_var(n_node->func.body, &node->func.depth, &n_node->func.depth);
			break;
	}

	return n_node;
}


void free_node(struct node_t* node) {

	if (node == NULL) {
		return;
	}

	switch (node->type) {
		case Variable: break;
		case Function: free_node(node->func.body); break;
		case Root:	   free_node(node->child); break;
		case Application:
			free_node(node->apply.func);
			free_node(node->apply.arg);
			break;
	}

	free(node);

	return;
}
