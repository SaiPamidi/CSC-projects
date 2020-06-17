#ifndef LIBDISK_H
#define LIBDISK_H
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BLOCKSIZE 256
#define DISKNUMS 1024

typedef struct DiskInfo
{
    FILE *diskPtr;
    char *filename;
    int diskNums;
    int blockNums;
    int open;
} DiskInfo;
DiskInfo *disks;
int diskNum;
int capacity;

int openDisk(char *filename, int nBytes);
int closeDisk(int disk);
int readBlock(int disk, int bNum, void *block);
int writeBlock(int disk, int bNum, void *block);

#endif
