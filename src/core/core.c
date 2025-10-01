#include <bits/types/stack_t.h>
#include <stdbool.h>
#include <stdlib.h>


#include "../error/error.h"
#include "../utils/utils.h"
#include "core.h"


void update_node_ref(struct node_t* node, struct node_t* old_ref, struct node_t* new_ref) {

	if (node == NULL) {
		return;
	}

	switch (node->type) {
		default:	   break;
		case Function: update_node_ref(node->body, old_ref, new_ref); break;
		case Variable: node->ref = (node->ref == old_ref) ? new_ref : node->ref; break;
		case Application:
			update_node_ref(node->func, old_ref, new_ref);
			update_node_ref(node->arg, old_ref, new_ref);
			break;
	}

	return;
}


void update_node_parent(struct node_t* node, struct node_t* parent) {

	if (node == NULL) {
		return;
	}

	node->parent = parent;

	switch (node->type) {
		default:	   break;
		case Function: update_node_parent(node->body, node); break;
		case Application:
			update_node_parent(node->func, node);
			update_node_parent(node->arg, node);
			break;
	}

	return;
}


depth_t get_node_depth(struct node_t* node) {

	if (node == NULL) {
		return -1;
	}

	switch (node->type) {
		default:	   return get_node_depth(node->parent); break;
		case Function: return node->depth; break;
	}
}


void update_node_depth(struct node_t* node) {

	if (node == NULL) {
		return;
	}

	switch (node->type) {
		default: break;
		case Function:
			node->depth = get_node_depth(node->parent) + 1;
			update_node_depth(node->body);
			break;
		case Application:
			update_node_depth(node->func);
			update_node_depth(node->arg);
			break;
	}

	return;
}


bool is_beta_normal(struct node_t* node) {

	if (node == NULL) {
		return true;
	}

	switch (node->type) {
		default:		  return true; break;
		case Function:	  return is_beta_normal(node->body); break;
		case Application: return is_beta_normal(node->arg) && is_beta_normal(node->func) && !(node->func->type == Function || node->func->type == Macro || node->func->type == Directive); break;
	}
}


void replace_var(struct node_t** node, struct node_t* n_node, struct node_t* ref) {

	if (node == NULL) {
		return;
	}

	switch ((*node)->type) {
		default:	   break;
		case Function: replace_var(&(*node)->body, n_node, ref); break;
		case Variable:
			if ((*node)->ref == ref) {
				free_node(*node);
				*node = copy_node(n_node);
			}
			break;
		case Application:
			replace_var(&(*node)->func, n_node, ref);
			replace_var(&(*node)->arg, n_node, ref);
			break;
	}

	return;
}


struct node_t* apply_directive(struct node_t* node, struct node_t* directive_node, struct array_t* mac_array, struct array_t* str_array) {

	if (directive_node == NULL) return node;

	for (bool valid = true; valid;) {
		node = directive_node->dire(node, &valid, mac_array, str_array);
		node = apply_directive(node, directive_node->next, mac_array, str_array);
	}

	return node;
}


bool is_node_self_contained(struct node_t* node, struct array_t* var_array) {

	if (node == NULL) return false;

	switch (node->type) {
		default:		  return true; break;
		case Variable:	  return (get_token_by_ref(var_array, node->ref) == NULL) ? false : true; break;
		case Application: return is_node_self_contained(node->func, var_array) || is_node_self_contained(node->arg, var_array); break;
		case Function:	  {
			struct token_t* n_token = malloc(sizeof(*n_token));
			n_token->ref			= node;
			add_array_elem(var_array, n_token);
			return is_node_self_contained(node->body, var_array);
			break;
		}
	}
}


bool are_node_equal(struct node_t* node_1, struct node_t* node_2) {

	if (node_1 == NULL || node_2 == NULL) return true;
	if (XOR(node_1 == NULL, node_2 == NULL)) return false;

	if (node_1->type != node_2->type) return false;


	switch (node_1->type) {
		default:		  return true; break;
		case Variable:	  return node_1->ref->depth == node_2->ref->depth; break;
		case Function:	  return are_node_equal(node_1->body, node_2->body); break;
		case Application: return are_node_equal(node_1->func, node_2->func) && are_node_equal(node_1->arg, node_2->arg); break;
		case Macro:		  return node_1->token == node_2->token; break;
		case Directive:	  return get_dire_symbol(node_1->dire) == get_dire_symbol(node_2->dire) && are_node_equal(node_1->next, node_2->next); break;
	}
}


bool beta_reduce(struct node_t** node, struct array_t* mac_array, struct array_t* str_array) {

	if (node == NULL) return false;


	struct node_t* n_node;

	switch ((*node)->type) {
		default:	   return false; break;
		case Function: return beta_reduce(&(*node)->body, mac_array, str_array); break;
		case Macro:
			if ((*node)->token->ref == NULL) error_s(E_UDEF_MAC, (*node)->token->name);
			n_node = copy_node((*node)->token->ref);

			break;

		case Application:
			switch ((*node)->func->type) {
				default: return beta_reduce(&(*node)->func, mac_array, str_array) ? true : beta_reduce(&(*node)->arg, mac_array, str_array); break;
				case Function:
					n_node				= (*node)->func->body;
					(*node)->func->body = NULL;
					replace_var(&n_node, (*node)->arg, (*node)->func);
					break;
				case Directive:
					n_node		 = apply_directive((*node)->arg, (*node)->func, mac_array, str_array);
					(*node)->arg = NULL;
					break;
			}
			break;
	}

	free_node(*node);
	*node = n_node;
	return true;
}


bool mu_factorize(struct node_t** node, struct array_t* mac_array) {

	if (node == NULL) return false;

	struct array_t* var_array = init_array(DEFAULT_ARRAY_LENGTH, free);
	if (is_node_self_contained(*node, var_array)) {

		struct node_t* parent = (*node)->parent;
		update_node_parent(*node, NULL);
		update_node_depth(*node);

		ITERATE_ARRAY(mac_array, i) {
			struct token_t* token = get_array_elem(mac_array, i);
			if (are_node_equal(*node, token->ref)) {
				free_node(*node);
				free_array(var_array);
				*node = create_macro(NULL, token);
				return true;
			}
		}

		update_node_parent(*node, parent);
	}

	free_array(var_array);

	switch ((*node)->type) {
		default:		  return NULL;
		case Function:	  return mu_factorize(&(*node)->body, mac_array);
		case Application: {
			return mu_factorize(&(*node)->func, mac_array) ? true : mu_factorize(&(*node)->arg, mac_array);
		}
	}
}


bool mu_expand(struct node_t** node, struct array_t* mac_array) {

	if (node == NULL) return false;


	struct node_t* n_node;

	switch ((*node)->type) {
		default:		  return false; break;
		case Function:	  return mu_expand(&(*node)->body, mac_array); break;
		case Application: return mu_expand(&(*node)->func, mac_array) ? true : mu_expand(&(*node)->arg, mac_array); break;

		case Macro:
			if ((*node)->token->ref == NULL) error_s(E_UDEF_MAC, (*node)->token->name);

			n_node = copy_node((*node)->token->ref);
			free_node(*node);
			*node = n_node;

			return true;
			break;
	}
}
