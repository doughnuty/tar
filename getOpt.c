#include "my_tar.h"

void printOpts(struct Options opts); // Testing function to check if Options were perceived correctly

void listContents(char** files); // Called if -t parameter passed

bool checkOpts(struct Options opts, int err); // Checks whether options were appropriately written

int createTar(char* tarname); // Creates/opens file based on the name passed by -f and returns its file descriptor

char** appendName(char* initial, char* appendable); // Add to the name (path or file)

int error(int errnum); // Print the error message

struct Options getOpts(int argc, char *argv[]); // Parse options, opens tar file if exists and writes file names
// if -f correctly specified, address createTar to get archfd
// otherwise print error message
// if -C is given with c, r, or u, append to all the following files path name
// if with t or x, change the archname in struct Options
// if error occured return nonzero integer

void freeOpts (struct Options); // deletes memory allocated