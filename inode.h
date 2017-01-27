#ifndef INODE_H
#define INODE_H

// return the # of inode which is available
// return -1 means there is an error
// return -2 means there is no available inode
int FindNextAvailableInodeBlock();
int FindNextAvailableDataBlock();

int ChangeInodeBitmapStatus(int inodeIndex, int status);
int ChangeDataBitmapStatus(int dataIndex, int status);

//Convert Bitmap to Bytemap and vice versa
int ConvertBitmapToBytemap(char* bitmap, char* bytemap);
int ConvertBytemapToBitmap(char* bitmap, char* bytemap);

// Place an inode in correct block
// inodeData must be 128 Byte
int WriteInodeInSector ( int inodeNumber , char* inodeData);

int isDirectoryInode (char *inode);


#endif // INODE_H
