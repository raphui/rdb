#include "db.h"

int *db = NULL;

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

	if( !db )
	{
		db = ( int * )zmalloc( MAX_DB_SIZE * sizeof( int ) );

		if( !db )
		{
			TRACE_ERROR( DB , "Failed to allocate database.\n");
			ret = -ENOMEM;
		}
		else
		{
			TRACE_1( DB , "Database has been allocate.\n");
			
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

	if( !db )
	{
		TRACE_1( DB , "No database has been allocate.\n");
		ret = -ENXIO;
	}
	else
	{
		zfree( db );
	}


	return ret;
}

int insert( int key , int value )
{
	TRACE_2( DB , "insert( %d , %d )." , key , value );

	int ret = 0;
	int index = 0;
	int *p = db;

	while( *p != 0 )
	{
		p++;
		index++;
	}

	TRACE_1( DB , "Find free index: p( %d ) , index( %d )." , p , index );

	p = db + index;

	*p = key;
	*++p = value;
	*++p = hash( key , value );
	
	return ret;
}


void printDb( void )
{
	TRACE_2( DB , "printDb().");

	int *p = db;
	int i = 0;

	printf("|\tKey\t|\tValue\t|\tHash\t\t");

	while( *p )
	{
		if( ( i % 3 ) == 0 )
			printf("|\n");
		
		printf("|\t0x%x\t" , *p );
		p++;
		i++;
	}
	
	printf("|\n");

}
