#include "builder.h"
#include "LibFS.h"
#include "parameters.h"
#include "LibDisk.h"
#include "directory.h"
#include <stdio.h>
#include <string.h>

int CheckFileSystemSuperBlock()
{
    // allocate memory size of sector
    char *readBuffer;
    readBuffer = calloc(sizeof(char), SECTOR_SIZE);

    // check whether memory is allocated or not ...
    if(readBuffer == NULL)
    {
        // Can't allocated memory for superBlock ...
        printf("Faild to allocate memory for readBuffer\n");
        return -1;
    }

    // check wether disk could read the sector ...
    if(Disk_Read(SUPER_BLOCK_INDEX, readBuffer)==-1)
    {
        printf("Disk faild to read superblock");
        return -1;
    }


    // Check magic number
    if( readBuffer[0]==MAGIC_NUMBER_0 &&
        readBuffer[1]==MAGIC_NUMBER_1 &&
        readBuffer[2]==MAGIC_NUMBER_2 &&
        readBuffer[3]==MAGIC_NUMBER_3)
    {
        printf("File opend successfully.");
    }
    else
    {
        printf("The existing file has an unsupported filesystem\n");
    }
    return 0;
}

int BuildMetadataBlocks()
{
    if (BuildSuperBlock() == -1)
    {
        osErrno = E_GENERAL;
        return -1;
    }

    if (BuildBitmapBlocks() == -1)
    {
        osErrno = E_GENERAL;
        return -1;
    }

    if (BuildRootDirectory() == -1)
    {
        osErrno = E_GENERAL;
        return -1;
    }
    return 0;
}

int BuildSuperBlock()
{

    // allocate memory size of sector
    char* superBlock;
    superBlock = calloc(sizeof(char), SECTOR_SIZE); 

    // check whether memory is allocated or not ...
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

    // Check whether disk wrote superBlock...
    if(diskErrno == E_MEM_OP)
    {
        // Disk couldn't write superBlock
        printf("Disk Failed to write superBlock\n");
        return -1;
    }

    return 0;
}

int BuildBitmapBlocks()
{
    // Building inode bitmap blocks

    // allocated memory size of sector
    int i;
    char* inodeBitmapBlock;
    inodeBitmapBlock = calloc(sizeof(char), SECTOR_SIZE);

    for(i = 0; i < INODE_BITMAP_BLOCK_NUM; i++)
    {
        // set all inode blocks to be available
        memset(inodeBitmapBlock, AVAILIBLE, SECTOR_SIZE);

        Disk_Write(INODE_FIRST_BLOCK_INDEX + i, inodeBitmapBlock);

        // Check whether disk wrote inodeBitmapBlock...
        if(diskErrno == E_MEM_OP)
        {
            // Disk couldn't write inode bitmap block
            printf("Disk Failed to write inodeBitmapBlock\n");
            free(inodeBitmapBlock);
            return -1;
        }
    }
    free(inodeBitmapBlock);


    // Building data bitmap blocks

    // allocate memory size of sector
    char* dataBitmapBlock;
    dataBitmapBlock = calloc(sizeof(char), SECTOR_SIZE);

    for(i = 0; i < DATA_BITMAP_BLOCK_NUM; i++)
    {
        // set add data blocks to be available
        memset(dataBitmapBlock, AVAILIBLE, SECTOR_SIZE);

        Disk_Write(DATA_FIRST_BLOCK_INDEX + i, dataBitmapBlock);

        // Check whether disk wrote data bitmap block...
        if(diskErrno == E_MEM_OP)
        {
            // Disk couldn't write data bitmap block
            printf("Disk Failed to write dataBitmapBlock\n");
            free(dataBitmapBlock);
            return -1;
        }
    }
    free(dataBitmapBlock);

    return 0;
}

int BuildDataBlock(char* dataBlock)
{

    return 0;
}

int BuildInode(char* inode, char type)
{
    // allocate memory size of sector
    inode = calloc(sizeof(char), SECTOR_SIZE);

    // check whether memory is allocated or not ...
    if(inode == NULL)
    {
        // Can't allocated memory for inode ...
        printf("Faild to allocate memory for inode\n");
        return -1;
    }

    inode[0] = type;

    return 0;
}
