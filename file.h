#ifndef FILE_H
#define FILE_H

typedef struct FileTableEntry
{
    int fileDescriptor;
    int inodePointer;
    int filePointer;
    int sizeOfFile;
    int isValid;
    char fileName[16];
    char filePath[256];
} FileTableEntry;


int addFile(int parentInode, char* fileName);
int CreateFileTable();


int getAvailabeFileDescriptor();

#endif // FILE_H
