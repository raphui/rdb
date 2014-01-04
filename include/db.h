#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <trace.h>

#define MAX_DB_SIZE	1024

extern int *db;

int createDb( const char *name );
int destroyDb( const char *name );
int insert( int key , int value );
void printDb( void );
void printFullDb( void );
void sortAsc( int *a , int n );

#endif /* DB_H */
