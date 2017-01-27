#include <stdio.h>

#include "LibFS.h"
#include "directory.h"

void usage(char *prog)
{
    fprintf(stderr, "usage: %s <disk image file>\n", prog);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
    }

    char *path = argv[1];

    printf("Starting filesystem\n");
    FS_Boot(path);
    Dir_Create("/alireza");
    FS_Sync();
    
    int output=-10;
    int error=-10;
    error=searchPathInInode(0,"alireza",&output);
    printf("error=%d\noutput=%d\n",error,output);
    
    return 0;
}

