/*  wMusic - Music system software
 *  Copyright (C) 2013  Raphaël POGGI
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "zmemory.h"

static int memoryAllocate = 0;
static int memoryFree = 0;

void *zmalloc( size_t size )
{
    TRACE_2( ZMEMORY , "zmalloc( %d )." , size );

    memoryAllocate += size + sizeof( size_t );

	void *p = malloc( size + sizeof( size_t ) );

	*( ( size_t * ) p ) = size;

    return p + sizeof( size_t );
}

void *zcalloc( size_t count , size_t size )
{
    TRACE_2( ZMEMORY , "zcalloc( %d , %d )." , count , size );

    memoryAllocate += size;

	void *p = calloc( count , size );

	memset( p , 0 , count * size );

    return p;
}

void zfree( void *ptr )
{
    TRACE_2( ZMEMORY , "zfree().");

    void *realptr;
    size_t sizeptr;

    if( ptr == NULL )
        return;

    realptr = ( char * )ptr - sizeof( size_t );
    sizeptr = *( ( size_t * )realptr );

    TRACE_1( ZMEMORY , "Size to be free : %d." , sizeptr );

    memoryFree += sizeptr;

    free( realptr );
}

char *getMemoryCount( struct environment *env )
{
    TRACE_2( ZMEMORY , "getMemoryCount().");

    char *buff = ( char * )zmalloc( 248 * sizeof( char ) );

    memset( buff , 0 , 248 );

	env->memoryAllocate = memoryAllocate;
	env->memoryFree = memoryFree;

    sprintf( buff , "Total memory allocate : %d\n" , env->memoryAllocate );
    sprintf( buff + strlen( buff ), "Total memory free : %d\n" , env->memoryFree );
    sprintf( buff + strlen( buff ), "Current memory allocate : %d\n" , env->memoryAllocate - env->memoryFree );

    return buff;
}
