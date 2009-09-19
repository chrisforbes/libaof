#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "libaof.h"
#include "internal.h"

struct aof_drs * drs_open( char const * filename )
{
	struct aof_drs * drs = malloc( sizeof( *drs ) );

	if (INVALID_HANDLE_VALUE == 
		(drs->hFile = CreateFileA( filename, GENERIC_READ, 
		FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 )))
		goto cleanup_none;

	if (!(drs->hMapping = 
		CreateFileMappingA( drs->hFile, 0, PAGE_WRITECOPY, 0, 0, 0 )))
		goto cleanup_file;

	drs->size = GetFileSize( drs->hFile, 0 );

	if (!(drs->base = 
		MapViewOfFile( drs->hMapping, FILE_MAP_COPY, 0, 0, 0 )))
		goto cleanup_mapping;

	return drs;
	
cleanup_mapping:
	CloseHandle( drs->hMapping );
cleanup_file:
	CloseHandle( drs->hFile );
cleanup_none:
	free( drs );
	return 0;
}

void drs_close( struct aof_drs * drs )
{
	UnmapViewOfFile( drs->base );
	CloseHandle( drs->hMapping );
	CloseHandle( drs->hFile );
	free( drs );
}