#include "parameters.h"
#include "LibDisk.h"
#include "LibFS.h"
#include "inode.h"
#include "directory.h"
#include "builder.h"
#include <string.h>

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


int BreakPathName( char* pathName , char* arrayOfBreakPathName[])
{
    // check path length
    if(strlen(pathName)>FULL_PATH_LENGTH_MAX)
    {
        printf("Full Path name is more than legal character\n");
        return -1;
    }
    
    // define variable
    int index=0;
    int i=0;
    const char* search="/";
    char* token;
    
    char strTemp[strlen(pathName)];
    strcpy(strTemp,pathName);
    
    // divide the strTemp(pathName) into directories
    token=strtok(strTemp,search);
    while(token!=NULL){
        arrayOfBreakPathName[index]=token;
        token = strtok(NULL, search);
    }
    
    // check value of index. it must be at most 16
    for (i=0;i<index;i++)
    {
        if(strlen(arrayOfBreakPathName[i])>PATH_LENGTH_MAX)
        {
            printf("Some Paths length are more than legal\n");
            return -1;
        }
    }
    
    return index;
    
}


//retrun -1 if error
//return -2 if inode is not directory
int searchPathInInode ( int inodeNumber , char* search , int* outputInodeNumber)
{
    char* sectorOfInodeBuffer=calloc(sizeof(char),SECTOR_SIZE)
    char* inodeBuffer=calloc(sizeof(char),INODE_SIZE);
    char* sectorBuffer=calloc(sizeof(char),SECTOR_SIZE);
    char
    
    DirectoryEntry directoryEntryTemp;
    int
    int sectorOfInodeNumber=inodeNumber/INODE_PER_BLOCK_NUM;
    int inodeIndexInSector=inodeNumber%INODE_PER_BLOCK_NUM;
    int i,j;
    
    // Read the inode
    if( Disk_Read(INODE_FIRST_BLOCK_INDEX + sectorOfInodeNumber, sectorOfInodeBuffer) == -1)
    {
        printf("Disk failed to read inode block\n");
        free(sectorOfInodeBuffer);
        free(inodeBuffer);
        free(sectorBuffer);
        return -1;
    }
    
    //Copy appropriate inode from sector into inodeBuffer
    memcpy((void*)sectorOfInodeBuffer+INODE_SIZE*inodeIndexInSector,(void*)inodeBuffer,INODE_SIZE);
    
    // Check that inode is Directory  FILE_ID=0x80 , DIRECORY_ID=0x00
    if (inodeBuffer[0] & FILE_ID)
    {
        printf("Inode is not directory, it is file.\n");
        free(sectorOfInodeBuffer);
        free(inodeBuffer);
        free(sectorBuffer);
        return -2;
    }
    
    for (i=0;i<SECTOR_PER_FILE_MAX;i++)
    {
        
    }

    





}