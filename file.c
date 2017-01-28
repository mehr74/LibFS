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

int initializeFileTableEntryByInode(int inodeNum, FileTableEntry *fileTableEntry)
{
    char* inodeBlock = calloc(sizeof(char), INODE_SIZE);
    if(ReadInode(inodeNum, inodeBlock) == -1)
        return -1;

    fileTableEntry->inodePointer = inodeNum;
    fileTableEntry->filePointer = 0;
    fileTableEntry->fileDescriptor = getAvailabeFileDescriptor();
}

int getAvailabeFileDescriptor()
{
    int i;
    if(numberOfOpenFiles > OPEN_FILE_NUM_MAX)
        return -1;
    for(i = 0; i < OPEN_FILE_NUM_MAX; i++)
    {
        FileTableEntry* fileTableEntry;
        if(getFileTableEntry(i, fileTableEntry) == -1)
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
