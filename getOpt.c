#include "my_tar.h"

static void print_options(struct Options *options ) // Testing function to check if Options were perceived correctly
{
    printf( "============TAR_OPTIONS============\n" );
    printf( "Creation node: %s\n", options -> create ? "true" : "false" );
    printf( "Extraction node: %s\n", options -> extract ? "true" : "false" );
    printf( "List node: %s\n", options -> list ? "true" : "false" );
    printf( "Append node: %s\n", options -> append ? "true" : "false" );
    printf( "Append new node: %s\n", options -> append_new ? "true" : "false" );
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
    opts -> create = false;
    opts -> extract = false;
    opts -> list = false;
    opts -> append_new = false;
    opts -> append = false;
    opts -> argcount = 0;
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

int createTar(char* tarname) // Creates/opens file based on the name passed by -f and returns its file descriptor
{
    int fd = open(tarname, O_CREAT, O_RDWR);

    return fd;
}

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

//int setArg(char** argv, int i, struct Options *opts);
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
                switch(argv[i][j])
                {
                    case 'c':
                        opts->create = true;
                        opts->argcount += 1;
                        break;
                    case 'r':
                        opts->append = true;
                        opts->argcount += 1;
                        break;
                    case 't':
                        opts->list = true;
                        opts->argcount += 1;
                        break;
                    case 'u':
                        opts->append_new = true;
                        opts->argcount += 1;
                        break;
                    case 'x':
                        opts->extract = true;
                        opts->argcount += 1;
                        break;
                    case 'C':
                        if(j != length - 1)
                        {
                            error(1);
                            opts -> error += 1;
                            return opts[0];
                        }
                        if(checkName(argv[i+1]) == 0)
                            opts->path = argv[i+1];
                        i++;
                        break;
                    case 'f':
                        if(j != length - 1)
                        {
                            error(1);
                            opts -> error += 1;
                            return opts[0];
                        }
                        //if(checkName(argv[i+1]) == 1)
                            opts->archname = argv[i+1];
                        i++;
                        break;
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
    if (opts -> argcount > 1)
    {
        printf("Please specify only one of the following: crtux\n");
        opts -> error += 1;
    }
    return opts[0];
}

//void freeOpts (struct Options); // deletes memory allocated
