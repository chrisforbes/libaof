#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "libaof.h"
#include "internal.h"

#ifdef WIN32
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

#else
/* POSIX version*/

struct aof_drs * drs_open( char const * filename )
{
	struct aof_drs * drs = malloc( sizeof( *drs ) );

	struct stat st;
	if (stat( filename, &st )) goto cleanup_none;
	drs->size = st.st_size;

	drs->fd = open( filename, O_RDONLY );
	if (drs->fd == -1) goto cleanup_none;

	drs->base = mmap( 0, drs->size, PROT_READ | PROT_WRITE, MAP_PRIVATE, drs->fd, 0 );
	if (drs->base == (void *)-1) goto cleanup_fd;

	return drs;

cleanup_fd:
	close( drs->fd );
cleanup_none:
	free( drs );
	return 0;
}

void drs_close( struct aof_drs * drs )
{
	munmap( drs->base, 
		(drs->size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1) );
	close( drs->fd );
}

#endif