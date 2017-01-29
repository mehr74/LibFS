#include <stdio.h>
#include <string.h>
#include "LibFS.h"


int testA();
int testB();

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
    File_Create("/test.txt");
    File_Open("/test.txt");
  //  testB();
    FS_Sync();
    return 0;
}

// failed
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

// lots of files
int testB()
{
    printf("Testing File");
    char buffer[256];
    int i;
    for(i = 0; i < 256; i++)
    {
        sprintf(buffer, "/file%d.txt", i);
        File_Create(buffer);
    }
    return 0;
}



