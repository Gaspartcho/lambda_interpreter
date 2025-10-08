#include <stdlib.h>
#include <string.h>


#include "../utils/utils.h"
#include "runtime.h"


#define IMAGE_ELEM(image, row, col) (image)->data[((row) * (image)->width) + (col)]

#define VAR_FLAG	N_BIT(8)
#define FUNC_FLAG	N_BIT(9)
#define FUNC_E_FLAG N_BIT(10)
#define APPL_B_FLAG N_BIT(11)
#define APPL_E_FLAG N_BIT(12)
#define HBAR_FLAG	N_BIT(13)
#define VBAR_FLAG	N_BIT(14)
#define VAL_MASK	(N_BIT(8) - 1)

#define TREE_FUNC_T		"┯"
#define TREE_FUNC_CROSS "┿"
#define TREE_FUNC_HBAR	"━"
#define TREE_VBAR		"│"
#define TREE_HBAR		"─"
#define TREE_CURVE		"┘"
#define TREE_SPLIT		"├"
#define TREE_SPACE		" "


struct tree_image_t {
	u_short width;
	u_short height;

	u_short* data;
};


void init_image(struct tree_image_t* image, u_short width, u_short height) {

	size_t data_size = sizeof(*image->data) * height * width;

	image->width  = width;
	image->height = height;
	image->data	  = malloc(data_size);
	memset(image->data, 0, data_size);

	return;
}


void copy_image(struct tree_image_t* dest, struct tree_image_t* src) {

	dest->width	 = src->width;
	dest->height = src->height;
	dest->data	 = src->data;

	return;
}


void paste_image(struct tree_image_t* canvas, struct tree_image_t* image, u_short row, u_short col) {

	for (u_short i = 0; i < image->height; i++) {
		for (u_short j = 0; j < image->width; j++) {
			if ((i + row < canvas->height) && (j + col < canvas->width)) IMAGE_ELEM(canvas, i + row, j + col) = IMAGE_ELEM(image, i, j);
		}
	}

	return;
}


void resize_image(struct tree_image_t* image, u_short width, u_short height) {

	struct tree_image_t n_image;
	init_image(&n_image, width, height);
	paste_image(&n_image, image, 0, 0);

	free(image->data);
	copy_image(image, &n_image);

	return;
}


bool build_tree(struct tree_image_t* image, struct node_t* node) {

	if (node == NULL) return false;

	switch (node->type) {

		case Variable: IMAGE_ELEM(image, 0, 0) = VAR_FLAG | node->ref->depth; return false;

		case Function: {
			struct tree_image_t body_image;
			init_image(&body_image, 1, 1);
			build_tree(&body_image, node->body);

			bool shift = IMAGE_ELEM(&body_image, 0, 0) & FUNC_FLAG;
			bool extend = IMAGE_ELEM(&body_image, 0, body_image.width-1) & FUNC_FLAG;

			resize_image(image, body_image.width + (shift ? 0 : 1) + (extend ? 0 : 1), body_image.height + 1);
			paste_image(image, &body_image, 1, (shift ? 0 : 1));
			free(body_image.data);

			for (u_short j = 0; j < image->width; j++)
				IMAGE_ELEM(image, 0, j) = FUNC_FLAG | node->depth;

			return true;
		}

		case Application: {
			struct tree_image_t func_image;
			struct tree_image_t arg_image;
			init_image(&func_image, 1, 1);
			init_image(&arg_image, 1, 1);
			bool shift_f = build_tree(&func_image, node->func);
			bool shift_a = build_tree(&arg_image, node->arg);

			bool squash = (((IMAGE_ELEM(&func_image, func_image.height - 1, shift_f) & VAR_FLAG) && (node->func->type != Application)) || (func_image.height < arg_image.height)) &&
						  (((IMAGE_ELEM(&arg_image, arg_image.height - 1, shift_a) & VAR_FLAG) && (node->arg->type != Application)) || (func_image.height > arg_image.height));

			resize_image(image, func_image.width + arg_image.width + 1, MAX(func_image.height, arg_image.height) + (squash ? 0 : 1));
			paste_image(image, &func_image, 0, 0);
			paste_image(image, &arg_image, 0, func_image.width + 1);
			free(func_image.data);
			free(arg_image.data);

			IMAGE_ELEM(image, image->height - 1, shift_f) |= APPL_B_FLAG;
			IMAGE_ELEM(image, image->height - 1, func_image.width + 1 + shift_a) |= APPL_E_FLAG;

			for (u_short i = 1 + shift_f; i < func_image.width + 1 + shift_a; i++) {
				IMAGE_ELEM(image, image->height - 1, i) |= HBAR_FLAG;
			}

			return shift_f;
		}

		case Macro: {
			char* name = node->token->name;
			resize_image(image, strlen(name), 1);

			for (u_short i = 0; name[i] != '\0'; i++)
				IMAGE_ELEM(image, 0, i) = name[i];

			return false;
		}

		case String: {
			char* name = node->str;
			resize_image(image, strlen(name) + 2, 1);
			IMAGE_ELEM(image, 0, 0)				   = STRING_OPEN;
			IMAGE_ELEM(image, 0, image->width - 1) = STRING_CLOSE;

			for (u_short i = 0; name[i] != '\0'; i++)
				IMAGE_ELEM(image, 0, i + 1) = name[i];

			return false;
		}

		case Directive: {
			struct tree_image_t next_image;
			init_image(&next_image, 1, 1);
			build_tree(&next_image, node->next);

			resize_image(image, next_image.width + 1, 1);
			paste_image(image, &next_image, 0, 1);
			free(next_image.data);

			IMAGE_ELEM(image, 0, 0) = get_dire_symbol(node->dire);

			return false;
		}
	}

	return false;
}


