#ifndef PARAMETERS_H
#define PARAMETERS_H

extern char MAGIC_NUMBER_0;    //This one and three below are some magic number that stored in super block to distinguish it from other disks
extern char MAGIC_NUMBER_1;
extern char MAGIC_NUMBER_2;
extern char MAGIC_NUMBER_3;

extern int SUPER_BLOCK_INDEX;   //This shows the block which Super block is stored in it
extern int SUPER_BLOCK_NUM;     //Number of super blocks!!!

extern int SECTOR_NUM;          //Number of sectors!!!
//extern int SECTOR_SIZE;       //there is a sector size in libfs   <><<><><>>><<>
extern int SECTOR_PER_FILE_MAX; //The maximum number of sectors that could be in an inode

extern int AVAILIBLE;   //This and OCCUPIED are show the readability of bitmaps
extern int OCCUPIED;

extern int FILE_ID;         //File IDentification number!!!
extern int DIRECTORY_ID;    //Directory ID!!!

extern int PATH_LENGTH_MAX;         //Maximum length of path!!! It is 15 but because of last character that is null term it would be 16.
extern int DIRECTORY_LENGTH;        //The length of a path + The size of an integer in Bytes
extern int FILENAME_LENGTH_MAX;     //Maximum length of filename
extern int OPEN_FILE_NUM_MAX;       //Maximum number of open files. It is used for determine the length of the fileTable
extern int FILE_NUM_MAX;            //Maximum number of files that a file system can hold

extern int INODE_BITMAP_BLOCK_NUM;
extern int INODE_BLOCK_NUM;         //Number of inode blocks!!!
extern int INODE_BYTEMAP_LENGTH;    //It is 250
extern int INODE_FIRST_BLOCK_INDEX; //This shows the index of first block which inodes are stored in it
extern int INODE_PER_BLOCK_NUM;     //Number of inode per block!!!

extern int DATA_BITMAP_BLOCK_NUM;
extern int DATA_BLOCK_NUM;              //Number of data blocks!!!
extern int DATA_BLOCK_BYTEMAP_LENGTH;   //Actually it is 1218.125 means 9745/8 because it is Bytemap
extern int DATA_FIRST_BLOCK_INDEX;      //This shows the index of first block which datas are stored in it
extern int DATA_BLOCK_PER_CHAR_NUM;

extern int NULL_TERM;   //Null Terminator

#endif // PARAMETERS_H
