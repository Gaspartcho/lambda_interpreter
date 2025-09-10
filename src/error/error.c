#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "error.h"


void error(char error_message[]) {

	fprintf(stderr, "ERROR: %s - %s\n", error_message, strerror(errno));
	exit(EXIT_FAILURE);

	return;
}
