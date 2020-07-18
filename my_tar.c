// created 07.06.2020
// Return values:
// 1 - cant process args
// 2 - cant create (expand with even nums)
// 3 - cant extract (expand with odd)
#include "my_tar.h"
int main(int argc, char *argv[]){
    int rv = 0; // return value
    if(argc < 2)
    {
        printf("No arguments specified. Abandoning\n");
        return 1;
    }
    struct Options *opts = getOpts(argc, argv); // get the options, set the fd (file descr) of the archive
    rv += opts->error;
    if(rv > 0)
    {
        printf("Error processing the arguments. Abandoning\n");
        return 1;
    }

    rv += fManip(opts[0]);

    //freeOpts(opts);
    free(opts->files);
    free(opts);

    return rv;
}
