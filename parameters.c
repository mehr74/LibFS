#include "parameters.h"

MAGIC_NUMBER_0 = 13;    //This one and three below are some magic number that stored in super block to distinguish it from other disks
MAGIC_NUMBER_1 = 13;
MAGIC_NUMBER_2 = 13;
MAGIC_NUMBER_3 = 13;

SUPER_BLOCK_INDEX = 0;  //This shows the block which Super block is stored in it
SUPER_BLOCK_NUM = 1;    //Number of super blocks!!!

SECTOR_NUM = 10000;     //Number of sectors!!!
//SECTOR_SIZE = 512;    //there is a sector size in libfs   <><<><><>>><<>
SECTOR_PER_FILE_MAX = 30;   //The maximum number of sectors that could be in an inode

AVAILIBLE = 0;      //This and OCCUPIED are show the readability of bitmaps
OCCUPIED = 1;

FILE_ID = 0;        //File IDentification number!!!
DIRECTORY_ID = 1;   //Directory ID!!!

PATH_LENGTH_MAX = 16;       //Maximum length of path!!! It is 15 but because of last character that is null term it would be 16.
DIRECTORY_LENGTH = 20;      //The length of a path + The size of an integer in Bytes
FILENAME_LENGTH_MAX = 16;   //Maximum length of filename
OPEN_FILE_NUM_MAX = 256;    //Maximum number of open files. It is used for determine the length of the fileTable
FILE_NUM_MAX = 1000;        //Maximum number of files that a file system can hold

INODE_BITMAP_BLOCK_NUM = 1;
INODE_BLOCK_NUM = 250;          //Number of inode blocks!!!
INODE_BYTEMAP_LENGTH = 250;     //It is 250
INODE_FIRST_BLOCK_INDEX = 5;    //This shows the index of first block which inodes are stored in it
INODE_PER_BLOCK_NUM = 4;        //Number of inode per block!!!

DATA_BITMAP_BLOCK_NUM = 3;
DATA_BLOCK_NUM =9745;               //Number of data blocks!!!
DATA_BLOCK_BYTEMAP_LENGTH = 1219;   //Actually it is 1218.125 means 9745/8 because it is Bytemap
DATA_FIRST_BLOCK_INDEX = 255;       //This shows the index of first block which datas are stored in it
DATA_BLOCK_PER_CHAR_NUM = 8;

NULL_TERM = 0;      //Null Terminator