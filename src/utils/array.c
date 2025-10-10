#include <stdlib.h>


#include "array.h"
#include "utils.h"


// TODO: add error detection in this place


struct array_t* create_array(size_t size, void (*free_function)(void*)) {

	struct array_t* n_array = malloc(sizeof(*n_array));
	n_array->contents	   = malloc(sizeof(*n_array->contents) * size);
	n_array->size		   = 0;
	n_array->capacity	   = size;
	n_array->free_function = (free_function == NULL) ? free_none : free_function;


	return n_array;
}


struct array_t* copy_array(struct array_t* array) {

	struct array_t* n_array						 = create_array(array->capacity, array->free_function);
	n_array->size								 = array->size;
	ITERATE_ARRAY(array, i) n_array->contents[i] = array->contents[i];

	return n_array;
}

void add_array_elem(struct array_t* array, void* elem) {

	if (array->size == array->capacity) {
		void** n_contents					  = malloc(sizeof(*n_contents) * (array->capacity * 2));
		ITERATE_ARRAY(array, i) n_contents[i] = array->contents[i];

		free(array->contents);
		array->contents = n_contents;
		array->capacity *= 2;
	}

	array->contents[array->size] = elem;
	array->size += 1;

	return;
}

void* pop_array_elem(struct array_t* array) {

	if (array->size == 0) {

		return NULL;
	}

	if (array->size <= array->capacity / 2 && array->size > 1) {

		void** n_contents					  = malloc(sizeof(*n_contents) * (array->capacity / 2));
		ITERATE_ARRAY(array, i) n_contents[i] = array->contents[i];

		free(array->contents);
		array->contents = n_contents;
		array->capacity /= 2;
	}

	array->size--;

	void* ret_val = array->contents[array->size];

	return ret_val;
}

void free_array(struct array_t* array) {

	ITERATE_ARRAY(array, i) array->free_function(array->contents[i]);
	free(array->contents);
	free(array);

	return;
}
