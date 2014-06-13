#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <trace.h>
#include <common.h>
#include <lzf.h>

#define MAX_DB_COUNT    3
#define MAX_DB_SIZE     2048

#define MAX_STRING_SIZE 512

typedef struct database
{
    int count;
    struct entry *head;
    struct entry *tail;

}Database;

typedef struct entry
{
    char *key;
    char *value;
    int hash;
    int used;

    struct entry *prev;
    struct entry *next;

    size_t key_size;
    size_t value_size;
    size_t size;

}Entry;

int createDb( void );
int destroyDb( void );
int insertDb( char *key , char *value );
struct entry *searchDb( char *key );
int removeDb( char *key );
int flushDb( void );
int compressDb( void );
int decompressDb( void );

char *sort( struct environment *env );
char *print( struct environment *env );
char *setPair( struct environment *env );
char *getPair( struct environment *env );
char *removePair( struct environment *env );
char *flush( struct environment *env );
char *compress( struct environment *env );
char *decompress( struct environment *env );

#endif /* DB_H */
