#include <stdio.h>
#include <stdlib.h>

#include "libaof.h"
#include "internal.h"

static struct aof_table_header const * find_table( struct aof_drs * drs, char const * ext )
{
	struct aof_drs_header const * h = drs->base;
	struct aof_table_header const * t = (struct aof_table_header const *)(h+1);
	int n = h->numtables;

	for( ; n--; t++ )
		if (ext[0] == t->rext[2] && 
			ext[1] == t->rext[1] && 
			ext[2] == t->rext[0])
			return t;

	return 0;
}

void const * drs_file( struct aof_drs * drs, int id, char const * ext, int * len )
{
	struct aof_table_header const * t = find_table( drs, ext );
	struct aof_file_header const * f = (struct aof_file_header const *)((char const *)drs->base + t->offset) + id;

	if (id >= t->numfiles) return 0;
	*len = f->size;
	return (char const *)drs->base + f->offset;
}