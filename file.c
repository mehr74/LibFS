#include "file.h"
#include "inode.h"
#include "parameters.h"
#include "builder.h"
#include "LibDisk.h"
#include "directory.h"
#include <string.h>
#include <stdio.h>
#include "LibDisk.h"

// the fileTable in memory (static makes it private to the file)
static FileTableEntry* fileTable;

static int numberOfOpenFiles = 0;

int addFile(int parentInode, char* fileName)
{
    printf("filename : %s\n", fileName);
    printf("parentInode : %d\n", parentInode);
    // Get an inode block
    int inodeIndex = FindNextAvailableInodeBlock();
    if(inodeIndex < 0)
        return -1;

    // Get a data block
    int dataIndex = FindNextAvailableDataBlock();
    if(dataIndex < 0)
        return -1;

    // update bitmap blocks of inode and data blocks
    ChangeInodeBitmapStatus(inodeIndex, OCCUPIED);
    ChangeDataBitmapStatus(dataIndex, OCCUPIED);

    char* inodeBlock = calloc(sizeof(char), SECTOR_SIZE/INODE_PER_BLOCK_NUM);
    BuildInode(inodeBlock, FILE_ID);
    InitializeFileInode(inodeBlock, dataIndex);

    // check if failed to write inode in disk
    if(WriteInodeInSector(inodeIndex, inodeBlock) == -1)
    {
        free(inodeBlock);
        return -1;
    }

    free(inodeBlock);

    char* dataBlock = calloc(sizeof(char), SECTOR_SIZE);

    // check if failed to write data block in disk
    if(Disk_Write(DATA_FIRST_BLOCK_INDEX + dataIndex, dataBlock) == -1)
    {
        printf("Failed to write datablock to disk");
        free(dataBlock);
        return -1;
    }

    DirectoryEntry * dirEntry = (DirectoryEntry *) malloc ( sizeof(DirectoryEntry));
    dirEntry->inodePointer = inodeIndex;
    strcpy(dirEntry->pathName, fileName);

    if(addDirectoryEntry(parentInode, dirEntry) != 0)
    {
        free(dirEntry);
        return -1;
    }

    // file create successfully...
    free(dataBlock);
    free(dirEntry);
    return 0;
}

int CreateFileTable()
{
    fileTable = (FileTableEntry *) calloc(OPEN_FILE_NUM_MAX, sizeof(FileTableEntry));
    if(fileTable == NULL)
        return -1;
    return 0;
}

int getAvailabeFileDescriptor()
{
    int i;
    if(numberOfOpenFiles > OPEN_FILE_NUM_MAX)
        return -1;
    for(i = 0; i < OPEN_FILE_NUM_MAX; i++)
    {
        if(fileTable[i].isValid == NOT_VALID)
            return i;
    }
    return -1;
}

int getFileTableEntry(int fileDescriptor, FileTableEntry *fileTableEntry)
{
    int i = 0;
    for(i = 0; i < numberOfOpenFiles; i++)
    {
        if(fileTable[i].fileDescriptor == fileDescriptor)
        {
            fileTableEntry = &fileTable[i];
            return 0;
        }
    }
    return -1;
}

