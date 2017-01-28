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

int SizeOfFile(int inodeNumber);
int DataBlocksOccupiedByFile(int inodeNumber,int* sectorNumbers);

int openFileDescriptor(char *path);
int isFileOpen(int fd);

int removeFileTableEntry(int fd);
int getInodePointerOfFileEntry(int fd);
int updateFilePointerOfFileEntry(int fd, int filePointer);

int FileRead(int fd, char *buffer, int size);
int WriteFile(int fd, char* buffer , int size);

void printFileTableEntry(int fd);
void printFileTable();
#endif // FILE_H
