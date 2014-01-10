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

#ifndef TRACE_H
#define TRACE_H

#ifndef HDEBUG_H
#define HDEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include <zmemory.h>

#define MODULE_COUNT    17

#define TRACE_LEVEL_ERR    ( 1 << 0 )
#define TRACE_LEVEL_WARN   ( 1 << 1 )
#define TRACE_LEVEL_INFO   ( 1 << 2 )
#define TRACE_LEVEL_1      ( 1 << 3 )
#define TRACE_LEVEL_2      ( 1 << 4 )
#define TRACE_LEVEL_3      ( 1 << 5 )

#define TRACE_LEVEL_DEFAULT (TRACE_LEVEL_ERR | TRACE_LEVEL_WARN | TRACE_LEVEL_INFO )
#define TRACE_LEVEL_MID     (TRACE_LEVEL_ERR | TRACE_LEVEL_WARN | TRACE_LEVEL_INFO | TRACE_LEVEL_1 )
#define TRACE_LEVEL_FUNC    (TRACE_LEVEL_ERR | TRACE_LEVEL_WARN | TRACE_LEVEL_INFO | TRACE_LEVEL_2 )
#define TRACE_LEVEL_ALL     (TRACE_LEVEL_ERR | TRACE_LEVEL_WARN | TRACE_LEVEL_INFO \
                                             | TRACE_LEVEL_1 | TRACE_LEVEL_2 \
                                             | TRACE_LEVEL_3)
#define IF_TRACES_ENABLED( LEVEL, MODULE, X ) do { if( Traces_enable(MODULE, LEVEL) == 1 ) { X; } } while(0)

#define IF_L1_ENABLED( MODULE, X) IF_TRACES_ENABLED( TRACE_LEVEL_1, MODULE, X)
#define IF_L2_ENABLED( MODULE, X) IF_TRACES_ENABLED( TRACE_LEVEL_2, MODULE, X)
#define IF_L3_ENABLED( MODULE, X) IF_TRACES_ENABLED( TRACE_LEVEL_3, MODULE, X)
#define IF_INFO_ENABLED( MODULE, X) IF_TRACES_ENABLED( TRACE_LEVEL_INFO, MODULE, X)
#define IF_WARN_ENABLED( MODULE, X) IF_TRACES_ENABLED( TRACE_LEVEL_WARN, MODULE, X)
#define IF_ERR_ENABLED( MODULE, X)  IF_TRACES_ENABLED( TRACE_LEVEL_ERR,  MODULE, X)


#define TRACE_3( MODULE, ... )          IF_L3_ENABLED( MODULE, Traces_print( __FILE__, __LINE__, __FUNCTION__, TRACE_LEVEL_3,    MODULE, __VA_ARGS__ ) );
#define TRACE_2( MODULE, ... )          IF_L2_ENABLED( MODULE, Traces_print( __FILE__, __LINE__, __FUNCTION__, TRACE_LEVEL_2,    MODULE, __VA_ARGS__ ) );
#define TRACE_1( MODULE, ... )          IF_L1_ENABLED( MODULE, Traces_print( __FILE__, __LINE__, __FUNCTION__, TRACE_LEVEL_1,    MODULE, __VA_ARGS__ ) );
#define TRACE_0( LEVEL, MODULE, ... )   Traces_printOnly( LEVEL, MODULE, __FUNCTION__, __VA_ARGS__ );
#define TRACE_INFO( MODULE, ... )       IF_INFO_ENABLED( MODULE, Traces_print( __FILE__, __LINE__, __FUNCTION__, TRACE_LEVEL_INFO, MODULE, __VA_ARGS__ ) );
#define TRACE_WARNING( MODULE, ... )    IF_WARN_ENABLED( MODULE, Traces_print( __FILE__, __LINE__, __FUNCTION__, TRACE_LEVEL_WARN, MODULE, __VA_ARGS__ ) );
#define TRACE_ERROR( MODULE, ... )      IF_ERR_ENABLED( MODULE, Traces_print( __FILE__, __LINE__, __FUNCTION__, TRACE_LEVEL_ERR,  MODULE, __VA_ARGS__ ) );


extern void Traces_printOnly( unsigned int level, unsigned int module, const char *function, const char *format, ... );
//extern void HTraces_print( unsigned int level, unsigned int module, const char *file , const int *line , const char *function, const char *format, ... );
extern void Traces_print( const char *file , const int line , const char *function , unsigned int level , unsigned int module ,  const char *format , ... );
extern int Traces_enable( unsigned int module , unsigned int level );
extern char *setTraceLevel( unsigned int module , unsigned int level );
extern char *dumpTrace( void );
extern char *levelInfo( void );

enum {

	DB,
	CLI,
    THREAD,
    ZMEMORY,
};



#endif /* HDEBUG_H */


#endif /* TRACE_H */
