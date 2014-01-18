#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <errno.h>

#define MAX_GENERIC_VAL	4

typedef struct environment
{
	int memoryAllocate;
	int memoryFree;
	int countThreads;
	
	struct database *db;

	/* Generic value, used by some functions */
	char *genericVal[MAX_GENERIC_VAL];
	unsigned int arg_count;

}Environment;

struct environment *env;

/* struct environment has to be declared before inclusion, because zmemory and trace need it */
#include <zmemory.h>
#include <trace.h>

int initEnvironment( void );

#endif /* COMMON_H */
