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

    int i = FindNextAvailableInodeBlock();
    ChangeInodeBitmapStatus(i, OCCUPIED);
    i = FindNextAvailableInodeBlock();
    printf("%d", i);
    ChangeInodeBitmapStatus(0, AVAILIBLE);
    printf("%d", FindNextAvailableInodeBlock());


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
