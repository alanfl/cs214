#include <stdio.h>

void* dumbmalloc(int value, int line, char* filename)
{
	printf("Error on line %d in file %s\n.", line, filename);
	return NULL;
}