int SizeOfFile(int inodeNumber)
{
    char* inodeBuffer=calloc(sizeof(char),INODE_SIZE);
    char* inodeSegmentPointerToSector =calloc(sizeof(char),sizeof(int));
    char* sectorBuffer=calloc(sizeof(char),SECTOR_SIZE);
    char* sizeBuffer=calloc(sizeof(char),sizeof(int));
    
    int inodePointerToSectorNumber;
    int fileSize;
    
    // Read the inode
    if( ReadInode(inodeNumber, inodeBuffer) == -1)
    {
        printf("Disk failed to read inode block\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        free(sizeBuffer);
        return -1;
    }
    
    // Check that inode is File  FILE_ID=0x80 , DIRECORY_ID=0x00
    if (!(inodeBuffer[0] & FILE_ID))
    {
        printf("Inode is not directory, it is file.\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        free(sizeBuffer);
        return -1;
    }
    
    //find appropriate sector which shows the size
    memcpy((void*)inodeSegmentPointerToSector,(void*)inodeBuffer+META_DATA_PER_INODE_BYTE_NUM,sizeof(int));
    inodePointerToSectorNumber=StringToInt(inodeSegmentPointerToSector);
        
    //read the appropriate sector and write in sectorBuffer
    if( Disk_Read(DATA_FIRST_BLOCK_INDEX + inodePointerToSectorNumber, sectorBuffer) == -1)
    {
        printf("Disk failed to read sector block\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        free(sizeBuffer);

        return -1;
    }
    
    //find size and convert it into int
    memcpy((void*)sizeBuffer,(void*)sectorBuffer,sizeof(int));
    fileSize=StringToInt(sizeBuffer);
    
    free(inodeBuffer);
    free(inodeSegmentPointerToSector);
    free(sectorBuffer);
    free(sizeBuffer);

    return fileSize;
}

int DataBlocksOccupiedByFile(int inodeNumber,int* sectorNumbers)
{
    char* inodeBuffer=calloc(sizeof(char),INODE_SIZE);
    char* inodeSegmentPointerToSector =calloc(sizeof(char),sizeof(int));
    
    int inodePointerToSectorNumber;
    int i;
    
    
    int entryOccupiedNumber;
    int fileSize=0;
    
    // Find size of file
    fileSize=SizeOfFile(inodeNumber);
    
    if(fileSize == -1)
    {
        return -1;
    }
    
    // Number of occupied entry by file
    entryOccupiedNumber=min(SECTOR_PER_FILE_MAX,(fileSize+sizeof(int)-1)/SECTOR_SIZE+1);
    
    // Read the inode
    if( ReadInode(inodeNumber, inodeBuffer) == -1)
    {
        printf("Disk failed to read inode block\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        return -1;
    }
    
    //find appropriate sector and looking for search word
    for (i=0;i<entryOccupiedNumber;i++)
    {
        //find sector number
        memcpy((void*)inodeSegmentPointerToSector,(void*)inodeBuffer+META_DATA_PER_INODE_BYTE_NUM+i*sizeof(int),sizeof(int));
        inodePointerToSectorNumber=StringToInt(inodeSegmentPointerToSector);
        
        // add this sector numbers and increase counter
        sectorNumbers[i]=inodePointerToSectorNumber;
        
    }
    
    free(inodeBuffer);
    free(inodeSegmentPointerToSector);
    return entryOccupiedNumber;
    
}

int FileRead(int fd, char *buffer, int size){
    char* inodeBuffer=calloc(sizeof(char),INODE_SIZE);
    char* inodeSegmentPointerToSector =calloc(sizeof(char),sizeof(int));
    char* sectorBuffer=calloc(sizeof(char),SECTOR_SIZE);
    
    // determine how many bytes we must read
    int readSize=min(fileTable[fd].sizeOfFile-fileTable[fd].filePointer,size);
    int transferedSize=0;
    
    int inodePointerToSectorNumber;
    int i;
    
    // Number of occupied entry by file
    int entryOccupiedNumber=min(SECTOR_PER_FILE_MAX,(fileTable[fd].sizeOfFile+sizeof(int)-1)/SECTOR_SIZE+1);
    
    // Read the inode
    if( ReadInode(fileTable[fd].inodePointer, inodeBuffer) == -1)
    {
        printf("Disk failed to read inode block\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        return -1;
    }
    
    //find appropriate sectors and reading from them and save into buffer
    for (i=0;i<entryOccupiedNumber;i++)
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
        
        //transfering data
        if (i==0)
        {
            memcpy(buffer+transferedSize,sectorBuffer+sizeof(int),min(SECTOR_SIZE-sizeof(int),readSize));
            transferedSize=transferedSize + min(SECTOR_SIZE-sizeof(int),readSize);
        }
        else
        {
            memcpy(buffer+transferedSize,sectorBuffer,min(SECTOR_SIZE,readSize-transferedSize));
            transferedSize=transferedSize+min(SECTOR_SIZE,readSize-transferedSize);
        }
        
        if(transferedSize==readSize)
        {
            free(inodeBuffer);
            free(inodeSegmentPointerToSector);
            free(sectorBuffer);
            return transferedSize;
        }
        
        if(transferedSize>readSize)
        {
            printf("Some error in transfering data\n");
            free(inodeBuffer);
            free(inodeSegmentPointerToSector);
            free(sectorBuffer);
            return -1;
        }
        
    }
    
    free(inodeBuffer);
    free(inodeSegmentPointerToSector);
    free(sectorBuffer);
    return transferedSize;
    
}


//return -1 for errors
//return -2 if size is not appropriate
int WriteFile(int fd, char* buffer , int size)
{
    char* inodeBuffer=calloc(sizeof(char),INODE_SIZE);
    char* inodeSegmentPointerToSector =calloc(sizeof(char),sizeof(int));
    char* sectorBuffer=calloc(sizeof(char),SECTOR_SIZE);
    
    // determine how many bytes we must read
    int transferedSize=0;
    
    int inodePointerToSectorNumber;
    int i;
    
    // Number of occupied entry by file
    int entryOccupiedNumber=min(SECTOR_PER_FILE_MAX,(fileTable[fd].sizeOfFile+sizeof(int)-1)/SECTOR_SIZE+1);
    
    // Check size of file. it must be lower than tranfering size
    if(fileTable[fd].sizeOfFile-fileTable[fd].filePointer < size)
    {
        printf("There is not enough size to write file\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        return -2;
    }
    
    // Read the inode
    if( ReadInode(fileTable[fd].inodePointer, inodeBuffer) == -1)
    {
        printf("Disk failed to read inode block\n");
        free(inodeBuffer);
        free(inodeSegmentPointerToSector);
        free(sectorBuffer);
        return -1;
    }
    
    //find appropriate sectors and reading from them and save into buffer
    for (i=0;i<entryOccupiedNumber;i++)
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
        
        //transfering data
        if (i==0)
        {
            memcpy(sectorBuffer+sizeof(int),buffer+transferedSize,min(SECTOR_SIZE-sizeof(int),size));
            transferedSize=transferedSize + min(SECTOR_SIZE-sizeof(int),size);
        }
        else
        {
            memcpy(sectorBuffer,buffer+transferedSize,min(SECTOR_SIZE,size-transferedSize));
            transferedSize=transferedSize+min(SECTOR_SIZE,size-transferedSize);
        }
        
        if(transferedSize==size)
        {
            free(inodeBuffer);
            free(inodeSegmentPointerToSector);
            free(sectorBuffer);
            return transferedSize;
        }
        
        if(transferedSize>size)
        {
            printf("Some error in transfering data. Note: some sectors have changed\n");
            free(inodeBuffer);
            free(inodeSegmentPointerToSector);
            free(sectorBuffer);
            return -1;
        }
        
        // write sectorBuffer in disk
        if( Disk_Write(DATA_FIRST_BLOCK_INDEX + inodePointerToSectorNumber, sectorBuffer) == -1)
        {
            printf("Disk failed to write changed block\n");
            free(inodeBuffer);
            free(inodeSegmentPointerToSector);
            free(sectorBuffer);
            return -1;
        }
        
    }
    
    free(inodeBuffer);
    free(inodeSegmentPointerToSector);
    free(sectorBuffer);
    return transferedSize;
}
