// Created 08.06.2020
#ifndef MY_TAR_H
#define MY_TAR_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct posix_header {
	char name[ 100 ];
	char mode[ 8 ];
	char uid[ 8 ];
	char gid[ 8 ];
	char size[ 12 ];
	char mtime[ 12 ];
	char chksum[ 8 ];
	char typeflag;
	char linkname[ 100 ];
	char magic[ 6 ];
	char version[ 2 ];
	char uname[ 32 ];
	char gname[ 32 ];
	char devmajor[ 8 ];
	char devminor[ 8 ];
	char prefix[ 155 ];
} headerInfo;

struct Options
{
    bool create;
    bool extract;
    bool list;
    int archfd;
    char* archname;
    char** files;
    int error;
    int argcount;
    int fcount;
    char *path;
    _Bool append_new;
    _Bool append;
    int mod;
};

struct Options getOpts(int argc, char *argv[]);
void freeOpts (struct Options);
int create(int fd_arch, char** files, int fcount);
int extract(int fd_arch, char** files);

char *my_itoa( char *res, unsigned int number, int size, int base );

headerInfo *getHeaderInfo(char *filename);
void writeFileContent( int fd_archive, char *filename );
void fillMissing( int fd_arch );
int list(int fd_arch, char** files);

#endif
