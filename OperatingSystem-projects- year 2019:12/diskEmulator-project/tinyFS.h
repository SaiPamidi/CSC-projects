#ifndef TINYFS_H
#define TINYFS_H
/* The default size of the disk and file system block */
#define BLOCKSIZE 256
/* Your program should use a 10240 Byte disk size giving you 40 blocks total. This is a default size. You must be able to support different possible values */
#define DEFAULT_DISK_SIZE 10240
/* use this name for a default disk file name */
#define DEFAULT_DISK_NAME "tinyFSDisk"
typedef int fileDescriptor;
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BLOCKSIZE 256
#define DATASIZE 252
#define DEFAULT_DISK_SIZE 10240
typedef int fileDescriptor;

/* Block Types
    1: superblock
    2: inode
    3: file extent
    4: free
*/
typedef struct block
{
    unsigned char BlockType;
    unsigned char magicNum;
    unsigned char next;
    unsigned char empty;
    unsigned char nextFree;
    unsigned char data[251];
} block;
typedef struct InodeBlock
{
    unsigned char BlockType;
    unsigned char magicNum;
    unsigned char next;
    unsigned char empty;
    unsigned char dataNext;
    char name[8];
    int size;
    char createTime[60];
    char modificTime[60];
    char accessTime[60];
    unsigned char data[59];
} InodeBlock;
typedef struct fileExtentBlock
{
    unsigned char BlockType;
    unsigned char magicNum;
    unsigned char next;
    unsigned char empty;
    unsigned char data[DATASIZE];
} fileExtentBlock;
typedef struct node
{
    struct node *next;
    char *filename;
    char fileSize;
} node;
typedef struct SysResource
{
    char *filename;
    int fd;
    int inodeNum;
    int filePtr;
    struct SysResource *next;
} SysResource;
typedef struct SysList
{
    SysResource *head;
    int size;
} SysList;
int tfs_mkfs(char *filename, int nBytes);
int tfs_mount(char *diskname);
int tfs_unmount(void);
fileDescriptor tfs_openFile(char *name);
int tfs_closeFile(fileDescriptor FD);
int tfs_writeFile(fileDescriptor FD, char *buffer, int size);
int tfs_deleteFile(fileDescriptor FD);
int tfs_readByte(fileDescriptor FD, char *buffer);
int tfs_seek(fileDescriptor FD, int offset);
int createFd(char *name);
int writeToInode(char *name);
int searchFile(char *name);
void updateFreeBlock(int bnum, block *);
int addFreeBlock(int blockNum);
SysResource *searchFD(fileDescriptor FD);
int updateInode(char *filename);
int tfs_rename(fileDescriptor FD, char *newName);
void tfs_readdir();
int tfs_readFileInfo(fileDescriptor FD);
#endif
