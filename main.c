#include <stdio.h>
#include <stdlib.h>

#include <db.h>
#include <cli.h>
#include <server.h>

int main( int argc , char **argv )
{
	char command[124] = { 0 };
	char *opt = NULL;

	createDb("testdb");

	if( argc > 0 )
	{
		opt = strtok( argv[1] , "-");

		if( !strcmp( opt , "t") )
			launchServer();
	}
	
//	insertDb( "testdb" , 0x13 , 0x20 );
//	insertDb( "testdb" , 0x3EF , 0x2344 );
//	insertDb( "testdb" , 0xFE4F , 0x6576 );
//	insertDb( "testdb" , 0x343DA , 0xDAE13 );
//	insertDb( "testdb" , 0xE82B8A , 0x1245 );
//	insertDb( "testdb" , 0x6380 , 0xF9248 );
//	insertDb( "testdb" , 0x2343 , 0x1ED123 );
//	insertDb( "testdb" , 0x123DF , 0x7543D );
//	insertDb( "testdb" , 0x234E , 0x6841 );
//	printDb("testdb");
//	sortAscDb( "testdb" , MAX_DB_SIZE );
//	printFullDb("testdb");
//	destroyDb("testdb");


	while( 1 )
	{
		printf("rdb~>");
		gets( command );

		if( !strcmp("q" , command ) )
			break;
		else
			printf("%s\n" , ( char * )doCommand( command ) );

	}

	return 0;
}
