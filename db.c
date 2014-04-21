#include "db.h"

static struct database *db;

static int crc( char *val )
{
    TRACE_2( DB , "crc( %s )." , val );

    int poly = 0xEDB88320;
    int crc = 0;
    unsigned int length = strlen( val );
    unsigned int i = 0;
    char *p = val;

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

static int hash( char *key , char *value )
{

    TRACE_2( DB , "hash( %s , %s )." , key , value );

    int hash = 0;

    hash = crc( key );
    hash ^= crc( value );

    return hash;
}

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

int insertDb( char *key , char *value )
{
    TRACE_2( DB , "insertDb( %s , %s )." , key , value );

    int ret = 0;
    struct entry *e = NULL;
    struct entry *tmp = NULL;
    size_t size = 0;
    size_t global_size = 0;

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
            size = strlen( key );

            if( size > MAX_STRING_SIZE )
            {
                e->key = ( char * )zmalloc( MAX_STRING_SIZE * sizeof( char ) );
                size = MAX_STRING_SIZE;
            }
            else
                e->key = ( char * )zmalloc( size * sizeof( char ) );

            strncpy( e->key , key , size );
	    global_size += size;

            size = strlen( value );

            if( size > MAX_STRING_SIZE )
            {
                e->value = ( char * )zmalloc( MAX_STRING_SIZE * sizeof( char ) );
                size = MAX_STRING_SIZE;
            }
            else
                e->value = ( char * )zmalloc( size * sizeof( char ) );

            strncpy( e->value , value , size );
	    global_size += size;

            e->hash = hash( key , value );
            e->used = 1;
	    global_size += sizeof( e->hash );
	    global_size += sizeof( e->used );

	    e->size = global_size;

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

            size = strlen( key );

            if( size > MAX_STRING_SIZE )
            {
                e->key = ( char * )zmalloc( MAX_STRING_SIZE * sizeof( char ) );
                size = MAX_STRING_SIZE;
            }
            else
                e->key = ( char * )zmalloc( size * sizeof( char ) );

            strncpy( e->key , key , size );

            size = strlen( value );

            if( size > MAX_STRING_SIZE )
            {
                e->value = ( char * )zmalloc( MAX_STRING_SIZE * sizeof( char ) );
                size = MAX_STRING_SIZE;
            }
            else
                e->value = ( char * )zmalloc( size * sizeof( char ) );

            strncpy( e->value , value , size );

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


struct entry *searchDb( char *key )
{
    TRACE_2( DB , "searchDb( %s ).\n" , key );

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
            if( !strcmp( tmp->key , key ) )
            {
                if( tmp->hash == hash( key , tmp->value ) )
                {
                    ret = tmp;
		    tmp->used++;
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

int removeDb( char *key )
{
    TRACE_2( DB , "removeDb( %s ).\n" , key );

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
            if( !strcmp( tmp->key , key ) )
            {

                if( tmp->prev && tmp->next ) /* Removing node in the middle of list */
                {
                    p = tmp->prev;
                    p->next = tmp->next;

                    p = tmp->next;
                    p->prev = tmp->prev;
                    db->count--;
                }
                else if( !tmp->prev && !tmp->next ) /* Removing last node in list */
                {
                    db->head = NULL;
                    db->tail = NULL;
                    db->count = 0;
                }
                else if( !tmp->prev ) /* Removing head node */
                {
                    db->head = tmp->next;
                    db->head->prev = NULL;
                    db->count--;
                }
                else if( !tmp->next ) /* Removing tail node */
                {
                    db->tail = tmp->prev;
                    db->tail->next = NULL;
                    db->count--;
                }

                zfree( tmp->key );
                zfree( tmp->value );
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

int flushDb( void )
{
    TRACE_2( DB , "flushDb().\n");

    int ret = 0;
    struct entry *tmp = NULL;
    struct entry *p = NULL;

    if( !db->count )
    {
        TRACE_WARNING( DB , "Database is empty.\n");
        ret = -ENODATA;
    }
    else
    {
        tmp = db->head;
        p = tmp->next;

        while( --( db->count ) )
        {
            zfree( tmp->key );
            zfree( tmp->value );
            zfree( tmp );
            tmp = p;
            p = p->next;
        }

        db->head = NULL;
        db->tail = NULL;
        db->count = 0;
    }

    return ret;
}

int compressDb( void )
{
    TRACE_2( DB , "compressDb().\n");
    int ret = 0;
    int i = 0;
    struct entry *tmp = NULL;
    size_t compressed_size = 0;

    if( !db->count )
    {
	TRACE_WARNING( DB , "Database is empty.\n");
	ret = -ENODATA;
    }
    else
    {
	tmp = db->head;

	if( !tmp )
	{
	    TRACE_WARNING( DB , "Database has no head !!!\n");
	}
	else
	{
	    for( i = 0 ; i < db->count ; i++ )
	    {
		if( !tmp )
		    break;

		lzf_compress( tmp , tmp->size , tmp , tmp->size );

		tmp = tmp->next;
	    }
	}
    }

    return ret;
}

int decompressDb( void )
{
    TRACE_2( DB , "decompressDb().\n");
    int ret = 0;
    int i = 0;
    struct entry *tmp = NULL;
    size_t compressed_size = 0;

    if( !db->count )
    {
	TRACE_WARNING( DB , "Database is empty.\n");
	ret = -ENODATA;
    }
    else
    {
	tmp = db->head;

	if( !tmp )
	{
	    TRACE_WARNING( DB , "Database has no head !!!\n");
	}
	else
	{
	    for( i = 0 ; i < db->count ; i++ )
	    {
		if( !tmp )
		    break;

		lzf_decompress( tmp , tmp->size , tmp , tmp->size );

		tmp = tmp->next;
	    }
	}
    }

    return ret;
}


char *print( struct environment *env )
{
    TRACE_2( DB , "print().\n");

    char *status = NULL;
    struct entry *tmp = NULL;
    int nw = 0;
    int i = 0;

    if( db->count )
    {
        status = ( char * )zmalloc( db->count * 124 ); /*  The output string has a size of 124 */
    }

    /* If db->count == 0 , status will stay at NULL and will be handle in this condition. */

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

                nw += sprintf( status + nw , "%s - %s - %x\n" , tmp->key , tmp->value , tmp->hash );

                tmp = tmp->next;
            }

        }
    }

    return status;
}

char *setPair( struct environment *env )
{
    TRACE_2( DB , "setPair( %s , %s ).\n" , env->genericVal[0] , env->genericVal[1] );

    int ret = 0;
    char *status = NULL;
    char *key;
    char *value;

    status = ( char * )zmalloc( 124 * sizeof( char ) );

    if( !status )
    {
        TRACE_ERROR( DB , "Failed to allocate status string.\n");
    }
    else
    {
        if( env->arg_count > 2 )
        {
            TRACE_ERROR( DB , "Invalid arguments (max: 2 args).\n");
            snprintf( status , 124 , "Invalid arguments (max: 2 args).\n");
        }
        else if( env->arg_count < 2 )
        {
            TRACE_ERROR( DB , "Invalid arguments (min: 2 args).\n");
            snprintf( status , 124 , "Invalid arguments (min: 2 args).\n");
        }
        else
        {

            key = env->genericVal[0];
            value = env->genericVal[1];

            ret = insertDb( key , value );

            if( ret < 0 )
                snprintf( status , 124 , "Cannot insert in database.\n");
            else
                snprintf( status , 124 , "OK");
        }
    }

    return status;
}

/* value argument is not used, this argument is here just to use the cli (required 2 unsigned int args ) */
char *getPair( struct environment *env )
{
    TRACE_2( DB , "getPair( %s ).\n" , env->genericVal[0] );

    struct entry *ret = NULL;
    char *status = NULL;
    char *key;

    status = ( char * )zmalloc( 124 * sizeof( char ) );

    if( !status )
    {
        TRACE_ERROR( DB , "Failed to allocate status string.\n");
    }
    else
    {
        if( env->arg_count > 1 )
        {
            TRACE_ERROR( DB , "Invalid arguments (max: 1 args).\n");
            snprintf( status , 124 , "Invalid arguments (max: 1 args).\n");
        }
        else if( env->arg_count < 1 )
        {
            TRACE_ERROR( DB , "Invalid arguments (min: 1 args).\n");
            snprintf( status , 124 , "Invalid arguments (min: 1 args).\n");
        }
        else
        {
            key = env->genericVal[0];

            ret = searchDb( key );

            if( !ret )
                snprintf( status , 124 , "Cannot retrieve pair in database.\n");
            else
                snprintf( status , 124 , "%s - %s - %x\n" , ret->key , ret->value , ret->hash );
        }
    }

    return status;
}

/* value argument is not used, this argument is here just to use the cli (required 2 unsigned int args ) */
char *removePair( struct environment *env )
{
    TRACE_2( DB , "removePair( %d ).\n" , env->genericVal[0] );

    int ret = 0;
    char *status = NULL;
    char *key;

    status = ( char * )zmalloc( 124 * sizeof( char ) );

    if( !status )
    {
        TRACE_ERROR( DB , "Failed to allocate status string.\n");
    }
    else
    {
        if( env->arg_count > 1 )
        {
            TRACE_ERROR( DB , "Invalid arguments (max: 1 args).\n");
            snprintf( status , 124 , "Invalid arguments (max: 1 args).\n");
        }
        else if( env->arg_count < 1 )
        {
            TRACE_ERROR( DB , "Invalid arguments (min: 1 args).\n");
            snprintf( status , 124 , "Invalid arguments (min: 1 args).\n");
        }
        else
        {
            key = env->genericVal[0];

            ret = removeDb( key );

            if( ret < 0 )
                snprintf( status , 124 , "Cannot remove pair in database.\n");
            else
                snprintf( status , 124 , "Removed: %s\n" , key );
        }
    }


    return status;
}


char *flush( struct environment *env )
{

    TRACE_2( DB , "flush().\n");
    int ret = 0;
    char *status = NULL;

    status = ( char * )zmalloc( 124 * sizeof( char ) );

    if( !status )
    {
        TRACE_ERROR( DB , "Failed to allocate status string.\n");
    }
    else
    {
        ret = flushDb();

        if( ret < 0 )
            snprintf( status , 124 , "Cannot flush database.\n");
        else
            snprintf( status , 124 , "OK\n");
    }

    return status;
}

char *compress( struct environment *env )
{
    TRACE_2( DB , "compress().\n");
    int ret = 0;
    char *status = NULL;

    status = ( char * )zmalloc( 124 * sizeof( char ) );

    if( !status )
    {
        TRACE_ERROR( DB , "Failed to allocate status string.\n");
    }
    else
    {

	ret = compressDb();

        if( ret < 0 )
            snprintf( status , 124 , "Cannot compress database.\n");
        else
            snprintf( status , 124 , "OK\n");
    }

    return status;
}

char *decompress( struct environment *env )
{
    TRACE_2( DB , "decompress().\n");
    int ret = 0;
    char *status = NULL;

    status = ( char * )zmalloc( 124 * sizeof( char ) );

    if( !status )
    {
        TRACE_ERROR( DB , "Failed to allocate status string.\n");
    }
    else
    {
	ret = decompressDb();

        if( ret < 0 )
            snprintf( status , 124 , "Cannot decompress database.\n");
        else
            snprintf( status , 124 , "OK\n");
    }

    return status;
}
