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
    Dir_Create("/alireza/lotfi");
    Dir_Create("/alireza/lotfi/sholi");
    Dir_Create("/alireza/lotfi/sholi/test");
    FS_Sync();
    
    int output=0;
    int error=0;
    error=searchPathInInode(0,"ghol",&output);
    printf("error=%d\noutput=%d\n",error,output);
    
    return 0;
}

