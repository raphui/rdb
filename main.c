#include <stdio.h>
#include <stdlib.h>

#include <db.h>

int main( int argc , char **argv )
{
	createDb("testdb");
	insert( 0x13 , 0x20 );
	insert( 0x3EF , 0x2344 );
	insert( 0xFE4F , 0x6576 );
	printDb();

	return 0;
}
