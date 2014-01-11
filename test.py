#!/usr/bin/python
import socket
import sys
import time
import random

HOST = '127.0.0.1'
PORT = 1339

s = socket.socket( socket.AF_INET , socket.SOCK_STREAM )

s.connect( ( HOST , PORT ) )

usleep = lambda x: time.sleep( x / 1000000.0 );

for i in range( 1 , 1024 ) :
	s.sendall( ('set ' + str( random.randint( 1 , 999999 ) ) + ' ' + str( random.randint( 1 , 999999 ) ) ) )
	print '{0}: Sended'.format( i )
	i = i + 1
	usleep( 100000 );

#s.sendall( ( sys.argv[1] ) )

s.close()

#print 'Sended' , sys.argv[1]
