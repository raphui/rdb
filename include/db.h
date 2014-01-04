#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <trace.h>

#define MAX_DB_SIZE	1024


int createDb( const char *name );
int destroyDb( const char *name );
int insert( int key , int value );
void printDb( void );

#endif /* DB_H */
