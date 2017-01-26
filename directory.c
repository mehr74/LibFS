#include "parameters.h"
#include "LibDisk.h"
#include "LibFS.h"
#include "inode.h"
#include "directory.h"
#include "builder.h"

int BuildRootDirectory()
{
    // allocate memory size of inode
    char *rootInode;

    // allocate memory size of sector
    rootInode = calloc(sizeof(char), SECTOR_SIZE);
    BuildInode(rootInode, DIRECTORY_ID);

    // get one availabe inode block
    // since all inode are no availabe i must be 0
    int i = FindNextAvailableInodeBlock();
    int j;
    Disk_Save("disk1.txt");
    ChangeInodeBitmapStatus(i, OCCUPIED);
    Disk_Save("disk2.txt");

    for(i = 0; i < 513; i++)
    {
        j = FindNextAvailableDataBlock();
        ChangeDataBitmapStatus(j, OCCUPIED);
        printf("%d\n", j);
    }
    Disk_Save("disk3.txt");
    ChangeDataBitmapStatus(j, OCCUPIED);

    // Write root directory data in disk
    WriteInodeInSector(i, rootInode);

    free(rootInode);
    return 0;
}
