#pragma once


#include <stddef.h>


#define DEFAULT_ARRAY_LENGTH 6 // Insanely arbitrary value


#define ITERATE_ARRAY(arr, index) for (int index = 0; index < arr->size; index++)
#define ARRAY_ELEM(arr, index)	  arr->contents[index]


struct array_t {
	size_t size;
	size_t capacity;
	void (*free_function)(void*);
	void** contents;
};


extern struct array_t* init_array(size_t size, void (*free_function)(void*));
extern struct array_t* copy_array(struct array_t* array);
extern void			   add_array_elem(struct array_t* array, void* elem);
extern void*		   pop_array_elem(struct array_t* array);
extern void			   free_array(struct array_t* array);
