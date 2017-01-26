#ifndef DIRECTORY_H
#define DIRECTORY_H

typedef struct directoryEntry {
    int inodePointer
    char pathName[PATH_LENGTH_MAX];
} DirectoryEntry;

int BuildRootDirectory();

// Break the path into detail up to "/" e.g: /ali/gholi/ --> "ali" , "gholi"
// return number of its
int BreakPathName( char* pathName , char* arrayOfBreakPathName[]);

int searchPathInInode ( int inodeNumber , char* search , int* outputInodeNumber);

#endif // DIRECTORY_H
