#include <stdlib.h>


#include "error/error.h"
#include "runtime/runtime.h"
#include "utils/array.h"
#include "utils/utils.h"

int main(int argc, char* argv[]) {

	if (argc != 2) error(E);

	struct array_t* mac_array = init_array(DEFAULT_ARRAY_LENGTH, free_mac_token);
	run_file(argv[1], mac_array);
	free_array(mac_array);

	return EXIT_SUCCESS;
}
