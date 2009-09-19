#pragma once

#pragma pack( push, 1 )

struct slp_file_header
{
	char version[4];
	int numframes;
	char comment[24];
};

struct slp_frame_info
{
	int cmd_table_offset;
	int outline_table_offset;
	int palette_offset;
	int props;

	int width;
	int height;
	int hsx;
	int hsy;
};

struct slp_row 
{ 
	unsigned short left, right; 
};

enum slp_cmd 
{ 
	cmd_color_list, 
	cmd_skip, 
	cmd_big_color_list,
	cmd_big_skip,
	cmd_player_color_list = 0x6,
	cmd_fill,
	cmd_player_color_fill = 0xa,
	cmd_shadow_transparent,
	cmd_shadow_player = 0xe,
	cmd_end_of_row = 0xf,
	cmd_outline = 0x4e,
	cmd_outline_span = 0x5e,
};

#pragma pack( pop )