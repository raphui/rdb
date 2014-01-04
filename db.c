#include "db.h"

struct entry *db[];

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

int createDb( const char *name )
{
	TRACE_2( DB , "createDb( %s )." , name );

	int ret = 0;
	int i = 0;

	if( !db )
	{
		*db = ( struct entry * )zmalloc( MAX_DB_SIZE * sizeof( struct entry * ) );

		if( !db )
		{
			TRACE_ERROR( DB , "Failed to allocate database.\n");
			ret = -ENOMEM;
		}
		else
		{
			TRACE_1( DB , "Database has been allocate.\n");

			for( i == 0 ; i < MAX_DB_SIZE ; i++ )
				db[i] = ( struct entry * )zmalloc( sizeof( struct entry ) );
			
			memset( db , 0 , MAX_DB_SIZE );
		}
	}
	else
	{
		TRACE_1( DB , "A database is already allocate.\n");
	}


	return ret;
}

int destroyDb( const char *name )
{
	TRACE_2( DB , "destroyDb( %s )." , name );
	
	int ret = 0;
	int i = 0;

	if( !db )
	{
		TRACE_1( DB , "No database has been allocate.\n");
		ret = -ENXIO;
	}
	else
	{
		for( i = 0 ; i < MAX_DB_SIZE ; i++ )
			zfree( db[i] );
	
		zfree( db );
	}


	return ret;
}

int insert( int key , int value )
{
	TRACE_2( DB , "insert( %d , %d )." , key , value );

	int ret = 0;
	int index = 0;
	struct entry *p = db;

	while( p[index].hash != 0 )
	{
		p++;
		index++;
	}

	TRACE_1( DB , "Find free index: p( %d ) , index( %d )." , p , index );

	p = db + index;

	p[index].key = key;
	p[index].value = value;
	p[index].hash = hash( key , value );
	
	return ret;
}


void printDb( void )
{
	TRACE_2( DB , "printDb().");

	struct entry *p = db;
	int i = 0;
	int index = 0;

	printf("|\tKey\t|\tValue\t|\tHash\t|\n");

	while( p[index].hash != 0 )
	{
		printf("|\t0x%x\t" , p[index].key );
		printf("|\t0x%x\t" , p[index].value );
		printf("|\t0x%x\t" , p[index].hash );
		printf("|\n");
		index++;
		i++;
	}
	
}

void printFullDb( void )
{
	TRACE_2( DB , "printFullDb().");

	struct entry *p = db;
	int i = 0;
	int index = 0;

	printf("|\tKey\t|\tValue\t|\tHash\t|\n");

	while( index < MAX_DB_SIZE )
	{
		printf("|\t0x%x\t" , p[index].key );
		printf("|\t0x%x\t" , p[index].value );
		printf("|\t0x%x\t" , p[index].hash );
		printf("|\n");
		index++;
		i++;
	}
	
}

void sortAsc( struct entry **a , int n )
{
	TRACE_2( DB , "sortAsc( %p , %d )." , a , n );
	
	if( n < 2 )
		return;

	int p =  a[n / 2].key;
	struct entry *l = a;
	struct entry *r = a + n - 1;
	int t;

	while( l <= r )
	{
		if( l.key < p )
			l++;
		else if( r.key > p )
			r--;
		else
		{
			t = *l.key;
			*l++.key = *r.key;
			*r--.key = t;
		}
	}


	sortAsc( a , r - a + 1 );
	sortAsc( l, a + n - l );
}
