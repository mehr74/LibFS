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
    ChangeInodeBitmapStatus(i, OCCUPIED);

    // get one availabe data block
    int j = FindNextAvailableDataBlock();
    ChangeDataBitmapStatus(j, OCCUPIED);

    InitializeDirectoryInode(rootInode, j);

    char *dataBlock;
    dataBlock = calloc(sizeof(char), SECTOR_SIZE);


    InitializeDirectoryFirstDataBlock(dataBlock, i, i);

    if(Disk_Write(DATA_FIRST_BLOCK_INDEX + j, dataBlock) == -1)
    {
        // Disk couldn't write dataBlock
        printf("Disk Failed to write datablock\n");
        return -1;
    }

    // Write root directory inode in disk
    WriteInodeInSector(i, rootInode);

    free(rootInode);
    return 0;
}

int  BreakPathName(char *pathName, char **arrayOfBreakPathName)
{
    int index = 0;
    while (*pathName != '\0')
    {
        index++;
        // if not the end of pathName
        while (*pathName == '/')
            *pathName++ = '\0';
        if(*pathName)
            *arrayOfBreakPathName++ = pathName;          // save the argument position
        while (*pathName != '/' && *pathName != '\0')
            pathName++;             // skip the argument until
    }
    *arrayOfBreakPathName = '\0';                 // mark the end of argument list
    return index;
}

// return -2 if there is no space to store dirEntry
// return -1 if there is no space to store terminator
// return 0 if the file stored successfully
int addDirectoryEntryOnSector(char* dataBlock, DirectoryEntry *dirEntry)
{
    int i;
    for(i = 0; i < SECTOR_SIZE/DIRECTORY_LENGTH; i++)
    {
        if(dataBlock[i*DIRECTORY_LENGTH + 4] == '\0')
            break;
    }

    if(i * DIRECTORY_ID + 19 > SECTOR_SIZE)
        return -1;
    strcpy(&dataBlock[i*DIRECTORY_LENGTH + 4], dirEntry->pathName);
    dataBlock[i*DIRECTORY_LENGTH] = dirEntry->inodePointer;

    if((i+1)*DIRECTORY_LENGTH + 19 > SECTOR_SIZE)
    {
        return -2;
    }

    dataBlock[(i+1)*DIRECTORY_LENGTH + 4] = '\0';

    return 0;
}

int addDirectoryEntry(int inodeNum, DirectoryEntry *dirEntry)
{
    int i = 0;

    // allocate memory of a sector block for datablock
    char * dataBlock = calloc(sizeof(char), SECTOR_SIZE);

    // allocate memory of a sector block for inode block
    char * inodeBlock = calloc(sizeof(char), SECTOR_SIZE/INODE_PER_BLOCK_NUM);


    // Try to read inode block from disk....
    if ( ReadInode(inodeNum, inodeBlock) == -1)
    {
        printf("Faild to read inode block %d from disk\n", inodeNum);
        free(dataBlock);
        free(inodeBlock);
        return -1;
    }

    for( i = 0; i < SECTOR_PER_FILE_MAX; i++)
    {
        // Try to append dirEntry to data part...
        int dataBlockIndex = inodeBlock[8+i*4];
        if(Disk_Read(dataBlockIndex + DATA_FIRST_BLOCK_INDEX , dataBlock) == -1)
        {
            printf("Faild to read data block %d from disk\n", dataBlockIndex);
            free(dataBlock);
            free(inodeBlock);
            return -1;
        }

        int res = addDirectoryEntryOnSector(dataBlock, dirEntry);


        if(res == 0)        // dirEntry entered successfully
        {
            Disk_Write(dataBlockIndex + DATA_FIRST_BLOCK_INDEX, dataBlock);
            free(dataBlock);
            free(inodeBlock);
            return 0;
        }
        else if(res == -1)  // There is no space to add terminator
            break;
    }

    // check if there is space in directory to store more files
    if(i == SECTOR_SIZE)
    {
        printf("No space to add more directory or file to directory #%d\n", inodeNum);
        free(dataBlock);
        free(inodeBlock);
        return -1;
    }

    // allocate a new free data block to inode
    int k = FindNextAvailableDataBlock();
    ChangeDataBitmapStatus(k, OCCUPIED);

    if(Disk_Read(k + DATA_FIRST_BLOCK_INDEX, dataBlock) == -1)
    {
        printf("Faild to read data block %d from disk\n", k);
        free(dataBlock);
        free(inodeBlock);
        return -1;
    }

    addDirectoryEntryOnSector(dataBlock, dirEntry);
    Disk_Write(k, dataBlock);

    free(dataBlock);
    free(inodeBlock);
    return 0;
    
}

