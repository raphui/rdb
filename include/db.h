#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <trace.h>

#define MAX_DB_COUNT	3
#define MAX_DB_SIZE		512

typedef struct entry
{
	int key;
	int value;
	int hash;

}Entry;

typedef struct database
{
	char *name;
	struct entry **db;

}Database;

//extern struct entry **db;

int createDb( const char *name );
int destroyDb( const char *name );
int insertDb( const char *name , int key , int value );
int searchDb( const char *name , int key );
void printDb( const char *name );
void printFullDb( const char *name );
//void sortAsc( struct entry **a , int n );
void sortAscDb( const char *name , int n );

char *setPair( unsigned int key , unsigned int value );
char *getPair( unsigned int key , unsigned int value );

#endif /* DB_H */
