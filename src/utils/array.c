#include <stdlib.h>


#include "array.h"


// TODO: add error detection in this place

struct array_t* init_array(size_t size, void (*free_function)(void*)) {

	struct array_t* n_array = malloc(sizeof(*n_array));
	n_array->content		= malloc(sizeof(*n_array->content) * size);
	n_array->size			= 0;
	n_array->capacity		= size;
	n_array->free_function	= (free_function == NULL) ? free : free_function;


	return n_array;
}

struct array_t* copy_array(struct array_t* array) {

	struct array_t* n_array = init_array(array->capacity, array->free_function);
	n_array->size			= array->size;

	for (size_t i = 0; i < array->size; i++) {
		n_array->content[i] = array->content[i];
	}

	return n_array;
}

void add_array_elem(struct array_t* array, void* elem) {

	if (array->size == array->capacity) {
		void** n_contents = malloc(sizeof(*n_contents) * (array->capacity * 2));

		for (int i = 0; i < array->size; i++) {
			n_contents[i] = array->content[i];
		}

		free(array->content);
		array->content = n_contents;
		array->capacity *= 2;
	}

	array->content[array->size] = elem;
	array->size += 1;

	return;
}

void set_array_elem(struct array_t* array, void* elem, size_t index) {

	array->content[index] = elem;
	return;
}

void* get_array_elem(struct array_t* array, size_t index) {
	return array->content[index];
}

void* pop_array_elem(struct array_t* array) {

	if (array->size == 0) {
		free_array(array);
		return NULL;
	}

	if (array->size <= array->capacity / 2 && array->size > 1) {

		void** n_contents = malloc(sizeof(*n_contents) * (array->capacity / 2));

		for (int i = 0; i < array->size; i++) {
			n_contents[i] = array->content[i];
		}

		free(array->content);
		array->content = n_contents;
		array->capacity /= 2;
	}

	array->size--;

	void* ret_val = array->content[array->size];

	return ret_val;
}

void free_array(struct array_t* array) {

	for (size_t i = 0; i < array->size; i++) {
		array->free_function(array->content[i]);
	}

	free(array->content);

	free(array);

	return;
}
