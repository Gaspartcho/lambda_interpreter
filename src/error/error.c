#include <stdio.h>
#include <stdlib.h>

#include "error.h"


void error(char error_message[]) {

	fprintf(stderr, "ERROR: %s.\n", error_message);
	exit(EXIT_FAILURE);

	return;
}


void error_s(char error_message[], char arg[]) {

	fprintf(stderr, "ERROR: %s - %s\n", error_message, arg);
	exit(EXIT_FAILURE);

	return;
}


void error_c(char error_message[], char arg) {

	fprintf(stderr, "ERROR: %s - %c\n", error_message, arg);
	exit(EXIT_FAILURE);

	return;
}
