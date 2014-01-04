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

#include "thread.h"

static int countThreads = 0;

void createThread( void *routine , void *arg )
{
    TRACE_2( THREAD , "createThread()");

    pthread_t newThread;

    if( arg == NULL )
    {
        pthread_create( &newThread , NULL , ( void * ) routine , NULL );

        countThreads++;
    }
    else
    {
        pthread_create( &newThread , NULL , ( void * ) routine , arg );

        countThreads++;
    }

    TRACE_1( THREAD , "Current number of threads: %d." , countThreads );
}

void incrementThreadCount( int n )
{
    TRACE_2( THREAD , "incrementThreadCount( %d )." , n );

    countThreads += n;
}

char *getThreadCount( void )
{
    TRACE_2( THREAD , "getThreadCount().");

    char *buff = ( char * )zmalloc( 40 * sizeof( char ) );

    memset( buff , 0 , 40 );

    sprintf( buff , "Total thread running : %d\n" , countThreads );

    return buff;
}

void releaseThread( void )
{
    TRACE_2( THREAD , "releaseThread().");

    countThreads--;
}
