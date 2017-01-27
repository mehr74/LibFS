#include "parameters.h"
#include "LibDisk.h"

char MAGIC_NUMBER_0 = 13;    //This one and three below are some magic number that stored in super block to distinguish it from other disks
char MAGIC_NUMBER_1 = 13;
char MAGIC_NUMBER_2 = 13;
char MAGIC_NUMBER_3 = 13;

int SUPER_BLOCK_INDEX = 0;  //This shows the block which Super block is stored in it
int SUPER_BLOCK_NUM = 1;    //Number of super blocks!!!

int SECTOR_NUM = 10000;     //Number of sectors!!!
// int SECTOR_SIZE = 512;    //there is a sector size in libfs   <><<><><>>><<>
int SECTOR_PER_FILE_MAX = 30;   //The maximum number of sectors that could be in an inode

int AVAILIBLE = 0;      //This and OCCUPIED are show the readability of bitmaps
int OCCUPIED = 1;

char FILE_ID = 0x80;        //File IDentification number!!!
char DIRECTORY_ID = 0x00;   //Directory ID!!!

int PATH_LENGTH_MAX = 16;       //Maximum length of path!!! It is 15 but because of last character that is null term it would be 16.
int FULL_PATH_LENGTH_MAX=256;   //Maximum of full path name
int DIRECTORY_LENGTH = 20;      //The length of a path + The size of an integer in Bytes
int FILENAME_LENGTH_MAX = 16;   //Maximum length of filename
int OPEN_FILE_NUM_MAX = 256;    //Maximum number of open files. It is used for determine the length of the fileTable
int FILE_NUM_MAX = 1000;        //Maximum number of files that a file system can hold

int INODE_BITMAP_BLOCK_NUM = 1;
int META_DATA_PER_INODE_BYTE_NUM=8;
int INODE_FIRST_BITMAP_BLOCK_INDEX=1;   //Index of bitmap
int INODE_BLOCK_NUM = 250;              //Number of inode blocks!!!
int INODE_BYTEMAP_LENGTH = 250;         //It is 250
int INODE_FIRST_BLOCK_INDEX = 5;        //This shows the index of first block which inodes are stored in it
int INODE_PER_BLOCK_NUM = 4;            //Number of inode per block!!!
int INODE_SIZE = 128;                   //Size of each inode in a block

int DATA_BITMAP_BLOCK_NUM = 3;
int DATA_FIRST_BITMAP_BLOCK_INDEX=2;    // Index of bitmap
int DATA_BLOCK_NUM =9745;               //Number of data blocks!!!
int DATA_BITMAP_BLOCK_LENGTH = 1219;   //Actually it is 1218.125 means 9745/8 because it is Bytemap
int DATA_FIRST_BLOCK_INDEX = 255;       //This shows the index of first block which datas are stored in it
int DATA_BLOCK_PER_CHAR_NUM = 8;

int NULL_TERM = 0;      //Null Terminator

char BIT_0=0x01;       //is 0x01
char BIT_1=0x02;       //is 0x02
char BIT_2=0x04;       //is 0x04
char BIT_3=0x08;       //is 0x08
char BIT_4=0x10;       //is 0x10
char BIT_5=0x20;       //is 0x20
char BIT_6=0x40;       //is 0x40
char BIT_7=0x80;       //is 0x80
