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

#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>

#include <types.h>
#include <trace.h>
#include <thread.h>
#include <cli.h>

#define BUFF_SIZE       1024
#define MAX_CLIENT      10
#define PORT_CLI        1339

typedef struct argumentReceivingThread
{
    int socket;
    int port;

}argumentReceivingThread_t;

void launchServer( void );
void createServer( void *port );
int closeServer( void );
void receivingThread( void *arg );
int disconnectClient( int *socket );

void sendVoid( void *data , size_t size );
void sendVoidSocket( int socket , void *data , size_t size );

pthread_mutex_t mutex;

#endif // SERVER_H

