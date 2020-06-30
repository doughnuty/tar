#include "my_tar.h"

static void print_options(struct Options *options ) // Testing function to check if Options were perceived correctly
{
    printf( "============TAR_OPTIONS============\n" );

    switch (options->mod)
    {
        case 'c':
            printf("Creation node: true\n");
            break;
        case 'x':
            printf("Extraction node: true\n");
            break;
        case 't':
            printf("List node: true\n");
            break;
        case 'r':
            printf("Append node: true\n");
            break;
        case 'u':
            printf("Append new node: true\n");
            break;
    }

    printf( "Archive name: %s\n", options -> archname);
    printf("Path changed to %s\n", options -> path);

    char **files = options -> files;
    int count = options->fcount;
    int i = 0;
    if (count == 0) printf("No files to archive\n");
    while(count != 0)
    {
        printf( "Work on file %s\n", files[i]);
        i ++ ;
        count--;
    }
    printf( "===================================\n" );
}

size_t getLength(const char *str){
    size_t length = 0;
    while (*str++) length++;

    return length;
}

struct Options *clear(void)
{
    struct Options *opts = malloc(sizeof(struct Options));
    opts -> mod = 0;
    opts -> path = NULL;
    opts -> archname = NULL;
    opts -> archfd = 0;
    opts -> files = malloc(sizeof(char**));
    opts -> fcount = 0;
    opts -> error = 0;
    return opts;
}

//void listContents(char** files); // Called if -t parameter passed

//bool checkOpts(struct Options opts, int err); // Checks whether options were appropriately written

int checkName (char* name)
{
    struct stat buf;
    int st = stat(name, &buf);
    if(S_ISDIR(buf.st_mode) && st == 0)
        return 0;
    else if(st != 0)
        return 2;
    else return 1;
}
char* appendName(char* initial, char* appendable) // Add to the name (path or file)
{
    int inLength = getLength(initial);
    int appndLength = getLength(appendable);
    char* result = malloc((inLength+appndLength+1)*sizeof(char));
    int i = 0, j = 0;
    for(i = 0; i < inLength; i++)
    {
        result[i] = initial[i];
    }
    result[i] = '/';
    for(i = inLength + 1; i < inLength + appndLength + 1; i++)
    {
        result[i] = appendable[j];
        j++;
    }
    return result;
}

int error(int errnum) // Print the error message
{
    switch (errnum)
    {
        case 1:
            printf("Please specify archive name after passing f parameter\n");
            break;
        case 2:
            printf("Cannot stat: no such file or directory\n");
            break;
        case 3:
            printf("Error 3");
            break;
        default:
            printf("Error default");
            break;
    }
    return 1;
}
int getArchfd(char* archname, bool is_create)
{
    int fd_archive;
    int flags;
    const mode_t DEFAULT_CREAT_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    if(is_create)
        flags = O_CREAT | O_WRONLY | O_TRUNC;
    else
        flags = O_WRONLY;

    fd_archive = open(archname, flags, DEFAULT_CREAT_MODE);

    return fd_archive;
}
struct Options getOpts(int argc, char *argv[]) /* Parse options, opens tar file if exists and writes file names
                                                // if -f correctly specified, address createTar to get archfd
                                                // otherwise print error message
                                                // if -C is given with c, r, or u, append to all the following files path name
                                                // if with t or x, change the archname in struct Options
                                                // if error occured return nonzero integer
                                                */
{
    char *file;
    struct Options *opts = clear();
    int num = 0;
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            int length = getLength(argv[i]);
            for(int j = 1; j < length; j++){
                    if (argv[i][j] == 'r' || argv[i][j] == 'c' || argv[i][j] == 't' || argv[i][j] == 'u' || argv[i][j] == 'x')
                        {
                        if(opts->mod != 0) {
                            printf("Please specify only one of the following: crtux\n");
                            opts -> error += 1;
                            return opts[0];
                        }
                        opts -> mod = argv[i][j];
                        }

                    else if (argv[i][j] == 'C')
                        {
                        if(j != length - 1)
                        {
                            error(1);
                            opts -> error += 1;
                            return opts[0];
                        }
                        if(checkName(argv[i+1]) == 0)
                            opts->path = argv[i+1];
                        i++;
                        }

                    else if (argv[i][j] == 'f')
                        {
                        if(j != length - 1)
                        {
                            error(1);
                            opts -> error += 1;
                            return opts[0];
                        }
                        //if(checkName(argv[i+1]) == 1)
                        opts->archname = argv[i+1];
                        i++;
                        }
            }
        }
        else
        {
            file = argv[i];
            if(opts->path != NULL)
                file = appendName(opts->path, file);
            if (checkName(file) < 2)
            {
                opts->files[num] = file;
                opts->fcount++;
                num++;
            }
            else
            {
                printf("%s: ", file);
                error(2);
                opts -> error += 1;
                return opts[0];
            }
        }
    }
    print_options(opts);
    if (opts -> archname == NULL)
    {
        printf("tar: Refusing to read archive contents from terminal (missing -f option?)\n");
        opts -> error += 1;
    }
    else opts->archfd = getArchfd(opts->archname, opts->mod == 'c');
    return opts[0];
}

//void freeOpts (struct Options); // deletes memory allocated
