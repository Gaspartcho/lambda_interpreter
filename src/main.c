#include <stdio.h>
#include <stdlib.h>


#include "core/core.h"
#include "runtime/runtime.h"

int main(int argc, char* agrv[]) {
	printf("Hello World!\n");

	struct node_t* one = create_root(NULL, NULL);
	struct node_t* two = copy_node(one);

	free_node(one);
	free_node(two);

	struct node_t* true_func = create_function(NULL, create_function(NULL, create_variable(NULL, NULL)));

	update_node_parent(true_func, NULL);
	update_node_depth(true_func);
	true_func->func.body->func.body->var = &true_func->func.depth;

	struct node_t* false_func = copy_node(true_func);

	update_node_parent(false_func, NULL);
	update_node_depth(false_func);
	false_func->func.body->func.body->var = &false_func->func.body->func.depth;

	struct node_t* apply = create_application(NULL, true_func, false_func);
	update_node_parent(apply, NULL);

	display_node(true_func);
	display_node(false_func);
	display_node(apply);

	free_node(apply);

	return EXIT_SUCCESS;
}
