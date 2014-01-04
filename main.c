#include <stdio.h>
#include <stdlib.h>

#include <db.h>

int main( int argc , char **argv )
{
	createDb("testdb");
	insert( 0x13 , 0x20 );
	insert( 0x3EF , 0x2344 );
	insert( 0xFE4F , 0x6576 );
	insert( 0x343DA , 0xDAE13 );
	insert( 0xE82B8A , 0x1245 );
	insert( 0x6380 , 0xF9248 );
	insert( 0x2343 , 0x1ED123 );
	insert( 0x123DF , 0x7543D );
	insert( 0x234E , 0x6841 );
	printDb();
	sortAsc( db , MAX_DB_SIZE );
	printFullDb();

	return 0;
}
