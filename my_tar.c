// created 07.06.2020
// Return values:
// 1 - cant process args
// 2 - cant create (expand with even nums)
// 3 - cant extract (expand with odd)
#include "my_tar.h"
int main(int argc, char *argv[]){
    int rv = 0; // return value
    struct Options opts = getOpts(argc, argv); // get the options, set the fd (file descr) of the atchive
    if(rv > 0)
    {
        printf("Error processing the arguments.\n");
        return 1;
    }

    if (opts.create)
    {
        rv += create(opts.archfd, opts.files); // Create tar based on its file descriptor and fill with the contents
    }

    else if (opts.extract)
    {
        rv += extract(opts.archfd, opts.files); // Create files based on their headers
    }

    else if (opts.list)
    {
        rv += list(opts.archfd, opts.files); // List tar contents
    }

    freeOpts(opts);

    return rv;
}