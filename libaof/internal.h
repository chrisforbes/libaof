#pragma once

#ifdef WIN32
typedef void * HANDLE;
#endif

struct aof_drs
{
#ifdef WIN32
	HANDLE hFile;
	HANDLE hMapping;
#else
	int fd;
#endif

	void * base;
	int size;
};

#pragma pack( push, 1 )

struct aof_drs_header
{
	char copyright[40];
	int version;			/* should be "1.00" [at least for RTM] */
	char filetype[12];		/* should be "tribe" */
	int numtables;
	int first_file_offset;
};

struct aof_table_header
{
	char type;				/* related with filetype, for some reason */
	char rext[3];			/* reversed extension (don't know why) */
	int offset;
	int numfiles;
};

struct aof_file_header
{
	int id;
	int offset;
	int size;
};

#pragma pack( pop )