#ifndef INODE_H
#define INODE_H

// return the # of inode which is available
// return -1 means there is an error
// return -2 means there is no available inode
int FindNextAvailableInodeBlock();
int ChangeInodeBitmapStatus(int inodeIndex, int status);
int ConvertBitmapToBytemap(char* bitmap, char* bytemap);
int ConvertBytemapToBitmap(char* bitmap, char* bytemap);


#endif // INODE_H
