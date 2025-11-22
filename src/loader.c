#include "loader.h"

#include "config.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int load_program(char* program)
{
	printf("Loading program...\n");
	FILE* fp = fopen("fibonacci.bc", "r");
	if (fp == NULL) {
		perror("Error opening file");
		return -1;
	}
	// Load into buffer
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	fread(program, 1, fsize, fp);
	program[fsize] = 0;

	printf("Program loaded\n");
	
	if (DEBUG) {
		printf("%s\n", program);
	}

	fclose(fp);

    return 0;
}
