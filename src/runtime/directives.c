#include <stdio.h>
#include <stdlib.h>


#include "../error/error.h"
#include "runtime.h"


struct node_t* macro(struct node_t* node, struct array_t* mac_array) {

	if (node->type != Macro) error_s(E_INV_TYPE, "Supposed to be Macro");
	if (node->parent->parent == NULL) error_s(E_INV_TYPE, "Function takes 2 arguments, gave 1");
	if (node->parent->parent->type != Application) error_s(E_INV_TYPE, "Supposed to be Application");

	struct array_t* var_array = init_array(DEFAULT_ARRAY_LENGTH, free);
	if (!is_node_self_contained(node->parent->parent->arg, var_array))
		error_s(E_INV_TYPE, "Supposed to be self-contained");
	free_array(var_array);

	if (node->token->ref != NULL) error_s(E_DEF_MAC, node->token->name);

	node->token->ref = copy_node(node->parent->parent->arg);
	free_node(node);

	struct node_t* identity = create_function(NULL, create_variable(NULL, NULL));
	identity->body->ref		= identity;

	return identity;
}

struct node_t* evaluate(struct node_t* node, struct array_t* mac_array) {

	struct node_t* parent = node->parent;

	while (beta_reduce(&node, mac_array))
		update_node_parent(node, parent);

	update_node_parent(node, parent);

	return node;
}

struct node_t* eval_once(struct node_t* node, struct array_t* mac_array) {

	struct node_t* parent = node->parent;
	beta_reduce(&node, mac_array);
	update_node_parent(node, parent);

	return node;
}


struct node_t* display(struct node_t* node, struct array_t* mac_array) {

	update_node_depth(node);

	char* name;
	get_node_str(&name, node);
	printf("%s\n", name);
	free(name);

	return node;
}

struct node_t* tree(struct node_t* node, struct array_t* mac_array) {
	error_s(E_DIRE_NIMPL, "TREE (#)");
	return node;
}


struct node_t* display_eval(struct node_t* node, struct array_t* mac_array) {
	return display(evaluate(node, mac_array), mac_array);
}

struct node_t* display_eval_once(struct node_t* node, struct array_t* mac_array) {
	return display(eval_once(node, mac_array), mac_array);
}

struct node_t* tree_eval(struct node_t* node, struct array_t* mac_array) {
	return tree(evaluate(node, mac_array), mac_array);
}

struct node_t* tree_eval_once(struct node_t* node, struct array_t* mac_array) {
	return tree(eval_once(node, mac_array), mac_array);
}