int addDirectory(char* pathName, char **arrayOfBreakPathName, int index)
{
    int curInode;
    int prevInode;

    if(findLeafInodeNumber(pathName, arrayOfBreakPathName, index, &prevInode, &curInode, 1) != 0)
        return -1;

    //--------------------------------------------------------------------
    // allocate inode block and data block
    int inodeIndex = FindNextAvailableInodeBlock();
    if(inodeIndex < 0)
    {
        printf("No availabe inode block.\n");
        return -1;
    }

    ChangeInodeBitmapStatus(inodeIndex, OCCUPIED);

    int dataIndex = FindNextAvailableDataBlock();
    if(dataIndex < 0)
    {
        printf("No availabe data block.\n");
        return -1;
    }

    ChangeDataBitmapStatus(dataIndex, OCCUPIED);

    char *inodeBlock = calloc(sizeof(char), SECTOR_SIZE);
    BuildInode(inodeBlock, DIRECTORY_ID);

    InitializeDirectoryInode(inodeBlock, dataIndex);

    char *dataBlock = calloc(sizeof(char), SECTOR_SIZE);
    InitializeDirectoryFirstDataBlock(dataBlock, prevInode, inodeIndex);

    Disk_Write(dataIndex + DATA_FIRST_BLOCK_INDEX, dataBlock);

    // Write directory inode in disk
    WriteInodeInSector(inodeIndex, inodeBlock);

    free(inodeBlock);
    free(dataBlock);

    //---------------------------------------------------------------------
    // Add entry to parent
    DirectoryEntry *dirEntry = (DirectoryEntry *) malloc ( sizeof(DirectoryEntry));
    strcpy(dirEntry->pathName, arrayOfBreakPathName[index-1]);
    dirEntry->inodePointer = inodeIndex;
    addDirectoryEntry(prevInode, dirEntry);
    free(dirEntry);

    return 0;
}



