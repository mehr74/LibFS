#include "parameters.h"
#include "LibDisk.h"
#include "LibFS.h"

// return the # of sector which is available
// return -1 means there is an error
// return -2 means there is no available inode
int FindNextAvailableInode(){
    
    // define char* to read inode bitmap sector
    char* inodeBitmapBuffer=malloc(sizeof(char),SECTOR_SIZE);
    //define a temp boolean
    boolean findAvailable=FALSE;
    int i,j;
    int temp;
    char* bytemapTemp=malloc(sizeof(char),1);

    //check next free inode
    //first handle errors
    if(Disk_Read(INODE_FIRST_BITMAP_BLOCK_INDEX,inodeBitmapBuffer)==-1){
        if (diskErrno==E_INVALID_PARAM){
            printf("Invalid sector access or buffer\n");
        }
        
        if(diskErrno==E_MEM_OP){
            printf("Memory coping is failed\n");
        }
        return -1;
    }
    
    // checking available inodes
    for(i=0;i<SECTOR_SIZE;i++){
        ConvertBitmapToBytemap(inodeBitmapBuffer[i],bytemapTemp);
        for(j=0;j<8;j++){
            if(!bytemapTemp[j]){
                return i*8+j;
            }
        }
    }
    
    // there is no available inode
    return -2;
    
}


int ConvertBitmapToBytemap(char bitmap,char* bytemap){
    bytemap[0]=(bitmap & BIT_0)?1:0;
    bytemap[1]=(bitmap & BIT_1)?1:0;
    bytemap[2]=(bitmap & BIT_2)?1:0;
    bytemap[3]=(bitmap & BIT_3)?1:0;
    bytemap[4]=(bitmap & BIT_4)?1:0;
    bytemap[5]=(bitmap & BIT_5)?1:0;
    bytemap[6]=(bitmap & BIT_6)?1:0;
    bytemap[7]=(bitmap & BIT_7)?1:0;
    return 0;
}

int ConvertBytemapToBitmap(char* bitmap,char* bytemap){
    bitmap[0]=0x00;
    bitmap[0]=(bytemap[0])?(bitmap|BIT_0):(bitmap);
    bitmap[0]=(bytemap[1])?(bitmap|BIT_1):(bitmap);
    bitmap[0]=(bytemap[2])?(bitmap|BIT_2):(bitmap);
    bitmap[0]=(bytemap[3])?(bitmap|BIT_3):(bitmap);
    bitmap[0]=(bytemap[4])?(bitmap|BIT_4):(bitmap);
    bitmap[0]=(bytemap[5])?(bitmap|BIT_5):(bitmap);
    bitmap[0]=(bytemap[6])?(bitmap|BIT_6):(bitmap);
    bitmap[0]=(bytemap[7])?(bitmap|BIT_7):(bitmap);
    return 0;
}
