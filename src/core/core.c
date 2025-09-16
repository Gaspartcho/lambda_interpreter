#include <stdlib.h>


#include "core.h"
#include "../utils/utils.h"
#include "../error/error.h"


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
		default:	   return true; break;
		case Function: return is_beta_normal(node->body); break;
		case Application:
			return is_beta_normal(node->arg) && is_beta_normal(node->func) &&
				   !(node->func->type == Function || node->func->type == Macro || node->func->type == Directive);
			break;
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


bool beta_reduce(struct node_t** node, struct array_t* mac_array) {

	if (node == NULL) {
		return false;
	}

	struct node_t* n_node;

	switch ((*node)->type) {
		default:	   return false; break;
		case Function: return beta_reduce(&(*node)->body, mac_array); break;
		case Macro:
			if ((*node)->token->ref == NULL) error_s(E_UDEF_MAC, (*node)->token->name);
			n_node = copy_node((*node)->token->ref);
			free_node(*node);
			*node = n_node;
			return true;
			break;

		case Application: {
			switch ((*node)->func->type) {
				default: return beta_reduce(&(*node)->func, mac_array) || beta_reduce(&(*node)->arg, mac_array); break;
				case Function:
					n_node = (*node)->func->body;
					(*node)->func->body = NULL;
					replace_var(&n_node, (*node)->arg, (*node)->func);
					break;
				case Directive:
					n_node		 = (*node)->func->dire((*node)->arg, mac_array);
					(*node)->arg = NULL;
					break;
			}

			free_node(*node);
			*node = n_node;
			return true;
			break;
		}
	}
}


bool is_node_self_contained(struct node_t* node, struct array_t* var_array) {
	if (node == NULL) {
		return true;
	}

	switch (node->type) {
		default:	   return true; break;
		case Variable: return (get_token_by_ref(var_array, node->ref) == NULL) ? false : true; break;
		case Application:
			return is_node_self_contained(node->func, var_array) || is_node_self_contained(node->arg, var_array);
			break;
		case Function: {
			struct token_t* n_token = malloc(sizeof(*n_token));
			n_token->ref			= node;
			add_array_elem(var_array, n_token);
			return is_node_self_contained(node->body, var_array);
			break;
		}
	}
}
