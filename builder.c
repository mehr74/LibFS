#include "builder.h"
#include "LibFS.h"
#include "parameters.h"
#include "LibDisk.h"

int BuildSuperBlock()
{

    // allocate memory size of sector
    char* superBlock;
    superBlock = calloc(sizeof(char), SECTOR_SIZE); 

    // check wheather memory allocated or not ...
    if(superBlock == NULL)
    {
        // Can't allocated memory for superBlock ...
        printf("Faild to allocate memory for super-block\n");
        return -1;
    }

    // Memory for superBlock is allocated...
    // add the magic number to superBlock
    superBlock[0] = MAGIC_NUMBER_0;
    superBlock[1] = MAGIC_NUMBER_1;
    superBlock[2] = MAGIC_NUMBER_2;
    superBlock[3] = MAGIC_NUMBER_3;

    Disk_Write(SUPER_BLOCK_INDEX, superBlock); 
    free(superBlock);

    // Check weather disk wrote superBlock...
    if(diskErrno == E_MEM_OP)
    {
        // Disk couldn't write superBlock
        printf("Disk Failed to write superBlock\n");
        return -1;
    }

    return 0;
}

int BuildBitmapBlock(char* bitmapBlock)
{

    return 0;
}

int BuildDataBlock(char* dataBlock)
{

    return 0;
}

int BuildInode(char* inode)
{

    return 0;
}
