#include <stdio.h>
#include <stdlib.h>

#include "libaof.h"
#include "internal.h"

void drs_dump( struct aof_drs const * drs )
{
	struct aof_drs_header const * h = drs->base;
	struct aof_table_header const * t = (struct aof_table_header const *)(h+1);
	int n = h->numtables;

	printf( "DRS contains %d tables.\n", h->numtables );

	for( ; n; t++, n-- )
		printf( "table %d: %d .%c%c%c files\n",
			h->numtables - n,
			t->numfiles,
			t->rext[2], t->rext[1], t->rext[0] );
}