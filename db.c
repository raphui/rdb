#include "db.h"

//struct entry **db;

static struct database *databases[MAX_DB_COUNT];

static int crc( int val )
{
	TRACE_2( DB , "crc( %d )." , val );

	int poly = 0xEDB88320;
	int crc = 0;
	unsigned int length = sizeof( int );
	unsigned int i = 0;
	unsigned char *p = ( unsigned char * )&val;

	while( length-- )
	{
		crc ^= *p++;

		for( i = 0 ; i < 8 ; i++ )
		{
			if( crc & 1 )
				crc = ( crc >> 1 ) ^ poly;
			else
				crc = ( crc >> 1 );
		}
	}

	return ~crc;
}

static int hash( int key , int value )
{

	TRACE_2( DB , "hash( %d , %d )." , key , value );

	int hash = 0;
	
	hash = crc( key );
	hash ^= crc( value );
	
	return hash;
}

static int getDatabase( const char *name )
{
	TRACE_2( DB , "getDatabase( %s ).\n" , name );

	int i = 0;

	for( i = 0 ; i < MAX_DB_COUNT ; i++ )
	{
		if( databases[i] != NULL )
			if( databases[i]->name != NULL )
				if( !strcmp( databases[i]->name , name ) )
					return i;
	}

	return -ENODATA;
}

int createDb( const char *name )
{
	TRACE_2( DB , "createDb( %s )." , name );

	int ret = 0;
	int i = 0;
	int index = 0;

	for( i = 0 ; i < MAX_DB_COUNT ; i++ )
	{
		if( !databases[i] )
		{
			index = i;
			break;
		}
	}

	TRACE_1( DB , "Find free index: %d in databases entry.\n" , index );

	databases[index] = ( struct database * )zmalloc( sizeof( struct database ) );

	if( !databases[index] )
	{
		TRACE_ERROR( DB , "Failed to allocate database entry.\n");
		ret = -ENOMEM;
	}
	else
	{
		TRACE_1( DB , "Database: %s has been added to the databases entry.\n" , name );

		databases[index]->name = ( char * )zmalloc( strlen( name ) + 1 * sizeof( char ) );
		snprintf( databases[index]->name , strlen( name ) + 1 , "%s" , name );

		databases[index]->db = ( struct entry ** )zmalloc( MAX_DB_SIZE * sizeof( struct entry ) );

		if( !databases[index]->db )
		{
			TRACE_ERROR( DB , "Failed to allocate database.\n");
			ret = -ENOMEM;
		}
		else
		{
			TRACE_1( DB , "Database has been allocate.\n");
	
			for( i = 0 ; i < MAX_DB_SIZE ; i++ )
			{
				databases[index]->db[i] = ( struct entry * )zmalloc( sizeof( struct entry ) );
				if( !databases[index]->db[i] )
					printf("Failed to allocate at index: %d\n" , i );
				else
					TRACE_3( DB , "%d: %p\n" , i , databases[index]->db[i] );
			}
		}

	}


	return ret;
}

int destroyDb( const char *name )
{
	TRACE_2( DB , "destroyDb( %s )." , name );
	
	int ret = 0;
	int i = 0;
	int index;

	index = getDatabase( name );

	if( !databases[index]->db )
	{
		TRACE_1( DB , "No database has been allocate.\n");
		ret = -ENXIO;
	}
	else
	{
		for( i = 0 ; i < MAX_DB_SIZE ; i++ )
			zfree( databases[index]->db[i] );

		zfree( databases[index]->db );
	}


	return ret;
}

int insertDb( const char *name , int key , int value )
{
	TRACE_2( DB , "insertDb( %d , %d )." , key , value );

	int ret = 0;
	int index = 0;
	struct entry **p = NULL;

	index = getDatabase( name );
	p = databases[index]->db;

	index = 0;

	while( p[index]->used != 0 )
	{
		index++;
	}

	TRACE_1( DB , "Find free index: p( %d ) , index( %d )." , p , index );

	p[index]->key = key;
	p[index]->value = value;
	p[index]->hash = hash( key , value );
	p[index]->used = 1;
	
	return ret;
}


int searchDb( const char *name , int key )
{
	TRACE_2( DB , "searchDb( %s , %d ).\n" , name , key );

	int ret = -ENODATA;
	int index = 0;
	int i = 0;
	struct entry **p = NULL;

	index = getDatabase( name );
	p = databases[index]->db;

	for( i = 0 ; i < MAX_DB_SIZE ; i++ )
	{
		if( p[i]->key == key )
		{
			/* Perform a hash, to check data integrity */
			if( p[i]->hash == hash( key , p[i]->value ) )
			{
				TRACE_1( DB , "Pair has been finded !\n");
				ret = i;
				break;
			}
		}
	}

	return ret;
}


