


#include "runtime.h"
#include "../utils/utils.h"


void expand_elem_cmacro(struct r_elem_t* elem, struct r_elem_t* macro);
void expand_elem_macro(struct r_elem_t* elem, struct r_elem_t* macro);

struct r_elem_t* eval_elem(struct r_elem_t* elem);


void parse_file(FILE* file, struct array_t* frame) {
	struct parser_t parser = {.file = file, .next_char = '\0'};

	get_parser_next_char(&parser);

	while (parser.next_char != EOF) {
		add_array_elem(frame, parse_next_elem(&parser));
	}
}



void register_args(struct array_t* frame, int argc, char* argv[]);

void run_file(FILE* file, int argc, char* argv[]) {

	struct array_t* main_frame = init_array(DEFAULT_FRAME_SIZE);

	parse_file(file, main_frame);

	for (size_t i = 0; i < main_frame->size; i++) {
		struct r_elem_t* elem = get_array_elem(main_frame, i);

		if (elem->type == Macro_Def)
			for (size_t j = 0; j < main_frame->size; j++) {
				expand_elem_macro(get_array_elem(main_frame, j), elem);
			}

		if (elem->type == CMacro_Def)
			for (size_t j = 0; j < main_frame->size; j++) {
				expand_elem_cmacro(get_array_elem(main_frame, j), elem);
			}
	}

	for (size_t i = 0; i < main_frame->size; i++) {
		eval_elem(get_array_elem(main_frame, i));
	}

	free_array(main_frame);
}
