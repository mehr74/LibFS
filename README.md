# LibFS API Application [![Build Status](https://api.travis-ci.org/mehr74/LibFS.png)](https://travis-ci.org/mehr74/LibFS)
In this project we created a user-level library, libFS, which handles an adequate portion of a file system. We had incorporated our file system into a library that applications can link with to access files and directories. Our library in turn hooks with LibDisk, a library that just mocks behaviors of a "disk".

## 1. Generic File System API

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

## 2. File Access API

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

---

**int File_Close(int fd)**

*File_Close()* closes the file referred to by file descriptor *fd*. 

* If the file is not currently open, return -1 and set osErrno to *E_BAD_FD*
* Upon success, return 0.

---

**int File_Unlink(char *file)**

This should delete the file referenced by file, including removing its name
from the directory it is in, and freeing up any data blocks and inodes that 
the file was using.
* If the file is currently open, return -1 and set *osErrno* to *E_FILE_IN_USE*
 (and do NOT delete the file).
* Upon success, return 0.

---

**int File_Seek(int fd, int offset)**

*File_Seek()* should update the current location of the file pointer. The 
location is given as an offset from the beginning of the file.

* If offset is larger than the size of the file or negative, return -1 and 
set *osErrno* to *E_SEEK_OUT_OF_BOUNDS*.
* If the file is not currently open, return -1 and set osErrno to 
*E_BAD_FD*.
* Upon success, return the new location of the file pointer.

## 3. Folder Access API

**int Dir_Create(char *path)**

*Dir_Create()* creates a new directory as named by path. To do so, we first 
allocate a new file (of type directory), and then add a new directory entry
in the current directory's parent.

* Upon failure of any sort, return -1 and set *osErrno* to *E_CREATE*
* Upon success, return 0.
* *Dir_Create()* is not recursive (that is, if only "/" exists, and we
want to create a directory "/a/b", we must first create "/a", and then
create "/a/b").

---

**int Dir_Size(char *path)**

*Dir_Size()* returns the number of bytes in the directory referred to by path.
This routine should be used to find the size of the directory before calling
*Dir_Read()* to find the contents of the directory.

---

**int Dir_Read(char *path, void *buffer, int size)**

*Dir_Read()* can be used to read the contents of a directory. It should return
in the buffer a set of directory entries. Each entry is of size 20 bytes, and 
contains 16-byte names of the directories and files within the directory name
by path, followed by the 4-byte integer inode number.

* If size is not big enough to contain all of the entries, return -1 and set 
*osErrno* to *E_BUFFER_TOO_SMALL*
* Return the number of directory entries that are in the directory (e.g., 2 
if there are two entries in the directory).

---

**int Dir_Unlink(char *path)**

*Dir_Unlink()* removes a directory referred to by path, freeing up its inode
and data blocks, and removing its entry from the parent directory. 

* Upon success, return 0
* Dir_Unlink() should only be successful if there are no files within the 
directory. If there are still files within the directory, return -1 and set 
*osErrno* to *E_DIR_NO_EMPTY*.
* If someone tries to remove the root directory("/"), don't allow them to do 
it! Return -1 and set *osErrno* to *E_ROOT_DIR*.
