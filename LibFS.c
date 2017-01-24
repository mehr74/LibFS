#include "LibFS.h"
#include "LibDisk.h"
#include "builder.h"
#include "parameters.h"

// global errno value here
int osErrno;
// a temp variable for reading buffers
char* read_buffer;

int FS_Boot(char *path)
{
    printf("FS_Boot %s\n", path);

    // check for path and read the existing file
    if (Disk_Load(path)==-1)
    {
        if (diskErrno==E_OPENING_FILE)
        {
            printf("This filename is not exist\n");
        }else if(diskErrno==E_READING_FILE){
            printf("There is some errors with reading from disk image\n");
            return -1;
        }
        
        // oops, check for errors
        if (Disk_Init() == -1)
        {
            printf("Disk_Init() failed\n");
            osErrno = E_GENERAL;
            return -1;
        }
        
        // do all of the other stuff needed...
        if (BuildMetadataBlocks() == -1)
        {
            osErrno = E_GENERAL;
        }
        
    }else{
        // check for magic number of block
        //handling errors
        if(Disk_Read(SUPER_BLOCK_INDEX,read_buffer)==-1){
            if (diskErrno==E_INVALID_PARAM){
                printf("Invalid sector access or buffer\n");
            }
            
            if(diskErrno==E_MEM_OP){
                printf("Memory coping is failed\n");
            }
            return -1;
        }
        // checking number
        if(read_buffer[0]==MAGIC_NUMBER_0 && read_buffer[1]==MAGIC_NUMBER_1 && read_buffer[2]==MAGIC_NUMBER_2 && read_buffer[3]==MAGIC_NUMBER_3){
            printf("Misson Complete\n");
        }else{
            printf("The existing file is not a filesystem\n");
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
    return 0;
}

int Dir_Size(char *path)
{
    printf("Dir_Size\n");
    return 0;
}

int Dir_Read(char *path, void *buffer, int size)
{
    printf("Dir_Read\n");
    return 0;
}

int Dir_Unlink(char *path)
{
    printf("Dir_Unlink\n");
    return 0;
}

