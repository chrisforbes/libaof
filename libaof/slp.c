#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "libaof.h"
#include "slp.h"

static struct slp_frame_info const * get_frame( void const * slp, int frame )
{
	struct slp_file_header const * h = slp;
	return (struct slp_frame_info const *)(h+1) + frame;
}

void slp_size( void const * slp, int * w, int * h, int frame )
{
	struct slp_frame_info const * f = get_frame( slp, frame );
	*w = f->width;
	*h = f->height;
}

void slp_offset( void const * slp, int * x, int * y, int frame )
{
	struct slp_frame_info const * f = get_frame( slp, frame );
	*x = f->hsx;
	*y = f->hsy;
}

int slp_frames( void const * slp )
{
	struct slp_file_header const * h = slp;
	return h->numframes;
}

static unsigned char const * render_row( unsigned char * dest, 
										 unsigned char const * src, 
										 struct slp_row const * row,
										 int player )
{
	printf( "render_row --\n" );
	/* todo: clipping */
	dest += row->left;

#define arg_shr4_or_next( X )	\
	{ \
	count = (unsigned)(X & 0xf0) >> 4; \
	if (!count) count = *src++; \
	}

#define arg_shl4_next( X ) \
	{ \
	count = ((unsigned)(X & 0xf0) << 4) | *src++; \
	}

	for(;;)
	{
		unsigned char a = *src++;
		int count;
		unsigned char b;
		
		if (a == cmd_end_of_row) { return src; }
		else if (a == cmd_outline) { printf( "cmd_outline\n" ); }
		else if (a == cmd_outline_span) { printf( "cmd_outline_span %d\n", *src++ ); }
		else
			switch( a & 0x0f ) 
			{
			case cmd_color_list:
				count = a >> 2;
				printf( "cmd_color_list %d = [", count );
				while( count-- ) { 
					dest++;
					printf( "%d,", *src++ );
				}
				printf( "]\n" );
				break;

			case cmd_skip:
				count = a >> 2;
				if (!count) count = *src++;
				printf( "cmd_skip %d\n", count );
				dest += count;
				break;

			case cmd_big_color_list:
				arg_shl4_next( a );
				printf( "cmd_big_color_list %d = [...]\n", count );	
				src += count;
				dest += count;
				break;

			case cmd_big_skip:
				arg_shl4_next( a );
				printf( "cmd_big_skip %d\n", count );
				dest += count;
				break;

			case cmd_player_color_list:
				arg_shr4_or_next( a );
				printf( "cmd_player_color_list %d = [\n", count );
				while( count-- ) {
					dest++;
					b = (player+1) * 16 + *src++;
					printf( "%d,", b );
				}
				printf( "]\n" );
				break;

			case cmd_fill:
				arg_shr4_or_next( a );
				b = *src++;
				printf( "cmd_fill %d = %d\n", count, b );
				dest += count;
				break;

			case cmd_player_color_fill:
				arg_shr4_or_next( a );
				b = (player+1) * 16 + *src++;	// map to player section of paletter
				printf( "cmd_player_color_fill %d = %d\n", count, b );
				dest += count;
				break;

			case cmd_shadow_transparent:
				arg_shr4_or_next( a );		/* todo: how does one actually do this? */
				printf( "cmd_shadow_transparent %d\n", count );
				dest += count;
				break;

			case cmd_shadow_player:
				count = *src++;
				printf( "cmd_shadow_player %d\n", count );
				dest += count;
				break;

			default:
				printf( "cmd_what? = %2x\n", (unsigned)a );
			}
	}
}

void slp_render( void const * slp, int frame, int player, unsigned char * p, int stride, int x, int y )
{
	struct slp_frame_info const * f = get_frame( slp, frame );
	unsigned char const * base = (unsigned char const *)
		slp;
	//	get_frame( slp, slp_frames( slp ) );	/* ptr past end */

	unsigned const * q = base + f->cmd_table_offset;
	struct slp_row const * row = (struct slp_row const *)
		(base + f->outline_table_offset);
	unsigned char * dest = p + (x - f->hsx) + stride * (y - f->hsy);

	int n = f->height;	
	for( ; n; n--, row++, dest += stride )
		render_row( dest, base + *q++, row, player );
}