#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "parameters.h"

typedef struct directoryEntry {
    int inodePointer;
    char pathName[16];
} DirectoryEntry;

int BuildRootDirectory();

// Break the path into detail up to "/" e.g: /ali/gholi/ --> "ali" , "gholi"
// return number of its
int BreakPathName(char* pathName , char **arrayOfBreakPathName);
int addDirectoryEntryOnSector(char* dataBlock, DirectoryEntry *dirEntry);
int addDirectoryEntry(int inodeNum, DirectoryEntry* dirEntry);
int addDirectory(char* pathName, char **arrayOfBreakPathName, int index);

int StringToInt (char* numberInStringType);
int searchPathInInode ( int inodeNumber , char* search , int* outputInodeNumber);

#endif // DIRECTORY_H
