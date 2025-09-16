#pragma once


#include <stddef.h>


#define DEFAULT_ARRAY_LENGTH 6 // Insanely arbitrary value


struct array_t {
	size_t size;
	size_t capacity;
	void (*free_function)(void*);
	void** content;
};


extern struct array_t* init_array(size_t size, void (*free_function)(void*));
extern struct array_t* copy_array(struct array_t* array);
extern void			   add_array_elem(struct array_t* array, void* elem);
extern void			   set_array_elem(struct array_t* array, void* elem, size_t index);
extern void*		   get_array_elem(struct array_t* array, size_t index);
extern void*		   pop_array_elem(struct array_t* array);
extern void			   free_array(struct array_t* array);
