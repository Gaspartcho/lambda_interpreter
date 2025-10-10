#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "runtime/runtime.h"
#include "utils/array.h"
#include "utils/utils.h"


int main(int argc, char* argv[]) {

	bool interactive = false;
	bool help		 = false;

	if (argc < 2) help = true;

	struct array_t* mac_array = create_array(DEFAULT_ARRAY_LENGTH, free_mac_token);
	struct array_t* str_array = create_array(DEFAULT_ARRAY_LENGTH, free);
	struct array_t* file_list = create_array(argc, NULL);

	char* zero_file = malloc(sizeof(*zero_file));
	*zero_file		= '\0';
	add_array_elem(str_array, zero_file);


	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-i")) interactive = true;
		else if (!strcmp(argv[i], "-h")) help = true;
		else add_array_elem(file_list, argv[i]);
	}

	if (help) {
		printf("Lambda Interpreter (\\lprogi) (v.0.1)\n");
		printf("Licensed under the Apache License, Version 2.0\n");
		printf("This software is distributed on an \"AS IS\" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n\n");

		printf("Usage:\n");
		printf("lprogi [options...] [files...]\n\n");
		printf("Options\n");
		printf("-h : Prints this help and exits.\n");
		printf("-i : Runs in interactive mode after running the files.\n");

	} else {
		ITERATE_ARRAY(file_list, i) run_file(ARRAY_ELEM(file_list, i), mac_array, str_array);
		if (interactive) exec_file(stdin, true, mac_array, str_array);
	}

	free_array(mac_array);
	free_array(str_array);
	free_array(file_list);

	return EXIT_SUCCESS;
}
