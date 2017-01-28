#include "LibFS.h"
#include "LibDisk.h"
#include "builder.h"
#include "parameters.h"
#include "directory.h"
#include "inode.h"
#include "file.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// global errno value here
int osErrno;
// a temp variable for reading buffers
char* read_buffer;

int FS_Boot(char *path)
{
    printf("FS_Boot %s\n", path);

    // oops, check for errors
    if (Disk_Init() == -1)
    {
        printf("Disk_Init() failed\n");
        osErrno = E_GENERAL;
        return -1;
    }

    // do all of the other stuff needed...
    // check for path and read the existing file
    if (Disk_Load(path)==-1)
    {
        if (diskErrno==E_OPENING_FILE)
        {
            printf("This filename does not exist\n");
        }
        else if(diskErrno==E_READING_FILE)
        {
            printf("There is some errors with reading from disk image\n");
        }

        // do all of the other stuff needed...
        if (BuildMetadataBlocks() == -1) {
            osErrno = E_GENERAL;
        }
    }
    else
    {
        // check for magic number of block
        // handling errors
        if (CheckFileSystemSuperBlock() == -1)
        {
            osErrno = E_GENERAL;
            return -1;
        }
    }
    return 0;
}

int FS_Sync()
{
    printf("FS_Sync\n");
    Disk_Save("disk.txt");
    return 0;
}


int File_Create(char *file)
{
    printf("FS_Create\n");

    // allocate memory for storing string...
    char* array[128];
    char myPath[256];

    // make a copy of path to modify
    strcpy(myPath, file);

    // tokenize path and make array of path elements...
    int i = BreakPathName(myPath, array);

    int parent;
    int current;

    if(findLeafInodeNumber(myPath, array, i, &parent, &current, 1) != 0)
    {
        if(findLeafInodeNumber(myPath, array, i, &parent, &current, 0) == 0)
        {
            osErrno = E_GENERAL;
            return -1;
        }
        osErrno = E_CREATE;
        return -1;
    }

    if(addFile(parent, array[i-1]) != 0)
    {
        osErrno = E_CREATE;
        return -1;
    }

    return 0;
}

int File_Open(char *file)
{
    printf("FS_Open\n");
    return 0;
}

int File_Read(int fd, void *buffer, int size)
{

    printf("FS_Read\n");
    return 0;
}

int File_Write(int fd, void *buffer, int size)
{
    printf("FS_Write\n");
    return 0;
}

int File_Seek(int fd, int offset)
{
    printf("FS_Seek\n");
    return 0;
}

int File_Close(int fd)
{
    printf("FS_Close\n");
    return 0;
}

int File_Unlink(char *file)
{
    printf("FS_Unlink\n");
    return 0;
}


// directory ops
int Dir_Create(char *path)
{
    printf("Dir_Create %s\n", path);

    // allocate memory for storing string...
    char* array[128];
    char myPath[256];

    // make a copy of path to modify
    strcpy(myPath, path);

    // tokenize path and make array of path elements...
    int i = BreakPathName(myPath, array);

    if(addDirectory(myPath, array, i) != 0)
    {
        osErrno = E_CREATE;
        return -1;
    }

    return 0;
}

int Dir_Size(char *path)
{
    printf("Dir_Size\n");

    // allocate memory for storing string...
    char* array[128];
    char myPath[256];

    // make a copy of path to modify
    strcpy(myPath, path);

    // tokenize path and make array of path elements...
    int i = BreakPathName(myPath, array);

    int parent;
    int current;

    if(findLeafInodeNumber(myPath, array, i, &parent, &current, 0) != 0)
        return -1;
    int size = DirSizeFromInode(current);

    printf("Size : %d\n", size);

    return size;
}

int Dir_Read(char *path, void *buffer, int size)
{
    buffer = calloc(sizeof(char), size);
    printf("Dir_Read ( %s, %d)\n", path, size);
    // allocate memory for storing string...
    char* array[128];
    char myPath[256];

    if(size < Dir_Size(path))
    {
        osErrno = E_BUFFER_TOO_SMALL;
        return -1;
    }
    // make a copy of path to modify
    strcpy(myPath, path);

    // tokenize path and make array of path elements...
    int i = BreakPathName(myPath, array);

    int parent;
    int current;

    if(findLeafInodeNumber(myPath, array, i, &parent, &current, 0) != 0)
        return -1;

    DirReadFromInode(current, buffer, size);

    printBlockHex(buffer, size);
    return 0;
}

int Dir_Unlink(char *path)
{
    printf("Dir_Unlink\n");

    // allocate memory for storing string...
    char* array[128];
    char myPath[256];

    // make a copy of path to modify
    strcpy(myPath, path);

    // tokenize path and make array of path elements...
    int i = BreakPathName(myPath, array);

    int parent;
    int current;

    if(findLeafInodeNumber(myPath, array, i, &parent, &current, 0) != 0)
        return -1;

    printf("DeleteEntryFromDirectory( %d, %d ) ", parent, current);
    DeleteEntryFromDirectory(parent, current);


    int DataBlocksOccupied[30];
    int j = DataBlocksOccupiedByInode(current, DataBlocksOccupied);

    for (i = 0; i < j; i++)
    {
        ChangeDataBitmapStatus(DataBlocksOccupied[i], AVAILIBLE);
    }

    ChangeInodeBitmapStatus(current, AVAILIBLE);

    return 0;
}

