#include <stdlib.h>


#include "core.h"


struct node_t* create_variable(struct node_t* parent, struct node_t* ref) {

	struct node_t* n_var = malloc(sizeof(*n_var));

	n_var->type	  = Variable;
	n_var->parent = parent;
	n_var->ref	  = ref;

	return n_var;
}

struct node_t* create_function(struct node_t* parent, struct node_t* body) {

	struct node_t* n_func = malloc(sizeof(*n_func));

	n_func->type   = Function;
	n_func->parent = parent;
	n_func->body   = body;
	n_func->depth  = 0;

	return n_func;
}

struct node_t* create_application(struct node_t* parent, struct node_t* func, struct node_t* arg) {

	struct node_t* n_apply = malloc(sizeof(*n_apply));

	n_apply->type	= Application;
	n_apply->parent = parent;
	n_apply->func	= func;
	n_apply->arg	= arg;

	return n_apply;
}

struct node_t* create_macro(struct node_t* parent, struct token_t* token) {

	struct node_t* n_macro = malloc(sizeof(*n_macro));

	n_macro->type	= Macro;
	n_macro->parent = parent;
	n_macro->token	= token;

	return n_macro;
}

struct node_t* create_directive(struct node_t* parent, directive_t dire, enum dire_type_t dire_type) {

	struct node_t* n_dire = malloc(sizeof(*n_dire));

	n_dire->type	  = Directive;
	n_dire->parent	  = parent;
	n_dire->dire	  = dire;
	n_dire->dire_type = dire_type;

	return n_dire;
}


struct node_t* reset_node(struct node_t* node) {

	struct node_t* n_node = copy_node(node);

	update_node_parent(n_node, node->parent);
	free_node(node);

	return n_node;
}


struct node_t* copy_node(struct node_t* node) {

	if (node == NULL) {
		return NULL;
	}

	switch (node->type) {
		case Variable:	  return create_variable(NULL, node->ref); break;
		case Application: return create_application(NULL, copy_node(node->func), copy_node(node->arg)); break;
		case Macro:		  return create_macro(NULL, node->token); break;
		case Directive:	  return create_directive(NULL, node->dire, node->dire_type); break;

		case Function: {

			struct node_t* n_node = create_function(NULL, copy_node(node->body));
			update_node_ref(n_node->body, node, n_node);
			return n_node;

			break;
		}
	}
	return NULL;
}


void free_node(struct node_t* node) {

	if (node == NULL) {
		return;
	}

	switch (node->type) {
		default:	   break;
		case Function: free_node(node->body); break;
		case Application:
			free_node(node->func);
			free_node(node->arg);
			break;
	}

	free(node);

	return;
}
