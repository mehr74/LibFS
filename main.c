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
    int i = 0;
    char buffer[256];
    for(i = 0; i < 256; i++)
    {
        sprintf(buffer, "tst%d.txt", i);
        File_Create(buffer);
        File_Open(buffer);
    }

    FS_Sync();
    return 0;
}

