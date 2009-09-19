#include <stdio.h>
#include <stdlib.h>
#include "../libaof/libaof.h"

void __declspec(noreturn) error( int exitcode, char const * msg )
{
	fflush(0);
	fprintf( stderr, "%s\n", msg );
	exit( exitcode );
}

int main( int argc, char ** argv )
{
	struct aof_drs * drs = drs_open( "c:\\games\\aoe\\data\\graphics.drs" );
	if (!drs)
		error( 1, "Cannot open DRS archive." );

	puts( "archive opened." );

	drs_dump( drs );

	{
		int len;
		void const * ptr = drs_file( drs, 0, "slp", &len );
		if (!ptr)
			error( 1, "Cannot fetch slp0" );

		printf( "slp0: {0x%x,%x}\n", ptr, len );

		{
			int frames, w, h, x, y;
			frames = slp_frames( ptr );
			slp_size( ptr, &w, &h, 0 );
			slp_offset( ptr, &x, &y, 0 );

			printf( "slp0 has %d frames: %dx%d hs={%d,%d}\n",
				frames, w, h, x, y );

			slp_render( ptr, 0, 0, 0, 640, 320, 240 );
		}
	}

	drs_close( drs );
}