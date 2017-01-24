#include "parameters.h"
#include "LibDisk.h"
#include "LibFS.h"
#include "directory.h"

int BuildRootDirectory()
{
    // allocate memory size of inode
    char *rootInode;
    rootInode= calloc(sizeof(char), INODE_SIZE);

    // check whether memory is allocated or not ...
    if(rootInode == NULL)
    {
        // Can't allocated memory for rootInode
        printf("Failed to allocate memory for root-inode\n");
        return -1;
    }



    free(rootInode);
    return 0;
}
