#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>


#include "../core/core.h"
#include "../error/error.h"
#include "../utils/utils.h"
#include "runtime.h"


struct node_t* d_identity(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {
	if (valid == NULL) return (struct node_t*)BLANK;

	*valid = false;
	return node;
}


struct node_t* d_macro(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)MACRO;

	if (node->type != Macro) error_s(E_INV_TYPE_M, get_node_str(node));
	if (node->parent->parent == NULL) error_s(E_NB_ARG, get_node_str(node->parent));
	if (node->parent->parent->type != Application) error_s(E_INV_TYPE_A, get_node_str(node->parent->parent));
	if (node->token->ref != NULL) error_s(E_DEF_MAC, node->token->name);

	struct node_t*	n_ref	  = node->parent->parent->arg;
	struct array_t* var_array = init_array(DEFAULT_ARRAY_LENGTH, free);
	if (!is_node_self_contained(n_ref, var_array)) error_s(E_SELFC, node->token->name);
	free_array(var_array);


	struct node_t* dire_node = node->parent->func;

	while (get_dire_symbol(dire_node->dire) != MACRO)
		dire_node = dire_node->next;

	n_ref					  = apply_directive(n_ref, dire_node->next, mac_array, str_array);
	node->parent->parent->arg = n_ref;
	node->token->ref		  = update_node_depth(update_node_parent(copy_node(n_ref), NULL));

	free_node(node);

	*valid = false;
	return create_directive(NULL, NULL, d_identity);
}


struct node_t* d_display(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)DISPLAY;

	update_node_depth(node);

	char* name = get_node_str(node);
	printf("%s\n", name);
	free(name);

	*valid = false;
	return node;
}


struct node_t* d_tree(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)TREE;

	struct array_t* var_array = init_array(DEFAULT_ARRAY_LENGTH, free);
	if (!is_node_self_contained(node, var_array)) error_s(E_INV_TYPE, "Supposed to be self-contained");
	free_array(var_array);

	char* tree = generate_tree(update_node_depth(node));
	printf("%s\n", tree);
	free(tree);

	*valid = false;
	return node;
}


struct node_t* d_evaluate(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)EVAL;

	struct node_t* parent = node->parent;

	for (bool changed = true; changed;) {
		changed = false;
		node	= update_node_parent(beta_reduce(node, &changed, mac_array, str_array), parent);
	}


	*valid = false;
	return node;
}

struct node_t* d_eval_once(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)EVAL_ONCE;

	struct node_t* parent  = node->parent;
	bool		   changed = false;
	node				   = update_node_parent(beta_reduce(node, &changed, mac_array, str_array), parent);

	*valid = false;
	return node;
}


struct node_t* d_eval_each(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {
	if (valid == NULL) return (struct node_t*)EVAL_EACH;

	struct node_t* parent  = node->parent;
	bool		   changed = false;
	node				   = update_node_parent(beta_reduce(node, &changed, mac_array, str_array), parent);
	*valid				   = !is_beta_normal(node);

	return node;
}


struct node_t* d_factorize(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)FACTORIZE;

	struct node_t* parent = node->parent;

	for (bool changed = true; changed;) {
		changed = false;
		node	= update_node_parent(mu_factorize(node, &changed, mac_array), parent);
	}


	*valid = false;
	return node;
}


struct node_t* d_expand(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)EXPAND;

	struct node_t* parent = node->parent;

	for (bool changed = true; changed;) {
		changed = false;
		node	= update_node_parent(mu_expand(node, &changed, mac_array), parent);
	}

	*valid = false;
	return node;
}


struct node_t* d_include(struct node_t* node, bool* valid, struct array_t* mac_array, struct array_t* str_array) {

	if (valid == NULL) return (struct node_t*)INCLUDE;

	if (node->type != String) error_s(E_INV_TYPE_S, get_node_str(node));


	char* filename = get_file_path_from_relative_path(ARRAY_ELEM(str_array, 0), node->str);
	run_file(filename, mac_array, str_array);
	free(filename);

	*valid = false;
	return node;
}
