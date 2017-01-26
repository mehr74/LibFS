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