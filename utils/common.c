#include "common.h"

int initEnvironment( void )
{
	TRACE_2( COMMON , "initEnvironment().\n");

	int ret = 0;

	env = ( struct environment * )zmalloc( sizeof( struct environment ) );

	if( !env )
	{
		TRACE_ERROR( COMMON , "Failed to allocate environment structure.\n");
		ret = -ENOMEM;
	}

	return ret;
}
