#include <stddef.h>


#define TO_CHAR(str) str[0]



struct array_t {
	size_t size;
	size_t capacity;
	void* content[];
};


struct array_t* init_array(size_t size);
struct array_t* copy_array(struct array_t* array);
void add_array_elem(struct array_t* array, void* elem);
void set_array_elem(struct array_t* array, void* elem, size_t index);
void* get_array_elem(struct array_t* array, size_t index);
void* pop_array(struct array_t* array);
void free_array(struct array_t* array);
