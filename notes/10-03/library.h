#ifndef _LIBRARY_H
#define _LIBRARY_H

#define malloc(x) dumbmalloc(x, __LINE__, __FILE__)

void* dumbmalloc(int, int, char*);

#endif
