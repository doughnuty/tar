#include "my_tar.h"

struct headerInfo {

};
int getContents(char* filename, int fd); // Write file contents of size 512 either to a new or a tar file, fd of which is passed

char* getHeader(struct headerInfo hi); // Create the file header based on the information provided

struct headerInfo reverseHeader(int archfd); // Get the file info from its header

int printFiles(char** filenames); // Print the filenames if -t provided

int fillMissing(int fd_arch); // Adds bzero to archive

int error(int errnum); // Print error message depending on the error case

int create(int fd_arch, char** files) // fill tar file with contents
{
    int chk = 0;
    while(files)
    {
        char* header = getHeader(files[i]);
        write(fd_arch, header, 512);
        chk += getContents(files[i], fd_arch);
        if(chk > 0) break;
    }
    chk += fillMissing(fd_arch);
}

int extract(int fd_arch, char** files) // extract the contents to the new files
{
    int chk = 0;
    while(tar)
    {
        struct headerInfo header = reverseHeader(fd_arch);
        if (header.type == REGULAR)
            chk += createReg(header);
        else if (header.type == DIRECTORY)
            Cchk += createDir(header);
        else return 1;
    }
}

int list(int fd_arch, char** files); // list the filenames
