#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef LIBAOF_EXPORTS
#define AOFEXPORT __declspec( dllexport )
#else
#define AOFEXPORT __declspec( dllimport )
#endif
#endif

	/* drs interface */
	AOFEXPORT struct aof_drs * drs_open( char const * filename );
	AOFEXPORT void drs_close( struct aof_drs * drs );
	AOFEXPORT void const * drs_file( struct aof_drs * drs, int id, char const * ext, int * len );
	AOFEXPORT void drs_dump( struct aof_drs const * drs );

	/* slp interface */
	AOFEXPORT void slp_size( void const * slp, int * w, int * h, int frame );
	AOFEXPORT void slp_offset( void const * slp, int * x, int * y, int frame );
	AOFEXPORT int slp_frames( void const * slp );
	AOFEXPORT void slp_render( void const * slp, int frame, int player, 
		unsigned char * p, int stride, int x, int y );

#ifdef __cplusplus
}
#endif