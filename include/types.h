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

#ifndef TYPES_H
#define TYPES_H

#include <pthread.h>

#include <trace.h>

//#define PC_SUCCESS          0
//#define PC_ERROR            -1

#define TRUE                1
#define FALSE               0

/**** Compilation Environment settings ****/
#define CROSS_COMPILE       0


/**** Mutex tracking ****/
#define LOCK_MUTEX( MODULE , MUTEX )    TRACE_1( MODULE , "Lock Mutex !") \
                                        pthread_mutex_lock( MUTEX );

#define UNLOCK_MUTEX( MODULE , MUTEX )    TRACE_1( MODULE , "Unlock Mutex !") \
                                        pthread_mutex_unlock( MUTEX );
#endif // TYPES_H
