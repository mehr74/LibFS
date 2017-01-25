#include "parameters.h"
#include "LibDisk.h"
#include "LibFS.h"
#include "inode.h"
#include <stdio.h>
#include <stdlib.h>

// return the # of sector which is available
// return -1 means there is an error
// return -2 means there is no available inode
int FindNextAvailableInodeBlock()
{
    // define char* to read inode bitmap sector
    char* inodeBitmapBuffer= calloc(sizeof(char),SECTOR_SIZE);

    // check whether memory is allocated or not ...
    if(inodeBitmapBuffer == NULL)
    {
        // Can't allocated memory for superBlock ...
        printf("Faild to allocate memory for readBuffer\n");
        return -1;
    }

    //define variables
    int k,i,j;
    char bytemapTemp[8];

    //check next free inode
    //first handle errors
    for(k = 0; k < INODE_BITMAP_BLOCK_NUM; k++)
    {
        if( Disk_Read(INODE_FIRST_BITMAP_BLOCK_INDEX + k, inodeBitmapBuffer) == -1)
        {
            printf("Disk failed to read inode bitmap block\n");
            free(inodeBitmapBuffer);
            return -1;
        }
    
        // checking available inodes
        for( i=0; i< min((FILE_NUM_MAX/8)-(k*SECTOR_SIZE), SECTOR_SIZE) ; i++)
        {
            ConvertBitmapToBytemap(&inodeBitmapBuffer[i],bytemapTemp);
            for(j=0;j<8;j++)
            {
                if(!bytemapTemp[j])
                {
                    free(inodeBitmapBuffer);
                    return i*8+j;
                }
            }
        }
    }
    
    free(inodeBitmapBuffer);

    // there is no available inode
    return -2;
}

int FindNextAvailableDataBlock()
{
    // define char* to read inode bitmap sector
    char* dataBitmapBuffer= calloc(sizeof(char),SECTOR_SIZE);

    // check whether memory is allocated or not ...
    if(dataBitmapBuffer == NULL)
    {
        // Can't allocated memory for superBlock ...
        printf("Faild to allocate memory for readBuffer\n");
        return -1;
    }

    //define variables
    int k,i,j;
    char bytemapTemp[8];

    //check next free inode
    //first handle errors
    for(k = 0; k < DATA_BITMAP_BLOCK_NUM; k++)
    {
        if( Disk_Read(DATA_FIRST_BLOCK_INDEX + k, dataBitmapBuffer) == -1)
        {
            printf("Disk failed to read inode bitmap block\n");
            free(dataBitmapBuffer);
            return -1;
        }

        // checking available datablock
        for( i=0; i< min((DATA_BLOCK_NUM/8)-(k*SECTOR_SIZE), SECTOR_SIZE) ; i++)
        {
            ConvertBitmapToBytemap(&dataBitmapBuffer[i],bytemapTemp);
            for(j=0;j<8;j++)
            {
                if(!bytemapTemp[j])
                {
                    free(dataBitmapBuffer);
                    return i*8+j;
                }
            }
        }
    }

    free(dataBitmapBuffer);

    // there is no available datablock
    return -2;
}

