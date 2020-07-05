#include "my_tar.h"

int create(int fd_arch, char** files, int fcount) // fill tar file with contents
{
	int chk = 0;
	headerInfo *header;
	for(int fileNum = 0; fileNum != fcount; fileNum ++)
	{
		header = getHeaderInfo( files[ fileNum ] );
		if( header != NULL ) {
			write(fd_arch, header, 512);
			writeFileContent( fd_arch, files[ fileNum ] );
			free( header );
		} else {
			chk ++;
		}
	}

	fillMissing( fd_arch );
	return chk;
}