//retrun -1 if error
//return -1 if inode is not directory
int searchPathInInode ( int inodeNumber , char* search , int* outputInodeNumber)
{
    char* inodeBuffer=calloc(sizeof(char),INODE_SIZE);
    char* inodeSegmentPointerToSector =calloc(sizeof(char),sizeof(int));
    char* sectorBuffer=calloc(sizeof(char),SECTOR_SIZE);
    char directoryEntryNumberInString[sizeof(int)];
    
    DirectoryEntry directoryEntryTemp;
    int inodePointerToSectorNumber;
    int i,j;
    
    // Read the inode
    if( ReadInode(inodeNumber, inodeBuffer) == -1)
    {
        printf("Disk failed to read inode block\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        return -1;
    }
    
    // Check that inode is Directory  FILE_ID=0x80 , DIRECORY_ID=0x00
    if (inodeBuffer[0] & FILE_ID)
    {
        printf("Inode is not directory, it is file.\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        return -1;
    }
    
    //find appropriate sector and looking for search word
    for (i=0;i<SECTOR_PER_FILE_MAX;i++)
    {
        //find sector number
        memcpy((void*)inodeSegmentPointerToSector,(void*)inodeBuffer+META_DATA_PER_INODE_BYTE_NUM+i*sizeof(int),sizeof(int));
        inodePointerToSectorNumber=StringToInt(inodeSegmentPointerToSector);

        //read the appropriate sector and write in sectorBuffer
        if( Disk_Read(DATA_FIRST_BLOCK_INDEX + inodePointerToSectorNumber, sectorBuffer) == -1)
        {
            printf("Disk failed to read sector block\n");
            free(inodeBuffer);
            free(inodeSegmentPointerToSector);
            free(sectorBuffer);
            return -1;
        }
        
        //create directoryEntry
        //check every entry if it is match with search word
        for (j=0; j<(SECTOR_SIZE)/DIRECTORY_LENGTH; j++) {
            memcpy(directoryEntryTemp.pathName,sectorBuffer+sizeof(int)+j*DIRECTORY_LENGTH,PATH_LENGTH_MAX);
            memcpy(directoryEntryNumberInString,sectorBuffer+j*DIRECTORY_LENGTH,sizeof(int));
            directoryEntryTemp.inodePointer=StringToInt(directoryEntryNumberInString);
            
            if(strcmp(directoryEntryTemp.pathName,"")==0)
            {
                free(inodeBuffer);
                free(inodeSegmentPointerToSector);
                free(sectorBuffer);
                return -1;
            }
            
            if(strcmp(search, directoryEntryTemp.pathName)==0)
            {
                *outputInodeNumber=directoryEntryTemp.inodePointer;
                free(inodeBuffer);
                free(inodeSegmentPointerToSector);
                free(sectorBuffer);
                return 0;
            }
        }
        
    }
    
    free(inodeBuffer);
    free(inodeSegmentPointerToSector);
    free(sectorBuffer);
    return -1;
}


int StringToInt (char* binaryCharArray)
{
    int result=binaryCharArray[0];
    result=binaryCharArray[1]<<8 | result;
    result=binaryCharArray[2]<<16 | result;
    result=binaryCharArray[3]<<24 | result;
    return result;
}


// Search in directory to find leaf inode, return -1 if not found
int findLeafInodeNumber(char *path, char** array, int index, int *parent, int *current, int step)
{
    int i = 0;
    *current = 0;
    *parent = 0;
    for(i = 0; i < index; i++)
    {
        *parent = *current;
        if(searchPathInInode(*parent, array[i], current) != 0)
        {
           // printf("SearchPathInInode(%d, %s) --> %d\n", *parent, array[i], *current);
            break;
        }
       // printf("SearchPathInInode(%d, %s) --> %d\n", *parent, array[i], *current);
    }


    if(index - i != step )
    {
        printf("Cant find parents of directory you want\n");
        return -1;
    }

    return 0;
}


int DirSizeFromInode(int inodeNumber)
{
    char* inodeBuffer=calloc(sizeof(char),INODE_SIZE);
    char* inodeSegmentPointerToSector =calloc(sizeof(char),sizeof(int));
    char* sectorBuffer=calloc(sizeof(char),SECTOR_SIZE);
    char directoryEntryNumberInString[sizeof(int)];
    
    DirectoryEntry directoryEntryTemp;
    int inodePointerToSectorNumber;
    int i,j;
    
    // Read the inode
    if( ReadInode(inodeNumber, inodeBuffer) == -1)
    {
        printf("Disk failed to read inode block\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        return -1;
    }
    
    // Check that inode is Directory  FILE_ID=0x80 , DIRECORY_ID=0x00
    if (inodeBuffer[0] & FILE_ID)
    {
        printf("Inode is not directory, it is file.\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        return -1;
    }
    
    //find appropriate sector and looking for search word
    for (i=0;i<SECTOR_PER_FILE_MAX;i++)
    {
        //find sector number
        memcpy((void*)inodeSegmentPointerToSector,(void*)inodeBuffer+META_DATA_PER_INODE_BYTE_NUM+i*sizeof(int),sizeof(int));
        inodePointerToSectorNumber=StringToInt(inodeSegmentPointerToSector);
        
        //read the appropriate sector and write in sectorBuffer
        if( Disk_Read(DATA_FIRST_BLOCK_INDEX + inodePointerToSectorNumber, sectorBuffer) == -1)
        {
            printf("Disk failed to read sector block\n");
            free(inodeBuffer);
            free(inodeSegmentPointerToSector);
            free(sectorBuffer);
            return -1;
        }
        
        //create directoryEntry
        //check how many directory we have
        for (j=0; j<(SECTOR_SIZE)/DIRECTORY_LENGTH; j++) {
            memcpy(directoryEntryTemp.pathName,sectorBuffer+sizeof(int)+j*DIRECTORY_LENGTH,PATH_LENGTH_MAX);
            memcpy(directoryEntryNumberInString,sectorBuffer+j*DIRECTORY_LENGTH,sizeof(int));
            directoryEntryTemp.inodePointer=StringToInt(directoryEntryNumberInString);
            
            if(strcmp(directoryEntryTemp.pathName,"")==0)
            {
                free(inodeBuffer);
                free(inodeSegmentPointerToSector);
                free(sectorBuffer);
                return ((i*((SECTOR_SIZE)/DIRECTORY_LENGTH)+(j))*DIRECTORY_LENGTH);
            }
        }
        
    }
    
    free(inodeBuffer);
    free(inodeSegmentPointerToSector);
    free(sectorBuffer);
    return ((SECTOR_SIZE)/DIRECTORY_LENGTH)*SECTOR_PER_FILE_MAX*DIRECTORY_LENGTH;

}


int DirReadFromInode(int inodeNumber, char* buffer , int size)
{
    char* inodeBuffer=calloc(sizeof(char),INODE_SIZE);
    char* inodeSegmentPointerToSector =calloc(sizeof(char),sizeof(int));
    char* sectorBuffer=calloc(sizeof(char),SECTOR_SIZE);
    
    
    int sectorNumber=size/(DIRECTORY_LENGTH*(SECTOR_SIZE/DIRECTORY_LENGTH))+1;
    int entryNumber=(size/DIRECTORY_LENGTH)%(SECTOR_SIZE/DIRECTORY_LENGTH);
    int inodePointerToSectorNumber;

    int i,j;
    
    // Read the inode
    if( ReadInode(inodeNumber, inodeBuffer) == -1)
    {
        printf("Disk failed to read inode block\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        return -1;
    }
    
    // Check that inode is Directory  FILE_ID=0x80 , DIRECORY_ID=0x00
    if (inodeBuffer[0] & FILE_ID)
    {
        printf("Inode is not directory, it is file.\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        return -1;
    }
    
    for (i=0;i<sectorNumber;i++)
    {
        //find sector number
        memcpy((void*)inodeSegmentPointerToSector,(void*)inodeBuffer+META_DATA_PER_INODE_BYTE_NUM+i*sizeof(int),sizeof(int));
        inodePointerToSectorNumber=StringToInt(inodeSegmentPointerToSector);
        
        //read the appropriate sector and write in sectorBuffer
        if( Disk_Read(DATA_FIRST_BLOCK_INDEX + inodePointerToSectorNumber, sectorBuffer) == -1)
        {
            printf("Disk failed to read sector block\n");
            free(inodeBuffer);
            free(inodeSegmentPointerToSector);
            free(sectorBuffer);
            return -1;
        }
        
        //create directoryEntry
        //copy the PathName of each directory to buffer
        for (j=0; j<(SECTOR_SIZE)/DIRECTORY_LENGTH; j++) {
            memcpy(buffer+(((i*(DIRECTORY_LENGTH*(SECTOR_SIZE/DIRECTORY_LENGTH)))+j)*DIRECTORY_LENGTH),
                   sectorBuffer+j*DIRECTORY_LENGTH,DIRECTORY_LENGTH);
            
            if(i==sectorNumber-1 && j==entryNumber-1)
            {
                break;
            }
        }
        
    }
    
    return 0;
}

int DeleteEntryFromDirectory(int inodeNumber , int inodeSearch )
{
    char* inodeBuffer=calloc(sizeof(char),INODE_SIZE);
    char* inodeSegmentPointerToSector =calloc(sizeof(char),sizeof(int));
    char* sectorBuffer=calloc(sizeof(char),SECTOR_SIZE);
    char* inodeSegmentPointerToNextSector =calloc(sizeof(char),sizeof(int));
    char* nextSectorBuffer=calloc(sizeof(char),SECTOR_SIZE);
    char* firstEntryOfNextSectorBuffer=calloc(sizeof(char),DIRECTORY_LENGTH);
    char directoryEntryNumberInString[sizeof(int)];
    
    DirectoryEntry directoryEntryTemp;

    int inodePointerToSectorNumber;
    int inodePointerToNextSectorNumber;
    int i,j;
    
    int isFind=0;
    
    // Read the inode
    if( ReadInode(inodeNumber, inodeBuffer) == -1)
    {
        printf("Disk failed to read inode block\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        free(inodeSegmentPointerToNextSector);
        free(nextSectorBuffer);
        free(firstEntryOfNextSectorBuffer);
        return -1;
    }
    
    // Check that inode is Directory  FILE_ID=0x80 , DIRECORY_ID=0x00
    if (inodeBuffer[0] & FILE_ID)
    {
        printf("Inode is not directory, it is file.\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        free(inodeSegmentPointerToNextSector);
        free(nextSectorBuffer);
        free(firstEntryOfNextSectorBuffer);
        return -1;
    }
    
    //find appropriate sector and looking for inodeSearch
    for (i=0;i<SECTOR_PER_FILE_MAX;i++)
    {
        //find sector number
        memcpy((void*)inodeSegmentPointerToSector,(void*)inodeBuffer+META_DATA_PER_INODE_BYTE_NUM+i*sizeof(int),sizeof(int));
        inodePointerToSectorNumber=StringToInt(inodeSegmentPointerToSector);
        
        //read the appropriate sector and write in sectorBuffer
        if( Disk_Read(DATA_FIRST_BLOCK_INDEX + inodePointerToSectorNumber, sectorBuffer) == -1)
        {
            printf("Disk failed to read sector block\n");
            free(inodeBuffer);
            free(inodeSegmentPointerToSector);
            free(sectorBuffer);
            free(inodeSegmentPointerToNextSector);
            free(nextSectorBuffer);
            free(firstEntryOfNextSectorBuffer);
            return -1;
        }
        
        // also find next sector number and read it for deletation
        if (i<SECTOR_PER_FILE_MAX-1)
        {
            memcpy((void*)inodeSegmentPointerToNextSector,(void*)inodeBuffer+META_DATA_PER_INODE_BYTE_NUM+(i+1)*sizeof(int),sizeof(int));
            inodePointerToNextSectorNumber=StringToInt(inodeSegmentPointerToNextSector);
        
            //read the appropriate sector and write in sectorBuffer
            if( Disk_Read(DATA_FIRST_BLOCK_INDEX + inodePointerToNextSectorNumber, nextSectorBuffer) == -1)
            {
                printf("Disk failed to read sector block\n");
                free(inodeBuffer);
                free(inodeSegmentPointerToSector);
                free(sectorBuffer);
                free(inodeSegmentPointerToNextSector);
                free(nextSectorBuffer);
                free(firstEntryOfNextSectorBuffer);
                return -1;
            }
        }
        
        // save first entry of next sector buffer
        memcpy(firstEntryOfNextSectorBuffer,nextSectorBuffer,DIRECTORY_LENGTH);
        
        //create directoryEntry
        //check every entry if it is match with search word
        for (j=0; j<(SECTOR_SIZE)/DIRECTORY_LENGTH; j++) {
            memcpy(directoryEntryTemp.pathName,sectorBuffer+sizeof(int)+j*DIRECTORY_LENGTH,PATH_LENGTH_MAX);
            memcpy(directoryEntryNumberInString,sectorBuffer+j*DIRECTORY_LENGTH,sizeof(int));
            directoryEntryTemp.inodePointer=StringToInt(directoryEntryNumberInString);
            
            if(strcmp(directoryEntryTemp.pathName,"")==0 && isFind==0)
            {
                free(inodeBuffer);
                free(inodeSegmentPointerToSector);
                free(sectorBuffer);
                free(inodeSegmentPointerToNextSector);
                free(nextSectorBuffer);
                free(firstEntryOfNextSectorBuffer);
                return -1;
            }
            
            if(directoryEntryTemp.inodePointer==inodeSearch)
            {
                isFind=1;
            }
            
            if(strcmp(directoryEntryTemp.pathName,"")==0 && isFind==1)
            {
                
                // write sectorBuffer in disk
                if( Disk_Write(DATA_FIRST_BLOCK_INDEX + inodePointerToSectorNumber, sectorBuffer) == -1)
                {
                    printf("Disk failed to write changed block\n");
                }
                free(inodeBuffer);
                free(inodeSegmentPointerToSector);
                free(sectorBuffer);
                free(inodeSegmentPointerToNextSector);
                free(nextSectorBuffer);
                free(firstEntryOfNextSectorBuffer);
                return 0;
            }
            
            if(isFind==1){
                if(j<((SECTOR_SIZE/DIRECTORY_LENGTH)-1))
                {
                    memcpy(sectorBuffer+j*DIRECTORY_LENGTH,sectorBuffer+(j+1)*DIRECTORY_LENGTH,DIRECTORY_LENGTH);
                }
                else if(j==(SECTOR_SIZE)/DIRECTORY_LENGTH-1 && i<SECTOR_PER_FILE_MAX-1)
                {
                    memcpy(sectorBuffer+j*DIRECTORY_LENGTH,firstEntryOfNextSectorBuffer,DIRECTORY_LENGTH);
                }
                else if(j==(SECTOR_SIZE)/DIRECTORY_LENGTH-1 && i==SECTOR_PER_FILE_MAX-1)
                {
                    memset(sectorBuffer+j*DIRECTORY_LENGTH,0,DIRECTORY_LENGTH);
                }
            }

        }
        
        // write sectorBuffer in disk
        if( Disk_Write(DATA_FIRST_BLOCK_INDEX + inodePointerToSectorNumber, sectorBuffer) == -1)
        {
            printf("Disk failed to write changed block\n");
        }
        
    }
    
    free(inodeBuffer);
    free(inodeSegmentPointerToSector);
    free(sectorBuffer);
    free(inodeSegmentPointerToNextSector);
    free(nextSectorBuffer);
    free(firstEntryOfNextSectorBuffer);

    return 0;

}
