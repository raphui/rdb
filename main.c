#include <stdio.h>
#include <stdlib.h>

#include <db.h>
#include <cli.h>
#include <server.h>
#include <common.h>
#include <zmemory.h>

int main( int argc , char **argv )
{
	char command[124] = { 0 };
	char *opt = NULL;
	int ret = 0;
	char *status = NULL;

	ret = initEnvironment();

	if( ret < 0 )
		return -1;

	createDb();

	if( argc > 1 )
	{
		opt = strtok( argv[1] , "-");

		if( !strcmp( opt , "t") )
			launchServer();
	}
	

	while( 1 )
	{
		printf("rdb~>");
		fgets( command , 124 , stdin );

		if( !strcmp("q\n" , command ) )
			break;
		else
		{
			status = doCommand( command );
			
			if( !status )
				printf("An error occurs.\n");
			else
			{
				printf("%s\n" , status );
				zfree( status );
			}

			
		}

	}

	return 0;
}
