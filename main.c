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
    Dir_Create("/alireza/lotfi/sholi/test/test");
    Dir_Create("/alireza/lotfi/sholi/test2");
        Dir_Create("/alireza/lotfi/sholi/test3");
            Dir_Create("/alireza/lotfi/sholi/test4");
                Dir_Create("/alireza/lotfi/sholi/test5");
                    Dir_Create("/alireza/lotfi/sholi/test6");
    Dir_Create("/alireza/lotfi/sholi/test/test2");
    Dir_Size("/alireza/lotfi/sholi/test/test2");
    Dir_Size("/alireza/lotfi/sholi");
    FS_Sync();
    return 0;
}

