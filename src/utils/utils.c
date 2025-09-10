#include <stddef.h>
#include <stdlib.h>


#include "utils.h"


// TODO: add error detection in this place

struct array_t* init_array(size_t size) {

	struct array_t* n_array = malloc(sizeof(*n_array) + (sizeof(void*) * (size - 1)));
	n_array->size			= 0;
	n_array->capacity		= size;

	return n_array;
}

struct array_t* copy_array(struct array_t* array) {

	struct array_t* n_array = init_array(array->capacity);
	n_array->size			= array->size;

	for (int i = 0; i < array->size; i++) {
		n_array->content[i] = array->content[i];
	}

	return n_array;
}

void add_array_elem(struct array_t* array, void* elem) {

	if (array->size == array->capacity) {
		struct array_t* n_array = init_array(array->capacity * 2);
		n_array->size			= array->size;

		for (int i = 0; i < array->size; i++) {
			n_array->content[i] = array->content[i];
		}

		free_array(array);
		array = n_array;
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

void* pop_array(struct array_t* array) {

	if (array->size == 0) {
		free_array(array);
		return NULL;
	}

	if (array->size <= array->capacity / 2) {
		struct array_t* n_array = init_array(array->capacity / 2);
		n_array->size			= array->size;

		for (int i = 0; i < array->size; i++) {
			n_array->content[i] = array->content[i];
		}

		free(array);
		array = n_array;
	}

	array->size -= 1;
	return array->content[array->size];
}

void free_array(struct array_t* array) {
	free(array);
	return;
}
