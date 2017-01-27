#ifndef BUILDER_H
#define BUILDER_H

int CheckFileSystemSuperBlock();
int BuildMetadataBlocks();
int BuildSuperBlock();
int BuildBitmapBlocks();
int BuildDataBlock(char* dataBlock);
int BuildInode(char* inode, char type);
int InitializeDirectoryInode(char* inode, int dataSectorNum);
int InitializeDirectoryFirstDataBlock(char* dataBlock, int parentInode, int myInode);
void printBlockHex(char* block, int size);

#endif  // BUILDER_H
