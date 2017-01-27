# LibFS API Application
In this project we created a user-level library, libFS, which handles an adequate portion of a file system. We had incorporated our file system into a library that applications can link with to access files and directories. Our library in turn hooks with LibDisk, a library that just mocks behaviors of a "disk".

## Generic File System API

**int FS_Boot(char *path)**

*FS_Boot()* should be called exactly once before any other LibFS functions are called.
It receives a single argument, a path, which either points to a real file where your
"disk image" is stored or to a file that does not yet exist and which must be created to
hold a new disk image. The path is which determined by Operating System. It's not the path
in the file system. We can store the disk image file anywhere.

* Upon success, return 0
* Upon failure, return -1 and set *osErrno* to *E_GENERAL*

**int FS_Sync()**

*FS_Sync()* ensures the contents of the file system are stored persistently on disk.


