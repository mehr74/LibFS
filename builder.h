#ifndef BUILDER_H
#define BUILDER_H

int CheckFileSystemSuperBlock();
int BuildMetadataBlocks();
int BuildSuperBlock();
int BuildBitmapBlocks();
int BuildDataBlock(char* dataBlock);
int BuildInode(char* inode, char type);

#endif  // BUILDER_H
