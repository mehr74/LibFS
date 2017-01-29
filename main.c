#include <stdio.h>

#include "LibFS.h"


int testA();

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

    FS_Boot(argv[1]);
    testA();
    FS_Sync();
    return 0;
}

int testA()
{
    return 0;
}



