#include "my_tar.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int create(int fd_arch, char** files, int fcount);
char *my_itoa( char *res, unsigned int number, int size, int base )
{
    const char symbols[] = "0123456789ABCDEF";
    memset( res, '0', size - 1 );
    res[ size - 1 ] = '\0';
    size --;
    while( number )
    {
        res[ -- size ] = symbols[ number % base ];
        number /= base;
    }
    return res;
}

long my_atol( const char *str, int size )
{
    long res = 0;
    int i = 0;
    while( str[ i ] && i < size && str[ i ] >= '0' && str[ i ] <= '9' )
    {
        res = res * 10 + ( str[ i ] - '0' );
        i ++;
    }
    return res;
}

int my_iterative_pow( int a, int b )
{
    int res = 1;
    for( int i = 0; i != b; i ++ )
        res *= a;
    return res;
}

long oct_to_dec( long oct )
{
    int dec = 0;
    int i = 0;

    while( oct != 0 )
    {
        dec += ( oct % 10 ) * my_iterative_pow( 8, i );
        i ++;
        oct /= 10;
    }
    return dec;
}

long dec_to_oct( long dec )
{
    int oct = 0;
    int i = 0;

    while( dec != 0 )
    {
        oct += ( dec % 8 ) * my_iterative_pow( 10, i );
        i ++;
        dec /= 8;
    }
    return oct;
}

int reverseConvert(char* a, int base)
{
    int temp, eight = 1;
    int result = 0;
    int converted = 0;
    int size = getLength(a);
    int numlen = size;
    int isStart = 0;
    for(int i = 0; i < size; i++)
    {
        result = result*10 + a[i] - '0';
        if(a[i]=='0' && isStart == 0)
            numlen--;
        else isStart++;
    }

    while(result!=0)
    {
        temp = result % 10;
        converted = converted + (temp * eight);
        eight = eight * base;
        result = result/10;
    }

    return converted;
}

int cmpName(char* name1, char* name2) {
    int a = getLength(name1), b = getLength(name2);
    for(int i = 0; i < a; i++){
        if(name1[i] > name2[i]) return 1;
        else if(name2[i] > name1[i]) return 2;
    }
    if(a > b) return 1;
    else if(a < b) return 2;
    
    return 0;
}

char* setName(const char* name, const char* path){
    int pathLen = getLength(path), nameLen = getLength(name);
    char* ans = (char*)malloc(sizeof(char)*(pathLen+nameLen+2));
    
    for(int i = 0; i < pathLen; i++){
        ans[i] = path[i];
    }
    
    ans[pathLen] = '/';

    for(int j = 1; j <= nameLen; j++){
        ans[pathLen+j] = name[j-1];
    }
    ans[pathLen+nameLen+1] = '\0';
    
    return ans;
}

char getTypeflag( int mode ) {
    switch( mode & S_IFMT ) {
        case S_IFREG:
            return '0';
        case S_IFLNK:
            return '2';
        case S_IFCHR:
            return '3';
        case S_IFBLK:
            return '4';
        case S_IFDIR:
            return '5';
        case S_IFIFO:
            return '6';
        default:
            return '0';
    }
}

headerInfo *getHeaderInfo(char *filename) {
    struct stat info;
    if(stat( filename, &info ) < 0 ) {
        printf( "Error opening file %s.\n", filename );
        return NULL;
    }

    headerInfo *header = malloc( 512 );
    if( header != NULL )
        bzero( header, 512 );

    strncpy( header -> name, filename, 100 );
    my_itoa( header -> mode, info.st_mode, 8, 8 );
    my_itoa( header -> uid, info.st_uid, 8, 8 );
    my_itoa( header -> gid, info.st_gid, 8, 8 );
    my_itoa( header -> size, info.st_size, 12, 8 );
    my_itoa( header -> mtime, info.st_mtime, 12, 8 );
    header -> typeflag = getTypeflag( info.st_mode );



    return header;
}

headerInfo *reverseHeader(int archfd, int place) // Get the file info from its header
{
    headerInfo *header = malloc(512);
    lseek(archfd, place, SEEK_SET);
    // int red = 
    read(archfd, header, 512);
    //printf("Red %d\n", red);
    return header;
}

int getFcount(char* namedir)
{
    int fcount = 0;
    DIR* directory;
    struct dirent *ptr;
    directory = opendir(namedir);
    if(directory==NULL)
        return -1;
    while((ptr = readdir(directory))!=NULL) 
        if(ptr->d_name[0] != '.')
            fcount++;
    closedir(directory);

    return fcount;
}

char** getFiles(char* namedir, int fcount, DIR* directory)
{
    char** files = malloc(fcount*100);
    int i = 0;
    struct dirent *ptr;
    if(directory==NULL)
        return NULL;
    while((ptr = readdir(directory))!=NULL && i!= fcount) 
        if(ptr->d_name[0] != '.')
            files[i++] = setName(ptr->d_name, namedir);
    return files;
}

long int getSize(char* archname)
{
    struct stat st;
    if(stat(archname, &st)==0)
        return st.st_size;
    else return -1;
}

long int getBlocksSize(int origin)
{
    int blocked = origin/512;
    if(origin%512)
        blocked += 1;
    return blocked*512;
}

void writeFileContent( int fd_archive, char *filename ) // Write file contents of size 512 either to a new or a tar file, fd of which is passed
{
    int fd_file = open( filename, O_RDONLY );
    int cc = -1;
    char buf[ 512 ];

    while( cc != 0 ) {
        cc = read( fd_file, buf, 512 );
        write( fd_archive, buf, cc );
    }

    close( fd_file );
}

void fillMissing(int fd_arch) // Adds bzero to archive
{
    char end[ 1024 ];
    bzero( end, 1024 );
    write( fd_arch, end, 1024 );
}

int createReg(headerInfo *header, int archfd)
{
    int size = reverseConvert(header->size, 8);
    const mode_t DEFAULT_CREAT_MODE = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int file_fd = open(header->name, O_CREAT | O_WRONLY | O_TRUNC, DEFAULT_CREAT_MODE);
    char *buf = malloc(size);
    read(archfd, buf, size);
    write(file_fd, buf, size);
    free(buf);
    return file_fd;
}

int createDir(headerInfo *header)
{
    return mkdir(header->name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);   
}

int recursiveTar(char* DirName, int fd_arch)
{
    int internFCOUNT = getFcount(DirName);
    DIR* directory = opendir(DirName);
    char** internFILES = getFiles(DirName, internFCOUNT, directory);
    if(internFCOUNT == -1 || internFILES == NULL)
        return -1;
    create(fd_arch, internFILES, internFCOUNT);
    for(int i = 0; i < internFCOUNT; i++)
    {
        free(internFILES[i]);
    }
    free(internFILES);
    closedir(directory);
    return 0;
}
int error(int errnum); // Print error message depending on the error case
