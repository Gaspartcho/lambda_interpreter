#include "stdlib.h"


#include "core.h"


void update_node_var(struct node_t* node, depth_t* old_var, depth_t* new_var) {

	if (node == NULL) {
		return;
	}

	switch (node->type) {
		case Function: update_node_var(node->func.body, old_var, new_var); break;
		case Root:	   update_node_var(node->child, old_var, new_var); break;
		case Variable:
			if (node->var == old_var) node->var = new_var;
			break;
		case Application:
			update_node_var(node->apply.func, old_var, new_var);
			update_node_var(node->apply.arg, old_var, new_var);
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
		case Variable: break;
		case Function: update_node_parent(node->func.body, node); break;
		case Root:	   update_node_parent(node->child, node); break;
		case Application:
			update_node_parent(node->apply.func, node);
			update_node_parent(node->apply.arg, node);
			break;
	}

	return;
}


depth_t get_node_depth(struct node_t* node) {

	if (node == NULL) {
		return -1;
	}

	switch (node->type) {
		case Function: return node->func.depth; break;
		default:	   return get_node_depth(node->parent); break;
	}
}


void update_node_depth(struct node_t* node) {

	if (node == NULL) {
		return;
	}

	switch (node->type) {
		case Variable: break;
		case Root:	   update_node_parent(node->child, node); break;
		case Function:
			node->func.depth = get_node_depth(node->parent) + 1;
			update_node_depth(node->func.body);
			break;
		case Application:
			update_node_depth(node->apply.func);
			update_node_depth(node->apply.arg);
			break;
	}
}
