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
    Dir_Create("/alireza/gholi");
    Dir_Create("/alireza/reza");
    File_Create("/alireza/reza/test.txt");
    File_Create("/alireza/test.txt");
    char *buffer = NULL;
    Dir_Read("/alireza", buffer,  Dir_Size("/alireza"));
    FS_Sync();
    return 0;
}

