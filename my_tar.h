// Created 08.06.2020
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

struct Options
{
    bool create;
    bool extract;
    bool list;
    int archfd;
    char* archname;
    char** files;
};

struct Options getOpts(int argc, char *argv[]);
void freeOpts (struct Options);
int create(int fd_arch, char** files);
int extract(int fd_arch, char** files);
int list(int fd_arch, char** files);