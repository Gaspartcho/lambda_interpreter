#include <stdlib.h>
#include <string.h>


#include "error/error.h"
#include "runtime/runtime.h"
#include "utils/array.h"
#include "utils/utils.h"

int main(int argc, char* argv[]) {

	if (argc < 2) error(E);

	struct array_t* mac_array = init_array(DEFAULT_ARRAY_LENGTH, free_mac_token);
	struct array_t* str_array = init_array(DEFAULT_ARRAY_LENGTH, free);

	char* zero_file = malloc(sizeof(*zero_file));
	*zero_file		= '\0';
	add_array_elem(str_array, zero_file);

	bool interactive = false;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-i")) interactive = true;
		else run_file(argv[i], mac_array, str_array);
	}

	if (interactive) exec_file(stdin, true, mac_array, str_array);

	free_array(mac_array);
	free_array(str_array);

	return EXIT_SUCCESS;
}