int ChangeInodeBitmapStatus(int inodeIndex, int status)
{
    // define char* to read inode bitmap sector
    char* inodeBitmapBuffer= calloc(sizeof(char),SECTOR_SIZE);
    char bytemapTemp[8];

    // check whether memory is allocated or not ...
    if(inodeBitmapBuffer == NULL)
    {
        // Can't allocated memory for superBlock ...
        printf("Faild to allocate memory for inodeBitmapBuffer\n");
        return -1;
    }

    // Read appropriated bitmap block...
    if( Disk_Read(INODE_FIRST_BITMAP_BLOCK_INDEX + (inodeIndex/SECTOR_SIZE), inodeBitmapBuffer) == -1)
    {
        printf("Disk failed to read inode bitmap block\n");
        free(inodeBitmapBuffer);
        return -1;
    }

    // update bitmap block - set inode #inodeIndex to status state...
    ConvertBitmapToBytemap(&inodeBitmapBuffer[(inodeIndex%SECTOR_SIZE)/8], bytemapTemp);
    bytemapTemp[(inodeIndex%SECTOR_SIZE) % 8] = status;
    ConvertBytemapToBitmap(&inodeBitmapBuffer[(inodeIndex%SECTOR_SIZE)/8], bytemapTemp);

    // Write changes to disk ...
    if( Disk_Write(INODE_FIRST_BITMAP_BLOCK_INDEX + (inodeIndex/(SECTOR_SIZE*8)), inodeBitmapBuffer) == -1)
    {
        printf("Disk failed to write inode bitmap block\n");
        free(inodeBitmapBuffer);
        return -1;
    }

    free(inodeBitmapBuffer);
    return 0;
}

int ConvertBitmapToBytemap(char* bitmap,char* bytemap)
{
    bytemap[0] = (*bitmap & BIT_0) ? 1:0;
    bytemap[1] = (*bitmap & BIT_1) ? 1:0;
    bytemap[2] = (*bitmap & BIT_2) ? 1:0;
    bytemap[3] = (*bitmap & BIT_3) ? 1:0;
    bytemap[4] = (*bitmap & BIT_4) ? 1:0;
    bytemap[5] = (*bitmap & BIT_5) ? 1:0;
    bytemap[6] = (*bitmap & BIT_6) ? 1:0;
    bytemap[7] = (*bitmap & BIT_7) ? 1:0;
    return 0;
}

int ConvertBytemapToBitmap(char* bitmap, char* bytemap)
{
    *bitmap = 0x00;
    *bitmap = (bytemap[0]) ? (*bitmap|BIT_0) : (*bitmap);
    *bitmap = (bytemap[1]) ? (*bitmap|BIT_1) : (*bitmap);
    *bitmap = (bytemap[2]) ? (*bitmap|BIT_2) : (*bitmap);
    *bitmap = (bytemap[3]) ? (*bitmap|BIT_3) : (*bitmap);
    *bitmap = (bytemap[4]) ? (*bitmap|BIT_4) : (*bitmap);
    *bitmap = (bytemap[5]) ? (*bitmap|BIT_5) : (*bitmap);
    *bitmap = (bytemap[6]) ? (*bitmap|BIT_6) : (*bitmap);
    *bitmap = (bytemap[7]) ? (*bitmap|BIT_7) : (*bitmap);
    return 0;
}

// Place an inode in correct block
// inodeData must be 128 Byte
int WriteInodeInSector ( int inodeNumber , char* inodeData){
    
    // define char* to read appropriate sector
    char* sectorBuffer= calloc(sizeof(char),SECTOR_SIZE);
    
    // check whether memory is allocated or not ...
    if(sectorBuffer == NULL)
    {
        // Can't allocated memory for superBlock ...
        printf("Faild to allocate memory for sectorBuffer\n");
        return -1;
    }
    
    // define variables
    int sectorNumber=inodeNumber/INODE_PER_BLOCK_NUM;
    int inodeOfSectorIndex=inodeNumber%INODE_PER_BLOCK_NUM;
    
    // Read the sector and check the errors
    if( Disk_Read(INODE_FIRST_BLOCK_INDEX + sectorNumber, sectorBuffer) == -1)
    {
        printf("Disk failed to read block of appropriate inode\n");
        free(sectorBuffer);
        return -1;
    }
    
    // Change the data of inoded
    memcpy((void*)inodeData,(void*)sectorBuffer+INODE_SIZE*inodeOfSectorIndex,INODE_SIZE);
    
    // Write changes to disk ...
    if( Disk_Write(INODE_FIRST_BLOCK_INDEX + sectorNumber, sectorBuffer) == -1)
    {
        printf("Disk failed to write changed block\n");
        free(sectorBuffer);
        return -1;
    }
    
    free(sectorBuffer);
    return 0;
}