void complete_tree(struct tree_image_t* image) {

	for (u_short i = 0; i < image->height; i++) {
		for (u_short j = 0; j < image->width; j++) {

			if (IMAGE_ELEM(image, i, j) & VAR_FLAG) {
				u_short elem = IMAGE_ELEM(image, i, j) & VAL_MASK;
				for (int k = i; k >= 0; k--) {
					if (elem == (IMAGE_ELEM(image, k, j) & (~FUNC_FLAG))) {
						IMAGE_ELEM(image, k, j) |= FUNC_E_FLAG;
						break;
					}
					IMAGE_ELEM(image, k, j) |= VBAR_FLAG;
				}
			}

			if (IMAGE_ELEM(image, i, j) & (APPL_B_FLAG | APPL_E_FLAG)) {
				for (int k = i - 1; k >= 0; k--) {
					if (IMAGE_ELEM(image, k, j) && !(IMAGE_ELEM(image, k, j) & VAR_FLAG)) break;
					IMAGE_ELEM(image, k, j) |= VBAR_FLAG;
				}
			}
		}
	}

	return;
}


size_t get_image_size(struct tree_image_t* image) {
	size_t count = 0;

	for (u_short i = 0; i < image->height; i++) {
		for (u_short j = 0; j < image->width; j++) {
			count += ((IMAGE_ELEM(image, i, j) & (~VAL_MASK))) ? 2 : 0;
		}
	}

	return (image->width + 1) * image->height + count + 1;
}


void draw_tree(char* buffer, struct tree_image_t* image) {
	for (u_short i = 0; i < image->height; i++) {
		for (u_short j = 0; j < image->width; j++) {

			char  default_character[2] = {IMAGE_ELEM(image, i, j) & VAL_MASK, BLANK};
			char* next_character	   = default_character;

			if ((IMAGE_ELEM(image, i, j) & FUNC_E_FLAG)) next_character = TREE_FUNC_T;
			else if ((IMAGE_ELEM(image, i, j) & FUNC_FLAG) && (IMAGE_ELEM(image, i, j) & VBAR_FLAG)) next_character = TREE_FUNC_CROSS;
			else if (IMAGE_ELEM(image, i, j) & FUNC_FLAG) next_character = TREE_FUNC_HBAR;
			else if (IMAGE_ELEM(image, i, j) & APPL_B_FLAG) next_character = TREE_SPLIT;
			else if (IMAGE_ELEM(image, i, j) & APPL_E_FLAG) next_character = TREE_CURVE;
			else if (IMAGE_ELEM(image, i, j) & VBAR_FLAG) next_character = TREE_VBAR;
			else if (IMAGE_ELEM(image, i, j) & HBAR_FLAG) next_character = TREE_HBAR;
			else if ((IMAGE_ELEM(image, i, j) & VAL_MASK) == BLANK) next_character = TREE_SPACE;

			buffer += sprintf(buffer, "%s", next_character);
		}
		buffer += sprintf(buffer, "\n");
	}
}


char* generate_tree(struct node_t* node) {

	struct tree_image_t image;
	init_image(&image, 1, 1);
	build_tree(&image, node);
	complete_tree(&image);

	char* buffer = malloc(sizeof(*buffer) * get_image_size(&image));
	draw_tree(buffer, &image);
	free(image.data);

	return buffer;
}
