#include "parameters.h"
#include "LibDisk.h"
#include "LibFS.h"
#include "inode.h"
#include "directory.h"

int BuildRootDirectory()
{
    // allocate memory size of inode
    char *rootInode;
    rootInode= calloc(sizeof(char), INODE_SIZE);

    printf("%d", FindNextAvailableInode());

    // check whether memory is allocated or not ...
    if(rootInode == NULL)
    {
        // Can't allocated memory for rootInode
        printf("Failed to allocate memory for root-inode\n");
        free(rootInode);
        return -1;
    }

    free(rootInode);
    return 0;
}
