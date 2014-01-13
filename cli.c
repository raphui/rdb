/*  rdb - In-memory database optimized for embedded world
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

#include "cli.h"

static char *help( struct environment *env );

static cliCommand_t cliCmd[] =
{
    {"memory_count"     ,   &getMemoryCount },
    {"thread_count"     ,   &getThreadCount },
    {"dump_trace_level" ,   &dumpTrace      },
    {"get_level"        ,   &levelInfo      },
    {"help"             ,   &help           },
	{"sort"				,	&sort			},
	{"print"			,	&print			},
    {"set_trace_level"  ,   &setTraceLevel  },
	{"set"				,	&setPair		},
	{"get"				,	&getPair		},
	{"remove"			,	&removePair		},
};

static char *help( struct environment *env )
{
    TRACE_2( CLI , "help().");

    int i = 0;
    char *buff = ( char * )zmalloc( 1024 * sizeof( char ) );

    memset( buff , 0 , 1024 );

    for( i = 0 ; i < CLI_COUNT_COMMAND ; i++ )
        sprintf( buff + strlen( buff ) , "%s\n" ,  cliCmd[i].command );

    return buff;
}

static int searchCliCmd( const char *cmd )
{
    TRACE_2( CLI , "searchCliCmd( %s )." , cmd );

    int i = 0;

    for( i = 0 ; i < CLI_COUNT_COMMAND ; i++ )
    {
        if( strstr( cmd , cliCmd[i].command ) != NULL )
        {
            TRACE_1( CLI , "Command found, id: %d" , i );

            return i;
        }
    }

    return -EINVAL;
}

void *doCommand( char *cmd )
{
    TRACE_2( CLI , "doCommand( %s )." , cmd );

	int i = 0;	
    int idFuncptr;
    void *ret = NULL;
	char *ptr = NULL;

    /* Dynamic allocation, because after send the response throught the socket, free() is called. (If it's declare like -char errorMsg[]="zedze"- this will crash for sure.) */
    char *errorMsg = ( char * )zmalloc( 28 * sizeof( char ) );

    memset( errorMsg , 0 , 28 );

	ptr = strtok( cmd , " ");

	for( i = 0 ; i < MAX_GENERIC_VAL ; i++ )
	{
		ptr = strtok( NULL , " " );
		
		if( !ptr )
			break;

		env->genericVal[i] = ( unsigned int )atoi( ptr );
	}

    if( ( idFuncptr = searchCliCmd( cmd ) ) >= 0 )
    {
        TRACE_3( CLI , "Execute function.");

        ret = ( void * )cliCmd[idFuncptr].func( env );

        TRACE_1( CLI , "Function return: %d" , ret );
    }
    else
    {
        TRACE_ERROR( CLI , "Command is not supported.");

        sprintf( errorMsg , "Command is not supported.\n");

        ret = ( void * )errorMsg;
    }

    return ret;
}
