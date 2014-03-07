/*  rdb - In-memory database optimized for embedded world
 *  Copyright (C) 2013  Raphaël Poggi
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

#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>

#include <types.h>
#include <trace.h>
#include <zmemory.h>
#include <common.h>

void createThread( void *routine , void *arg );
void incrementThreadCount( int n );
char *getThreadCount( struct environment *env );
void releaseThread( void );

#endif /* THREAD_H */
