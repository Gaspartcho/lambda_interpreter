#include <stdbool.h>
#include <stdlib.h>


#include "../error/error.h"
#include "../utils/utils.h"
#include "core.h"


struct node_t* update_node_ref(struct node_t* node, struct node_t* old_ref, struct node_t* new_ref) {

	if (node == NULL) return NULL;

	switch (node->type) {
		default:	   break;
		case Function: update_node_ref(node->body, old_ref, new_ref); break;
		case Variable:
			if (node->ref == old_ref) node->ref = new_ref;
			break;
		case Application:
			update_node_ref(node->func, old_ref, new_ref);
			update_node_ref(node->arg, old_ref, new_ref);
			break;
	}

	return node;
}


struct node_t* update_node_parent(struct node_t* node, struct node_t* parent) {

	if (node == NULL) return NULL;

	if(node->parent != parent) node->parent = parent;

	switch (node->type) {
		default:	   break;
		case Function: update_node_parent(node->body, node); break;
		case Application:
			update_node_parent(node->func, node);
			update_node_parent(node->arg, node);
			break;
	}

	return node;
}


depth_t get_node_depth(struct node_t* node) {

	if (node == NULL) return -1;

	switch (node->type) {
		default:	   return get_node_depth(node->parent); break;
		case Function: return node->depth; break;
	}
}


struct node_t* update_node_depth(struct node_t* node) {

	if (node == NULL) return NULL;

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

	return node;
}


bool is_beta_normal(struct node_t* node) {

	if (node == NULL) return true;

	switch (node->type) {
		default:		  return true; break;
		case Macro:		  return false; break;
		case Function:	  return is_beta_normal(node->body); break;
		case Application: return is_beta_normal(node->arg) && is_beta_normal(node->func) && !(node->func->type == Function || node->func->type == Directive || node->func->type == Loop); break;
	}
}


struct node_t* replace_var(struct node_t* node, struct node_t* n_node, struct node_t* ref) {

	if (node == NULL) return NULL;

	switch (node->type) {
		default:	   break;
		case Function: node->body = replace_var(node->body, n_node, ref); break;
		case Variable:
			if (node->ref == ref) {
				free_node(node);
				node = copy_node(n_node);
			}
			break;
		case Application:
			node->func = replace_var(node->func, n_node, ref);
			node->arg  = replace_var(node->arg, n_node, ref);
			break;
	}

	return node;
}


struct node_t* apply_directive(struct node_t* node, struct node_t* directive_node, bool* loop, struct array_t* mac_array, struct array_t* str_array) {

	if (directive_node == NULL) return node;

	switch (directive_node->type) {
		default:		error_c(E_INV_TYPE, '1' + directive_node->type); break;
		case Directive: {
			bool do_loop = false;
			node		 = directive_node->dire(node, &do_loop, mac_array, str_array);
			*loop |= do_loop;
			break;
		}
		case Loop:
			for (bool do_loop = true; do_loop;) {
				do_loop = false;
				node	= apply_directive(node, directive_node->start, &do_loop, mac_array, str_array);
			}
			break;
	}

	node = apply_directive(node, directive_node->next, loop, mac_array, str_array);

	return node;
}


bool is_node_self_contained(struct node_t* node, struct array_t* var_array) {

	if (node == NULL) return false;

	switch (node->type) {
		default:		  return true; break;
		case Variable:	  return get_token_by_ref(var_array, node->ref) != NULL; break;
		case Application: return is_node_self_contained(node->func, var_array) && is_node_self_contained(node->arg, var_array); break;
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

	if (node_1 == NULL && node_2 == NULL) return true;
	if (XOR(node_1 == NULL, node_2 == NULL)) return false;

	if (node_1->type != node_2->type) return false;


	switch (node_1->type) {
		default:		  return true; break;
		case Variable:	  return node_1->ref->depth == node_2->ref->depth; break;
		case Function:	  return are_node_equal(node_1->body, node_2->body); break;
		case Application: return are_node_equal(node_1->func, node_2->func) && are_node_equal(node_1->arg, node_2->arg); break;
		case Macro:		  return node_1->token == node_2->token; break;
		case Directive:	  return get_dire_symbol(node_1->dire) == get_dire_symbol(node_2->dire) && are_node_equal(node_1->next, node_2->next); break;
		case Loop:		  return are_node_equal(node_1->start, node_2->start) && are_node_equal(node_1->start, node_2->start);
		case String:	  return node_1->str == node_2->str;
	}
}


struct node_t* beta_reduce(struct node_t* node, bool* changed, struct array_t* mac_array, struct array_t* str_array) {

	if (node == NULL) return NULL;

	switch (node->type) {
		default:	   break;
		case Function: node->body = beta_reduce(node->body, changed, mac_array, str_array); break;
		case Macro:	   {
			struct token_t* token = node->token;
			if (token->ref == NULL) error_s(E_UDEF_MAC, token->name);
			free_node(node);
			node	 = copy_node(token->ref);
			*changed = true;
			break;
		}

		case Application:
			switch (node->func->type) {

				default:
					node->func = beta_reduce(node->func, changed, mac_array, str_array);
					if (!(*changed)) node->arg = beta_reduce(node->arg, changed, mac_array, str_array);
					break;
				case Function: {
					struct node_t* n_node = replace_var(node->func->body, node->arg, node->func);
					node->func->body	  = NULL;
					free_node(node);
					node	 = n_node;
					*changed = true;
					break;
				}
				case Directive:
				case Loop:		{
					bool		   loop	  = false;
					struct node_t* n_node = apply_directive(node->arg, node->func, &loop, mac_array, str_array);
					node->arg			  = NULL;
					free_node(node);
					node	 = n_node;
					*changed = true;
					break;
				}
			}
			break;
	}

	return node;
}


struct node_t* mu_factorize(struct node_t* node, bool* changed, struct array_t* mac_array) {

	if (node == NULL) return NULL;

	struct array_t* var_array = create_array(DEFAULT_ARRAY_LENGTH, free);
	if (is_node_self_contained(node, var_array)) {

		update_node_depth(update_node_parent(node, NULL));

		ITERATE_ARRAY(mac_array, i) {
			struct token_t* token = ARRAY_ELEM(mac_array, i);
			if (are_node_equal(node, token->ref)) {
				free_node(node);
				free_array(var_array);
				*changed = true;
				return create_macro(NULL, token);
			}
		}
	}

	free_array(var_array);

	switch (node->type) {
		default:	   break;
		case Function: node->body = mu_factorize(node->body, changed, mac_array); break;
		case Application:
			node->func = mu_factorize(node->func, changed, mac_array);
			if (!(*changed)) node->arg = mu_factorize(node->arg, changed, mac_array);
			break;
	}

	return node;
}


struct node_t* mu_expand(struct node_t* node, bool* changed, struct array_t* mac_array) {

	if (node == NULL) return NULL;

	switch (node->type) {
		default:	   break;
		case Function: node->body = mu_expand(node->body, changed, mac_array); break;
		case Application:
			node->func = mu_expand(node->func, changed, mac_array);
			if (!(*changed)) node->arg = mu_expand(node->arg, changed, mac_array);
			break;

		case Macro: {
			struct token_t* token = node->token;
			if (token->ref == NULL) error_s(E_UDEF_MAC, token->name);

			free_node(node);
			node	 = copy_node(token->ref);
			*changed = true;
			break;
		}
	}

	return node;
}
