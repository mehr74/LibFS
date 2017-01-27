#ifndef FILE_H
#define FILE_H

typedef struct FileTableEntry
{
    int fileDescriptor;
    int inodePointer;
    int filePointer;
    int sizeOfFile;
    char fileName[16];
    char filePath[256];
} FileTableEntry;


int addFile(int parentInode, char* fileName);

int initializeFileTableEntryByInode(int inodeNum, FileTableEntry *fileTableEntry);
int getAvailabeFileDescriptor();
int getFileTableEntry(int fileDescriptor, FileTableEntry *fileTableEntry);

#endif // FILE_H
