#ifndef INODE_H
#define INODE_H

// return the # of sector which is available
// return -1 means there is an error
// return -2 means there is no available inode
int FindNextAvailableInode();

int ConvertBitmapToBytemap(char bitmap,char* bytemap);
int ConvertBytemapToBitmap(char* bitmap,char* bytemap);


#endif // INODE_H