void printDb( const char *name )
{
	TRACE_2( DB , "printDb().");

	struct entry **p = NULL;
	int index = 0;

	index = getDatabase( name );
	p = databases[index]->db;

	printf("|\tKey\t|\tValue\t|\tHash\t|\n");

	while( p[index]->hash != 0 )
	{
		printf("|\t0x%x\t" , p[index]->key );
		printf("|\t0x%x\t" , p[index]->value );
		printf("|\t0x%x\t" , p[index]->hash );
		printf("|\n");
		index++;
	}
	
}

void printFullDb( const char *name )
{
	TRACE_2( DB , "printFullDb().");

	struct entry **p = NULL;
	int index = 0;

	index = getDatabase( name );
	p = databases[index]->db;

	printf("|\tKey\t|\tValue\t|\tHash\t|\n");

	while( index < MAX_DB_SIZE )
	{
		TRACE_3( DB , "%d: %p , %p %p\n" , index , p , *p );

		printf("|\t0x%x\t" , p[index]->key );
		printf("|\t0x%x\t" , p[index]->value );
		printf("|\t0x%x\t" , p[index]->hash );
		printf("|\n");
		index++;
	}
	
}

static void sortAsc( struct entry **a , int n )
{
	TRACE_2( DB , "sortAsc( %p , %d )." , a , n );
	
	if( n < 2 )
		return;

	int p =  a[n / 2]->key;
	struct entry **l = a;
	struct entry **r = a + n - 1;
	struct entry *t;

	while( l <= r )
	{
		TRACE_3( DB , "%p , %p\n" , l , r );
		TRACE_3( DB , "%x , %x\n" , (*l)->key , (*r)->key );

		if( (*l)->key < p )
			*l++;
		else if( (*r)->key > p )
			*r--;
		else
		{

			t = *l;
			*l++ = *r;
			*r-- = t;
		}
	}


	sortAsc( a , r - a + 1 );
	sortAsc( l, a + n - l );
}

void sortAscDb( const char *name , int n )
{
	TRACE_2( DB , "sortAscDb( %s , %d ).\n" , name , n );

	int index;
	struct entry **p = NULL;

	index = getDatabase( name );
	p = databases[index]->db;

	sortAsc( p , n );

}

char *sort( void )
{
	TRACE_2( DB , "sort().\n");

	char *status = NULL;

	status = ( char * )zmalloc( 24 * sizeof( char ) );

	if( !status )
	{
		TRACE_ERROR( DB , "Failed to allocate status string.\n");
	}
	else
	{
		sortAscDb("testdb" , MAX_DB_SIZE );

		snprintf( status , 24 , "OK" );
	}

	return status;
}

char *print( void )
{
	TRACE_2( DB , "print().\n");

	char *status = NULL;
	struct entry **p = NULL;
	int index = 0;
	int nw = 0;

	index = getDatabase("testdb");
	p = databases[index]->db;

	status = ( char * )zmalloc( ( MAX_DB_SIZE * sizeof( int ) * 4 ) * sizeof( char ) );
	
	index = 0;

	if( !status )
	{
		TRACE_ERROR( DB , "Failed to allocate status string.\n");
	}
	else
	{
		while( index < MAX_DB_SIZE )
		{
			nw += sprintf( status + nw , "%d - %d - %x\n" , p[index]->key , p[index]->value , p[index]->hash );
			index++;
		}
	}
	
	return status;	
}

char *setPair( unsigned int key , unsigned int value )
{
	TRACE_2( DB , "setPair( %d , %d ).\n" , key , value );

	int ret = 0;
	char *status = NULL;

	ret = insertDb( "testdb" , key , value );

	status = ( char * )zmalloc( 124 * sizeof( char ) );

	if( !status )
	{
		TRACE_ERROR( DB , "Failed to allocate status string.\n");
	}
	else
	{
		if( ret < 0 )
			snprintf( status , 124 , "Cannot insert in database.\n");
		else
			snprintf( status , 124 , "OK");
	}


	return status;
}

/* value argument is not used, this argument is here just to use the cli (required 2 unsigned int args ) */
char *getPair( unsigned int key , unsigned int value )
{
	TRACE_2( DB , "getPair( %d , %d ).\n" , key , value );

	int ret = 0;
	char *status = NULL;
	int index = 0;
	struct entry **p = NULL;

	index = getDatabase( "testdb" );
	p = databases[index]->db;

	status = ( char * )zmalloc( 124 * sizeof( char ) );

	if( !status )
	{
		TRACE_ERROR( DB , "Failed to allocate status string.\n");
	}
	else
	{
		ret = searchDb( "testdb" , key );

		if( ret < 0 )
			snprintf( status , 124 , "Cannot retrieve pair in database.\n");
		else
			snprintf( status , 124 , "%d - %d - %x\n" , p[ret]->key , p[ret]->value , p[ret]->hash );
	}

	return status;
}
