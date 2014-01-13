#include "db.h"

static struct database *db;

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

//static int getDatabase( const char *name )
//{
//	TRACE_2( DB , "getDatabase( %s ).\n" , name );
//
//	int i = 0;
//
//	for( i = 0 ; i < MAX_DB_COUNT ; i++ )
//	{
//		if( databases[i] != NULL )
//			if( databases[i]->name != NULL )
//				if( !strcmp( databases[i]->name , name ) )
//					return i;
//	}
//
//	return -ENODATA;
//}

int createDb( void )
{
	TRACE_2( DB , "createDb().");

	int ret = 0;

	db = ( struct database * )zmalloc( sizeof( struct database ) );

	if( !db )
	{
		TRACE_ERROR( DB , "Failed to allocate database.\n");
		ret = -ENOMEM;
	}
	else
	{

		TRACE_1( DB , "Initialise database.\n");

		db->count = 0;
		db->head = NULL;
		db->tail = NULL;

	}


	return ret;
}

int destroyDb( void )
{
	TRACE_2( DB , "destroyDb().");
	
	int ret = 0;

	if( !db )
	{
		TRACE_1( DB , "No database has been allocate.\n");
		ret = -ENXIO;
	}
	else
	{

		/* TODO: free nodes */

		zfree( db );
	}


	return ret;
}

int insertDb( int key , int value )
{
	TRACE_2( DB , "insertDb( %d , %d )." , key , value );

	int ret = 0;
	struct entry *e = NULL;
	struct entry *tmp = NULL;

	if( db->count == 0 )
	{
		TRACE_1( DB , "Database is empty.\n");

		e = ( struct entry * )zmalloc( sizeof( struct entry ) );

		if( !e )
		{
			TRACE_ERROR( DB , "Failed to allocate a new node.\n");
			ret = -ENOMEM;
		}
		else
		{
			e->key = key;
			e->value = value;
			e->hash = hash( key , value );
			e->used = 1;

			TRACE_1( DB , "Add node in database.\n");

			tmp = db->tail;

			tmp = e;

			db->head = tmp;
			db->tail = tmp;
			db->count++;
			e->prev = NULL;
			e->next = NULL;

		}
	}
	else
	{
		e = ( struct entry * )zmalloc( sizeof( struct entry ) );

		if( !e )
		{
			TRACE_ERROR( DB , "Failed to allocate a new node.\n");
			ret = -ENOMEM;
		}
		else
		{

			TRACE_1( DB , "Add node in database.\n");

			tmp = db->tail;

			e->key = key;
			e->value = value;
			e->hash = hash( key , value );	
			e->used = 1;

			TRACE_1( DB , "Add node in database.\n");

			tmp = db->tail;
			tmp->next = e;
			e->prev = tmp;
			e->next = NULL;

			db->tail = e;
			db->count++;
		}

	}
	
	
	return ret;
}


struct entry *searchDb( int key )
{
	TRACE_2( DB , "searchDb( %d ).\n" , key );

	struct entry *ret = NULL;
	struct entry *tmp = NULL;

	tmp = db->head;

	if( !tmp )
	{
		TRACE_WARNING( DB , "Database has no head !!!!\n");
		
		ret = NULL;
	}
	else
	{
		do
		{
			if( tmp->key == key )
			{
				if( tmp->hash == hash( key , tmp->value ) )
				{
					ret = tmp;
					break;
				}
			}

			tmp = tmp->next;
		}while( tmp );
	}
	
	if( !ret )
		TRACE_1( DB , "No pair with key ( %d ) has been found !\n" , key );

	return ret;
}

int removeDb( int key )
{
	TRACE_2( DB , "removeDb( %d ).\n" , key );

	int ret = 0;
	struct entry *tmp = NULL;
	struct entry *p = NULL;


	tmp = db->head;

	if( !tmp )
	{
		TRACE_WARNING( DB , "Database has no head !!!\n");
		ret = -ENODATA;
	}
	else
	{
		do
		{
			if( tmp->key == key )
			{

				if( tmp->prev && tmp->next ) /* Removing node in the middle of list */
				{
					p = tmp->prev;
					p->next = tmp->next;
				
					p = tmp->next;
					p->prev = tmp->prev;
				}
				else if( !tmp->prev ) /* Removing head node */
				{
					db->head = tmp->next;
					db->head->prev = NULL;
				}
				else if( !tmp->next ) /* Removing tail node */
				{
					db->tail = tmp->prev;
					db->tail->next = NULL;
				}

				zfree( tmp );
				ret = 0;
				break;
			}
			else
			{
				ret = -ENODATA;
			}
		
			tmp = tmp->next;

		}while( tmp );
	}

	if( ret < 0 )
		TRACE_1( DB , "No pair with key ( %d ) has been found, so nothing has been removed.\n" , key );

	return ret;
}

