#include <stdio.h>

#include "LibFS.h"

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
    Dir_Create("/a/b/c/d/e/f/g/h/i/j/k/l/m/n/o/p");
    FS_Sync();
    return 0;
}

