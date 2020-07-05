#include "my_tar.h"

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

/* референс, пока пишу код для хэдера
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
} headerInfo; */

// ux_tar: my_tar.c0000644000175000017500000000162013677560726014717 0ustar  jenny-johnsonjenny-johnson
// my_tar: my_tar.c01006440001750000175000000001620136775607260
// не могу понять откуда 14717 и ustar и нужны ли они вообще?

headerInfo *getHeaderInfo(char *filename) {
	struct stat info;
	if(stat( filename, &info ) < 0 ) {
		printf( "Error opening file.\n" );
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

char* getHeader(headerInfo hi); // Create the file header based on the information provided

struct headerInfo reverseHeader(int archfd); // Get the file info from its header

int printFileNames(char** filenames, int fcount) // Print the filenames if -t provided
{
	for( int i = 0; i < fcount; i ++ )
		printf( "%s\n", filenames[ i ] );
};

void fillMissing(int fd_arch) // Adds bzero to archive
{
	char end[ 1024 ];
	bzero( end, 1024 );
	write( fd_arch, end, 1024 );
}

int error(int errnum); // Print error message depending on the error case

// int list(int fd_arch, char** files); // list the filenames
