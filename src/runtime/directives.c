#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#include "../core/core.h"
#include "../error/error.h"
#include "../utils/utils.h"
#include "runtime.h"


struct node_t* d_identity(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {
	if (valid == NULL) return (struct node_t*)EMPTY;

	*valid = false;
	return node;
}


struct node_t* d_macro(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)MACRO;

	if (node->type != Macro) error_s(E_INV_TYPE, "Supposed to be Macro");
	if (node->parent->parent == NULL) error_s(E_INV_TYPE, "Function takes 2 arguments, gave 1");
	if (node->parent->parent->type != Application) error_s(E_INV_TYPE, "Supposed to be Application");
	if (node->token->ref != NULL) error_s(E_DEF_MAC, node->token->name);

	struct node_t*	n_ref	  = node->parent->parent->arg;
	struct array_t* var_array = init_array(DEFAULT_ARRAY_LENGTH, free);

	if (!is_node_self_contained(n_ref, var_array)) error_s(E_INV_TYPE, "Supposed to be self-contained");
	free_array(var_array);

	// God please forgive me for what is going to happen here...

	struct node_t* next_dire = node->parent->func;

	while (get_dire_symbol(next_dire->dire) != MACRO)
		next_dire = next_dire->next;

	node->token->ref = apply_directive(copy_node(n_ref), next_dire->next, mac_array, str_array);
	update_node_parent(node->token->ref, NULL);
	update_node_depth(node->token->ref);

	free_node(node);

	*valid = false;
	return create_directive(NULL, copy_node(next_dire->next), d_identity);
}


struct node_t* d_display(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)DISPLAY;

	update_node_depth(node);

	char* name;
	get_node_str(&name, node);
	printf("%s\n", name);
	free(name);

	*valid = false;
	return node;
}


struct node_t* d_tree(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)TREE;

	error_s(E_DIRE_NIMPL, "TREE (#)");

	*valid = false;
	return node;
}


struct node_t* d_evaluate(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)EVAL;

	struct node_t* parent = node->parent;

	while (beta_reduce(&node, mac_array, str_array))
		update_node_parent(node, parent);

	update_node_parent(node, parent);

	*valid = false;
	return node;
}

struct node_t* d_eval_once(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)EVAL_ONCE;

	struct node_t* parent = node->parent;
	beta_reduce(&node, mac_array, str_array);
	update_node_parent(node, parent);

	*valid = false;
	return node;
}


struct node_t* d_eval_each(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {
	if (valid == NULL) return (struct node_t*)EVAL_ONCE;

	struct node_t* parent = node->parent;
	beta_reduce(&node, mac_array, str_array);
	*valid = !is_beta_normal(node);
	update_node_parent(node, parent);

	return node;
}


struct node_t* d_factorize(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)FACTORIZE;

	struct node_t* parent = node->parent;

	while (mu_factorize(&node, mac_array))
		update_node_parent(node, parent);

	update_node_parent(node, parent);

	*valid = false;
	return node;
}


struct node_t* d_expand(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)EXPAND;

	struct node_t* parent = node->parent;

	while (mu_expand(&node, mac_array))
		update_node_parent(node, parent);

	update_node_parent(node, parent);

	*valid = false;
	return node;
}


struct node_t* d_include(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)INCLUDE;

	if (node->type != String) error_s(E_INV_TYPE, "Supposed to be string");

	char* filename;
	get_file_path_from_relative_path(&filename, get_array_elem(str_array, 0), node->str);

	run_file(filename, mac_array, str_array);
	free(filename);

	*valid = false;
	return node;
}
