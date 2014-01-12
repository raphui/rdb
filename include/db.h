#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <trace.h>

#define MAX_DB_COUNT	3
#define MAX_DB_SIZE		2048

typedef struct database
{
	int count;
	struct entry *head;
	struct entry *tail;

}Database;

typedef struct entry
{
	int key;
	int value;
	int hash;
	int used;

	struct entry *prev;
	struct entry *next;	

}Entry;

int createDb( void );
int destroyDb( void );
int insertDb( int key , int value );
//int searchDb( int key );
struct entry *searchDb( int key );
int removeDb( int key );

/* 0 : asc , 1 : desc */
//void sortDb( const char *name , int n , int direction );

char *sort( void );
char *print( void );
char *setPair( unsigned int key , unsigned int value );
char *getPair( unsigned int key , unsigned int value );
char *removePair( unsigned int key , unsigned int value );

#endif /* DB_H */
