#include <stdio.h>
#include <stdlib.h>


#include "../error/error.h"
#include "runtime.h"


struct node_t* display(struct node_t* node, struct array_t* mac_array) {
	// node = reset_node(node);

	char* name;

	get_node_str(&name, node);
	printf("%s\n", name);
	free(name);

	return node;
}

struct node_t* tree(struct node_t* node, struct array_t* mac_array) {
	// node = reset_node(node);

	error_s(E_DIRE_NIMPL, "TREE (#)");
	return node;
}

struct node_t* evaluate(struct node_t* node, struct array_t* mac_array) {
	struct node_t* parent = node->parent;
	node				  = reset_node(node);

	while (beta_reduce(&node, mac_array)) {
		update_node_parent(node, parent);
		update_node_depth(node);
	}

	update_node_parent(node, parent);
	update_node_depth(node);
	return node;
}

struct node_t* eval_once(struct node_t* node, struct array_t* mac_array) {
	struct node_t* parent = node->parent;
	node				  = reset_node(node);

	beta_reduce(&node, mac_array);

	update_node_parent(node, parent);
	update_node_depth(node);
	return node;
}

struct node_t* macro(struct node_t* node, struct array_t* mac_array) {

	if (node->type != Macro) error_s(E_INV_TYPE, "Supposed to be Macro");
	if (node->parent->parent == NULL) error_s(E_INV_TYPE, "Function takes 2 arguments, gave 1");
	if (node->parent->parent->type != Application) error_s(E_INV_TYPE, "Supposed to be Application");

	struct array_t* var_array = init_array(4, free);
	if (!is_node_self_contained(node->parent->parent->arg, var_array))
		error_s(E_INV_TYPE, "Supposed to be self-contained");
	free_array(var_array);

	node->token->ref = copy_node(node->parent->parent->arg);
	free_node(node);

	struct node_t* identity = create_function(NULL, create_variable(NULL, NULL));
	identity->body->ref		= identity;

	return identity;
}
