#include <stdio.h>
#include <string.h>
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
    printf("Testing File");
    File_Create("/main.c");
    File_Create("/lib.c");
    int fd = File_Open("/main.c");
    char buffer[24];
    strcpy(buffer, "This is a test string");
    File_Write(fd, buffer, 22);
    return 0;
}



