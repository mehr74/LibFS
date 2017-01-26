#ifndef DIRECTORY_H
#define DIRECTORY_H

int BuildRootDirectory();

// Break the path into detail up to "/" e.g: /ali/gholi/ --> "ali" , "gholi"
// return number of its
int BreakPathName( char* pathName , char* arrayOfBreakPathName[]);

#endif // DIRECTORY_H
