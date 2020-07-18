// Created 08.06.2020
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

struct Options
{
    char mod;
    int archfd;
    char* path;
    char* archname;
    int fcount;
    char** files;

    int error;
};

#define REGTYPE '0'
#define AREGTYPE '\0'
#define LINKTYPE '1'
#define SYMTYPE '2'
#define CHRTYPE '3'
#define BLKTYPE '4'
#define DIRTYPE '5'
#define FIFOTYPE '6'
#define CONTTYPE '7'

typedef struct posix_header
{
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

struct Options *getOpts(int argc, char *argv[]);
int fManip(struct Options opts);
void freeOpts (struct Options);

size_t getLength(const char *str);
