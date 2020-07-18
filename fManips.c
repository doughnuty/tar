#include <unistd.h>
#include "internalFuncts.c"

int displayheader(struct posix_header *header)
{
    printf("======= EXTRACTING THE FILE =======\n");
    printf("GETTING THE HEADER...\n");
    printf("Filename is %s\n", header->name);
    printf("%s\n", header->size);
    printf( "===================================\n" );

    return 0;
}
int append()
{
    int chk = 0;
    return chk;
}

int appendNew()
{
    int chk = 0;
    return chk;
}
int create(int fd_arch, char** files, int fcount) // fill tar file with contents
{
    int chk = 0;
    printf("fcount = %d", fcount);
    struct posix_header *header;
    for(int fileNum = 0; fileNum != fcount; fileNum ++)
    {
        header = getHeaderInfo( files[ fileNum ] );
        if( header != NULL ) {
            write(fd_arch, header, 512);
            if (header->typeflag != DIRTYPE)
                writeFileContent( fd_arch, files[ fileNum ] );
            else 
            {
                chk -= recursiveTar(header->name, fd_arch);
            }
            free( header );
        } else {
            chk ++;
            printf("tar: Error archiving %s\n", files[fileNum]);
        }
    }

    fillMissing( fd_arch );
    return chk;
}

int extract(int fd_arch, char *archname, int fcount, char** files) // extract the contents to the new files
{
    long int i = 0;
    long int size = 0;
    int chk = 0;
    int fchk = 0;
    //int size = 0;
    long int archsize = getSize(archname);
    printf("tar file size is %ld\n", archsize);
    while(i < archsize)
    {
        // if (i > 0)  
        //     i += 512 - (i % 512); 
        headerInfo *header = reverseHeader(fd_arch, i);
        i += 512;
        displayheader(header);
        if((fcount > 0 && cmpName(files[fchk], header->name) == 0) || fcount == 0)
        {
            if (header != NULL && getLength(header->name) > 0) 
            {
                if (header->typeflag != DIRTYPE)
                {
                    createReg(header, fd_arch);
                    long int size = getBlocksSize(reverseConvert(header->size, 8)); 
                    i += size;
                }
                else
                {
                    chk -= createDir(header);
                    printf("This is a dir with the name %s\n", header->name);
                }
                free(header);
            }
            else
            {
                chk++;
                free(header);
                return chk;
            }
        }

        printf("Stopped at %ld byte. Size is %ld\n", i, size);
        if (i < 0 && size < 0)
            return 2;
    }

    return chk;
}

int list(int archfd, int fcount, char** filenames) // list the filenames
{
    int chk = 0;
    int i = 0;
    char* filename = malloc(100);
    while(filename != NULL)
    {
        headerInfo *header = reverseHeader(archfd, i);
        filename = header->name;
        if (fcount == 0 || (fcount > 0 && cmpName(filenames[i], filename) == 0))
        {
            if (getLength(filename) <= 0)
                break;
            long int size = getBlocksSize(reverseConvert(header->size, 8)); 
            i += 512 + size;
            printf("%s\n", filename);
        }
    }
    free(filename);
    return chk;
}

int fManip(struct Options opts)
{
    switch (opts.mod)
    {
        case 'r':
            append();
            break;
        case 'u':
            appendNew();
            break;
        case 'c':
            create(opts.archfd, opts.files, opts.fcount);
            break;
        case 'x':
            extract(opts.archfd, opts.archname, opts.fcount, opts.files);
            break;
        case 't':
            list(opts.archfd, opts.fcount, opts.files);
    }
    return 0;
}
