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

---

**int FS_Sync()**

*FS_Sync()* ensures the contents of the file system are stored persistently on disk.

---

## File Access API

**int File_Create(char *file)**

*File_Create()* creates a new file.

* If the file already exists, function return -1 and set *osErrno* to *E_GENERAL*
* File is not going to be open after the create call. *File_Create* just simply creates a new file on disk of size 0.
* Upon success, return 0
* Upon a failure, return -1 and set *osErrno* to *E_CREATE*.

---

**int File_Open(char *file)**

*File_Open()* opens up a file and returns an integer file descriptor ( a
number greater than or equal to 0), which can be used to read or write data
to that file.
* If the file doesn't exist, return -1 and *osErrno* is set to 
*E_NO_SUCH_FILE*.
* if there are already a maximum number of files open, return -1 and set 
*osErrno* to *E_TOO_MANY_OPEN_FILES*.

---

**int File_Read(int fd, void *buffer, int size)**

*File_Read()* reads size bytes from the file referenced by the file descriptor
*fd*. The data should be read into the buffer pointed to by buffer. All reads
should begin at the current locatioin of the file pointer, and file pointer 
should be updated after the read to the new location.

* If the file is not open, return -1 and set *osErrno* to *E_BAD_FD*.
* If the file is open, the number of bytes actually read should be returned,
which can be less than or equal to size. (The number could be less than the
requested bytes because the end of the file could be reached.) 
* If the file pointer is already at the end of the file, zero should 
be returned, even under repeated calls to *File_Read()*

---

**int File_Write(int fd, void *buffer, int size)**

*File_Write()* writes size bytes from buffer and write thme into the file
referenced by *fd*. All writes should be updated after the write to its
current location plus size.

* Writes are the only way to extend the size of a file.
* If the file is not open, return -1 and set *osErrno* to *E_BAD_F*.
* Upon success of the write, all of the data should be written out to disk and the value of the size should be returned.
* If write cannot complete (due to a lack of space), return -1 and set *osErrno* to *E_NO_SPACE*. 
* If the file exceeds the maximum file size, return -1 and set *osErrno* to 
*E_FILE_TOO_BIG*
