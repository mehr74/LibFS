#ifndef BUILDER_H
#define BUILDER_H

int BuildMetadataBlocks();
int BuildSuperBlock();
int BuildBitmapBlocks();
int BuildDataBlock(char* dataBlock);
int BuildInode(char* inode);

#endif  // BUILDER_H
