#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#include "../core/core.h"
#include "../error/error.h"
#include "../utils/utils.h"
#include "runtime.h"


struct node_t* d_identity(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {
	if (loop == NULL) return (struct node_t*)IDENTITY;

	return node;
}


struct node_t* d_ask(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {
	if (loop == NULL) return (struct node_t*)ASK;

	while (getc(stdin) != '\n')
		continue;

	return node;
}


struct node_t* d_macro(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {

	if (loop == NULL) return (struct node_t*)MACRO;

	if (node->type != Macro) error_s(E_INV_TYPE_M, get_node_str(node));
	if (node->parent->parent == NULL) error_s(E_NB_ARG, get_node_str(node->parent));
	if (node->parent->parent->type != Application) error_s(E_INV_TYPE_A, get_node_str(node->parent->parent));
	if (node->token->ref != NULL) error_s(E_DEF_MAC, node->token->name);

	struct node_t*	n_ref	  = node->parent->parent->arg;
	struct array_t* var_array = create_array(DEFAULT_ARRAY_LENGTH, free);
	if (!is_node_self_contained(n_ref, var_array)) error_s(E_SELFC, node->token->name);
	free_array(var_array);

	struct node_t* dire_node = node->parent->func;

	while (get_dire_symbol(dire_node->dire) != MACRO) {
		dire_node = dire_node->next;
		if (dire_node == NULL) error_s(E, get_node_str(node->parent->func));
	}

	n_ref					  = apply_directive(n_ref, dire_node->next, loop, mac_array, str_array);
	node->parent->parent->arg = n_ref;
	node->token->ref		  = update_node_depth(update_node_parent(copy_node(n_ref), NULL));

	free_node(dire_node->next);
	free_node(node);
	dire_node->next = NULL;

	return CREATE_NULL_NODE;
}


struct node_t* d_display(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {

	if (loop == NULL) return (struct node_t*)DISPLAY;

	char* name = get_node_str(update_node_depth(node));
	printf("%s\n", name);
	free(name);

	return node;
}


struct node_t* d_tree(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {

	if (loop == NULL) return (struct node_t*)TREE;

	struct array_t* var_array = create_array(DEFAULT_ARRAY_LENGTH, free);
	if (!is_node_self_contained(node, var_array)) error_s(E_INV_TYPE, "Supposed to be self-contained");
	free_array(var_array);

	char* tree = generate_tree(update_node_depth(node));
	printf("%s\n", tree);
	free(tree);

	return node;
}


struct node_t* d_evaluate(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {

	if (loop == NULL) return (struct node_t*)EVAL;

	struct node_t* parent = node->parent;
	node				  = update_node_parent(beta_reduce(node, loop, mac_array, str_array), parent);

	return node;
}


struct node_t* d_factorize(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {

	if (loop == NULL) return (struct node_t*)FACTORIZE;

	struct node_t* parent = node->parent;
	node				  = update_node_parent(mu_factorize(node, loop, mac_array), parent);

	return node;
}


struct node_t* d_expand(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {

	if (loop == NULL) return (struct node_t*)EXPAND;

	struct node_t* parent = node->parent;
	node				  = update_node_parent(mu_expand(node, loop, mac_array), parent);

	return node;
}


struct node_t* d_include(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {

	if (loop == NULL) return (struct node_t*)INCLUDE;

	if (node->type != String) error_s(E_INV_TYPE_S, get_node_str(node));

	char* filename = get_file_path_from_relative_path(ARRAY_ELEM(str_array, 0), node->str);
	run_file(filename, mac_array, str_array);
	free(filename);

	return node;
}


struct node_t* d_discard(struct node_t* node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {

	if (loop == NULL) return (struct node_t*)DISCARD;

	struct node_t* identity = create_directive(node->parent, NULL, d_identity);
	free_node(node);

	return identity;
}