//static void sortAsc( struct entry **a , int n )
//{
//	TRACE_2( DB , "sortAsc( %p , %d )." , a , n );
//	
//	if( n < 2 )
//		return;
//
//	int p =  a[n / 2]->key;
//	struct entry **l = a;
//	struct entry **r = a + n - 1;
//	struct entry *t;
//
//	while( l <= r )
//	{
//		TRACE_3( DB , "%p , %p\n" , l , r );
//		TRACE_3( DB , "%x , %x\n" , (*l)->key , (*r)->key );
//
//		if( (*l)->key < p )
//			*l++;
//		else if( (*r)->key > p )
//			*r--;
//		else
//		{
//
//			t = *l;
//			*l++ = *r;
//			*r-- = t;
//		}
//	}
//
//
//	sortAsc( a , r - a + 1 );
//	sortAsc( l, a + n - l );
//}

//static void sortDesc( struct entry **a , int n )
//{
//	TRACE_2( DB , "sortDesc( %p , %d )." , a , n );
//	
//	if( n < 2 )
//		return;
//
//	int p =  a[n / 2]->key;
//	struct entry **l = a;
//	struct entry **r = a + n - 1;
//	struct entry *t;
//
//	while( l <= r )
//	{
//		TRACE_3( DB , "%p , %p\n" , l , r );
//		TRACE_3( DB , "%x , %x\n" , (*l)->key , (*r)->key );
//
//		if( (*l)->key > p )
//			*l++;
//		else if( (*r)->key < p )
//			*r--;
//		else
//		{
//
//			t = *l;
//			*l++ = *r;
//			*r-- = t;
//		}
//	}
//
//
//	sortDesc( a , r - a + 1 );
//	sortDesc( l, a + n - l );
//}



//void sortDb( const char *name , int n , int direction )
//{
//	TRACE_2( DB , "sortAscDb( %s , %d ).\n" , name , n );
//
//	int index;
//	struct entry **p = NULL;
//
//	index = getDatabase( name );
//	p = databases[index]->db;
//
//	if( direction )
//		sortDesc( p , n );
//	else
//		sortAsc( p , n );
//
//}

char *sort( struct environment *env )
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
//		sortDb("testdb" , MAX_DB_SIZE , 0 );

		snprintf( status , 24 , "OK" );
	}

	return status;
}

char *print( struct environment *env )
{
	TRACE_2( DB , "print().\n");

	char *status = NULL;
	struct entry *tmp = NULL;
	int nw = 0;
	int i = 0;

	status = ( char * )zmalloc( ( db->count * sizeof( int ) * 4 ) * sizeof( char ) );
	

	if( !status )
	{
		TRACE_ERROR( DB , "Failed to allocate status string.\n");
	}
	else
	{
		tmp = db->head;

		if( !tmp )
		{
			TRACE_WARNING( DB , "Database has no head !!!\n");
			sprintf( status , "Error: database has no head.\n");
		}
		else
		{
			for( i = 0 ; i < db->count ; i++ )
			{
				if( !tmp )
					break;

				nw += sprintf( status + nw , "%d - %d - %x\n" , tmp->key , tmp->value , tmp->hash );

				tmp = tmp->next;
			}

		}
	}
	
	return status;	
}

char *setPair( struct environment *env )
{
	TRACE_2( DB , "setPair( %d , %d ).\n" , env->genericVal[0] , env->genericVal[1] );

	int ret = 0;
	char *status = NULL;
	unsigned int key;
	unsigned int value;

	key = env->genericVal[0];
	value = env->genericVal[1];

	ret = insertDb( key , value );

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
char *getPair( struct environment *env )
{
	TRACE_2( DB , "getPair( %d ).\n" , env->genericVal[0] );

	struct entry *ret = NULL;
	char *status = NULL;
	unsigned int key;

	key = env->genericVal[0];

	status = ( char * )zmalloc( 124 * sizeof( char ) );

	if( !status )
	{
		TRACE_ERROR( DB , "Failed to allocate status string.\n");
	}
	else
	{
		ret = searchDb( key );

		if( !ret )
			snprintf( status , 124 , "Cannot retrieve pair in database.\n");
		else
			snprintf( status , 124 , "%d - %d - %x\n" , ret->key , ret->value , ret->hash );
	}

	return status;
}

/* value argument is not used, this argument is here just to use the cli (required 2 unsigned int args ) */
char *removePair( struct environment *env )
{
	TRACE_2( DB , "removePair( %d ).\n" , env->genericVal[0] );

	int ret = 0;
	char *status = NULL;
	unsigned int key;

	key = env->genericVal[0];

	status = ( char * )zmalloc( 124 * sizeof( char ) );
	
	if( !status )
	{
		TRACE_ERROR( DB , "Failed to allocate status string.\n");
	}
	else
	{
		ret = removeDb( key );

		if( ret < 0 )
			snprintf( status , 124 , "Cannot remove pair in database.\n");
		else
			snprintf( status , 124 , "Removed: %d\n" , key );
	}


	return status;
}
