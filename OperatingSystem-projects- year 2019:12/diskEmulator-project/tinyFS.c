#include "tinyFS.h"
#include "libDisk.h"
#include "libTinyFS.h"
#include "tinyFS_errno.h"
/* Makes a blank TinyFS file system of size nBytes on the unix file specified by ‘filename’.
This function should use the emulated disk library to open the specified unix file, and upon
success, format the file to be a mountable disk. This includes initializing all data to 0x00,
setting magic numbers, initializing and writing the superblock and inodes, etc. Must return a
specified success/error code. */
//int resourceCapacity = 1;
int curFd = 0;
int mountF = 1;
SysList resources;
int curDisk;
int tfs_mkfs(char *filename, int nBytes)
{
    block *superblock, *freeBlock;
    int disk, i, j, numBlocks;
    char buffer[BLOCKSIZE] = {0x00};

    // openning disk using libDisk.c
    disk = openDisk(filename, nBytes);
    if (disk < 0)
    {
        fprintf(stderr, "openDisk() failed to create a disk\n");
        return MKFS_FAILED;
    }

    numBlocks = nBytes / BLOCKSIZE;

    // Create superblock
    superblock = malloc(sizeof(block));
    superblock->BlockType |= 1;
    superblock->magicNum |= 0x44;
    superblock->next |= 0;
    superblock->empty |= 0;
    superblock->nextFree |= 1;

    // Create free block
    freeBlock = malloc(sizeof(block));
    freeBlock->BlockType |= 4;
    freeBlock->magicNum |= 0x44;
    freeBlock->next |= 0;
    freeBlock->empty |= 0;
    freeBlock->nextFree |= j;

    // write superblock in first block
    if (writeBlock(disk, 0, superblock) < 0)
    {
        return WRITEBLOCK_FAILED;
    }

    // write freeBlocks to rest of blocks
    for (i = 1, j = 2; i < numBlocks; i++, j++)
    {
        // last block's next free block is 0
        freeBlock->nextFree &= 0;
        if (i == numBlocks - 1)
        {
            j = 0;
        }
        freeBlock->nextFree |= j;
        if (writeBlock(disk, i, freeBlock) < 0)
        {
            return WRITEBLOCK_FAILED;
        }
    }

    return MKFS_SUCCESS;
}

/* tfs_mount(char *diskname) “mounts” a TinyFS file system located within ‘diskname’
unix file. tfs_unmount(void) “unmounts” the currently mounted file system. As part of the
mount operation, tfs_mount should verify the file system is the correct type. Only one file
system may be mounted at a time. Use tfs_unmount to cleanly unmount the currently mounted
file system. Must return a specified success/error code. */
int tfs_mount(char *diskname)
{
    int res;
    unsigned char *block = calloc(1, BLOCKSIZE);
    int i, j;
    if (mountF) //check if it is the only mounted fs
    {
        mountF = 0;
        for (i = 0; i <= diskNum - 1; i++) //diskNum is ahead of diskSize
        {
            if (disks[i].open) //checks if disk is actually open
            {
                if (!strcmp(diskname, disks[i].filename))
                {
                    for (j = 0; j < disks[i].blockNums; j++)
                    {
                        if (readBlock(disks[i].diskNums, j, block) < 0)
                        {
                            return READBLOCK_FAILED;
                        }
                        if (block[1] != 0x44)
                        {
                            free(block);
                            return MOUNT_FAILED;
                        }
                    }
                    curDisk = disks[i].diskNums;
                    free(block);
                    return MOUNT_SUCCESS;
                }
            }
        }
    }
    free(block);
    return MOUNT_FAILED;
}

int tfs_unmount(void)
{
    if (mountF)
    {
        printf("nothing has been mounted\n");
        return UNMOUNT_FAILED;
    }
    else
    {
        mountF = 1;
        return UNMOUNT_SUCCESS;
    }
}

/* Creates or Opens an existing file for reading and writing on the currently mounted file
system. Creates a dynamic resource table entry for the file, and returns a file descriptor
(integer) that can be used to reference this file while the filesystem is mounted. */
fileDescriptor tfs_openFile(char *name)
{
    int fd;
    fd = searchFile(name);
    if (fd >= 0)
    {
        return fd;
    }
    else
    {
        return (createFd(name));
    }
}
int createFd(char *name)
{
    SysResource *resource;
    SysResource *temp;
    temp = resources.head;
    resource = calloc(1, sizeof(SysResource));
    resource->fd = curFd;
    resource->inodeNum = writeToInode(name);
    resource->next = NULL;
    resource->filename = name;

    if (resources.head == NULL)
    {
        resources.head = resource;
    }
    else
    {
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = resource;
    }
    resources.size++;
    return curFd++;
}

int writeToInode(char *name)
{
    block *temp;
    int freeBlockNum;
    int nextFreeBlockNum;
    time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    time_str[strlen(time_str) - 1] = '\0';
    block *superblock = calloc(1, sizeof(block));
    InodeBlock *inode = calloc(1, sizeof(InodeBlock));
    InodeBlock *prevInode = calloc(1, sizeof(InodeBlock));

    if (readBlock(curDisk, 0, superblock) < 0)
    {
        return READBLOCK_FAILED;
    }
    freeBlockNum = superblock->nextFree;
    updateFreeBlock(freeBlockNum, superblock);
    inode->BlockType = 2;
    inode->magicNum = 0X44;
    inode->next = 0;
    inode->dataNext = 0;
    inode->size = 0;
    strcpy(inode->name, name);
    strcpy(inode->createTime, time_str);
    if (freeBlockNum != 0) //update the next for previous inode
    {
        inode->next = superblock->next;
    }
    superblock->next = freeBlockNum;

    if (writeBlock(curDisk, 0, superblock) < 0)
    {
        return WRITEBLOCK_FAILED;
    }

    if (writeBlock(curDisk, freeBlockNum, inode) < 0)
    {
        return WRITEBLOCK_FAILED;
    }
    return freeBlockNum;
    //read cur disk super block
    //get free block and assign next free block to super block
    //make into inode
    //get previous inode and make changes to the next of this inode
}

void updateFreeBlock(int bnum, block *superblock)
{
    block *freeblock = calloc(1, sizeof(block));
    int nextFree;
    //printf("check %d", bnum);
    if (readBlock(curDisk, bnum, freeblock) < 0) //read the free block to find next free block
    {
        fprintf(stderr, "read block failed");
        //return READBLOCK_FAILED;
    }
    nextFree = (freeblock->nextFree);
    (superblock->nextFree) = nextFree;
    //printf("what is free %d\n", nextFree);
}
int searchFile(char *name)
{
    SysResource *temp = resources.head;
    while (temp != NULL)
    {
        if (!strcmp(name, temp->filename))
        {
            return temp->fd;
        }
        temp = (temp->next);
    }
    return -1;
}

/* Closes the file, de-allocates all system/disk resources, and removes table entry */
int tfs_closeFile(fileDescriptor FD)
{
    SysResource *temp = resources.head;
    SysResource *temp1 = NULL;
    if (temp->fd == FD)
    {
        resources.head = temp->next;
        free(temp);
        resources.size += -1;
        return FD;
    }
    while (temp->next != NULL)
    {
        if ((temp->next)->fd == FD)
        {
            temp1 = temp->next;
            temp->next = (temp->next)->next;
            free(temp1);
            resources.size += -1;
            return FD;
        }
    }
    return CLOSEFILE_FAILED;
}

SysResource *searchFD(fileDescriptor FD)
{
    SysResource *temp = resources.head;
    while (temp != NULL)
    {
        if (FD == temp->fd)
        {
            return temp;
        }
        temp = (temp->next);
    }
    return NULL;
}

/* Writes buffer ‘buffer’ of size ‘size’, which represents an entire file’s content, to the file system.
Previous content (if any) will be completely lost. Sets the file pointer to 0 (the start of file) when done.
Returns success/error codes. */
int tfs_writeFile(fileDescriptor FD, char *buffer, int size)
{
    SysResource *file;
    int written = 0;
    int nextBlock; // used for next write block
    int curBlock;  // used for current write block
    file = searchFD(FD);
    InodeBlock *inode = calloc(1, BLOCKSIZE);
    fileExtentBlock *fextent = calloc(1, BLOCKSIZE);
    fileExtentBlock *temp = calloc(1, BLOCKSIZE);
    block *superblock = calloc(1, BLOCKSIZE);
    block *freeB = calloc(1, BLOCKSIZE);
    time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    time_str[strlen(time_str) - 1] = '\0';

    if (file != NULL && file->fd == FD)
    {
        // check if it already has allocated file extent block
        if (readBlock(curDisk, file->inodeNum, inode) < 0)
        {
            fprintf(stderr, "Error reading block\n");
            return -1;
        }

        // get superblock
        if (readBlock(curDisk, 0, superblock) < 0)
        {
            fprintf(stderr, "Error reading block\n");
            return -1;
        }

        fextent->BlockType = 3;
        fextent->magicNum = 0x44;

        // inode block has a file extent block
        if (inode->dataNext != 0)
        {
            curBlock = inode->dataNext;
            // printf("inode had previous data %d\n", curBlock);
        }
        // inode block doesn't have file extent block
        else
        {
            curBlock = superblock->nextFree;

            updateFreeBlock(curBlock, superblock);
            // printf("1 next free %d \n", superblock->nextFree);
            if (writeBlock(curDisk, 0, superblock) < 0)
            {
                fprintf(stderr, "Error writing block\n");
                return WRITEBLOCK_FAILED;
            }
        }

        inode->dataNext = curBlock;
        inode->size = size;
        strcpy(inode->modificTime, time_str);
        // write new data next to inode block
        if (writeBlock(curDisk, file->inodeNum, inode) < 0)
        {
            fprintf(stderr, "Error reading block\n");
            return WRITEBLOCK_FAILED;
        }

        // copy data into data block to be written
        strncpy((char *)fextent->data, buffer, DATASIZE);

        // prepare loop
        if (readBlock(curDisk, curBlock, temp) < 0)
        {
            fprintf(stderr, "Error reading block\n");
            return READBLOCK_FAILED;
        }

        if (writeBlock(curDisk, curBlock, fextent) < 0)
        {
            fprintf(stderr, "Error reading block\n");
            return -1;
        }
        written = written + DATASIZE;

        // begin loop to write data until written > size
        while (written < size)
        {
            // copy new data into data block
            strncpy((char *)fextent->data, buffer + written, DATASIZE);
            // printf("Next block type: %d Next block->next: %d\n", temp->BlockType, temp->next);

            // if the previously saved block was a data block and had a next, use it
            if (temp->BlockType == 3 && temp->next != 0)
            {
                fextent->next = temp->next;
                nextBlock = fextent->next;
            }
            // otherwise grab a new free block
            else if (written < size)
            {
                nextBlock = superblock->nextFree;
                fextent->next = nextBlock;
                updateFreeBlock(nextBlock, superblock);

                // write updated super block
                if (writeBlock(curDisk, 0, superblock) < 0)
                {
                    fprintf(stderr, "Error writing block\n");
                    return WRITEBLOCK_FAILED;
                }
            }

            // read next block for next value
            if (readBlock(curDisk, nextBlock, temp) < 0)
            {
                fprintf(stderr, "Error reading block\n");
                return READBLOCK_FAILED;
            }

            // write in data block for prevBlock
            if (writeBlock(curDisk, curBlock, fextent) < 0)
            {
                fprintf(stderr, "Error reading block\n");
                return WRITEBLOCK_FAILED;
            }
            curBlock = nextBlock;
            written = written + DATASIZE;
        }
        // printf("written %d/ size %d  \n",written, size );

        // delete the rest of the file
        freeB->BlockType |= 4;
        freeB->magicNum |= 0x44;
        freeB->next |= 1;

        curBlock = temp->next;
        while (curBlock != 0)
        {
            addFreeBlock(curBlock);
            if (readBlock(curDisk, curBlock, temp) < 0)
            {
                fprintf(stderr, "Error reading block\n");
                return READBLOCK_FAILED;
            }
            if (writeBlock(curDisk, curBlock, freeB) < 0)
            {
                fprintf(stderr, "Error reading block\n");
                return WRITEBLOCK_FAILED;
            }
            curBlock = temp->next;
        }
    }
    else
    {
        return WRITEFILE_FAILED;
    }

    if (writeBlock(curDisk, 0, superblock) < 0)
    {
        printf("write block gone wrong");
        return WRITEBLOCK_FAILED;
    }
    file->filePtr = 0;
    return WRITEFILE_SUCCESS;
}

int addFreeBlock(int blockNum)
{
    block *s;
    int next;
    s = calloc(1, BLOCKSIZE);

    if (readBlock(curDisk, 0, s) < 0)
    {
        fprintf(stderr, "Error reading block\n");
        return READBLOCK_FAILED;
    }

    next = s->nextFree;
    while (next != 0)
    {
        if (readBlock(curDisk, next, s) < 0)
        {
            fprintf(stderr, "Error reading block\n");
        }

        next = s->nextFree;
    }
    s->nextFree = blockNum;
    free(s);
    return 0;
}

int updateInode(char *filename)
{
    InodeBlock *s, *prev, *temp1, *temp2;
    block *superblock;
    int s_num, p_num;
    s = calloc(1, BLOCKSIZE);
    prev = calloc(1, BLOCKSIZE);
    superblock = calloc(1, BLOCKSIZE);
    temp1 = s;
    temp2 = prev;

    if (readBlock(curDisk, 0, superblock) < 0)
    {
        fprintf(stderr, "Error reading block\n");
        free(temp1);
        free(temp2);
        free(superblock);
        return -1;
    }

    s_num = superblock->next;

    while (strcmp(s->name, filename) && s_num != 0)
    {
        p_num = s_num;
        s_num = s->next;

        // printf("previous %d next %d\n",p_num, s_num );

        if (readBlock(curDisk, s_num, s) < 0)
        {
            fprintf(stderr, "Error reading block\n");
            free(temp1);
            free(temp2);
            free(superblock);
            return -1;
        }

        if (readBlock(curDisk, p_num, prev) < 0)
        {
            fprintf(stderr, "Error reading block\n");
            free(temp1);
            free(temp2);
            free(superblock);
            return -1;
        }
    }
    prev->next = s->next;

    if (writeBlock(curDisk, p_num, prev) < 0)
    {
        fprintf(stderr, "Error writing block\n");
        return -1;
    }

    free(temp1);
    free(temp2);
    free(superblock);
    return 0;
}

/* deletes a file and marks its blocks as free on disk. */
int tfs_deleteFile(fileDescriptor FD)
{
    SysResource *file = NULL;
    InodeBlock *inode = calloc(1, BLOCKSIZE);
    fileExtentBlock *fextent = calloc(1, BLOCKSIZE);
    block *freeB = calloc(1, BLOCKSIZE);
    int fblock, iblock;

    freeB->BlockType = 4;
    freeB->magicNum = 0x44;
    freeB->next = 0;
    file = searchFD(FD);

    if (file != NULL && file->fd == FD)
    {

        iblock = file->inodeNum;
        // delete inode from disk

        // printf("iNodeNum%d\n",iblock );

        if (readBlock(curDisk, iblock, inode) < 0)
        {
            fprintf(stderr, "Error reading block\n");
            return READBLOCK_FAILED;
        }

        // delete the file if it exists on the disk
        if (inode->BlockType == 2)
        {
            updateInode(file->filename);
            if (writeBlock(curDisk, iblock, freeB) < 0)
            {
                fprintf(stderr, "Error reading block\n");
                free(inode);
                free(fextent);
                free(freeB);
                return WRITEBLOCK_FAILED;
            }
            else
            {
                printf("Successfully deleted %d\n", iblock);
            }
            addFreeBlock(iblock);

            // delete file extent blocks if they exist
            if (inode->dataNext != 0)
            {
                fblock = inode->dataNext;
                if (readBlock(curDisk, fblock, fextent) < 0)
                {
                    fprintf(stderr, "Error reading block\n");
                    free(inode);
                    free(fextent);
                    free(freeB);
                    return READBLOCK_FAILED;
                }

                while (fblock != 0 && fextent->BlockType == 3)
                {
                    if (readBlock(curDisk, fblock, fextent) < 0)
                    {
                        fprintf(stderr, "Error reading block\n");
                        free(inode);
                        free(fextent);
                        free(freeB);
                        return READBLOCK_FAILED;
                    }
                    if (writeBlock(curDisk, fblock, freeB) < 0)
                    {
                        fprintf(stderr, "Error writing block\n");
                        free(inode);
                        free(fextent);
                        free(freeB);
                        return WRITEFILE_FAILED;
                    }
                    fblock = fextent->next;
                }
            }
        }
        free(inode);
        free(fextent);
        free(freeB);
        return DELETEFILE_SUCCESS;
    }
    free(inode);
    free(fextent);
    free(freeB);

    return DELETEFILE_FAILED;
}

/* reads one byte from the file and copies it to buffer, using the current file pointer location and
incrementing it by one upon success. If the file pointer is already past the end of the file then
tfs_readByte() should return an error and not increment the file pointer. */
int tfs_readByte(fileDescriptor FD, char *buffer)
{
    // get SysResource from resource linked list
    SysResource *file;
    InodeBlock *inode;
    fileExtentBlock *fextent;
    int blocks = 0, curBlock;
    time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    time_str[strlen(time_str) - 1] = '\0';

    file = searchFD(FD);
    inode = calloc(1, BLOCKSIZE);
    fextent = calloc(1, BLOCKSIZE);

    // if it exists in resources
    if (file != NULL && file->fd == FD)
    {
        //get size from inode in disk
        if (readBlock(curDisk, file->inodeNum, inode) < 0)
        {
            fprintf(stderr, "Error reading block\n");
            return -1;
        }
        strcpy(inode->accessTime, time_str);
        if (writeBlock(curDisk, file->inodeNum, inode) < 0)
        {
            fprintf(stderr, "Error writing block\n");
            return -1;
        }
        // check to see if filePtr > size, return error
        if (file->filePtr > inode->size)
        {
            //printf("inode size %d\n", inode->size);
            free(inode);
            free(fextent);
            return OUT_OF_BOUNDS;
        }
        // read byte if filePtr is within bounds
        else
        {

            curBlock = inode->dataNext;
            while (blocks < file->filePtr / DATASIZE)
            {
                curBlock = fextent->next;
                blocks++;
            }
            if (readBlock(curDisk, curBlock, fextent) < 0)
            {
                fprintf(stderr, "Error reading block\n");
                free(inode);
                free(fextent);
                return READBLOCK_FAILED;
            }
            *buffer = fextent->data[((file->filePtr) % DATASIZE)];
            (file->filePtr) += 1;
            //printf("%d\n", file->filePtr);
            free(inode);
            free(fextent);
            return READB_SUCCESS;
        }
    }
    return FILE_NOT_FOUND;
}

/* change the file pointer location to offset (absolute). Returns success/error codes.*/
int tfs_seek(fileDescriptor FD, int offset)
{
    SysResource *file;
    file = searchFD(FD);

    if (file != NULL && file->fd == FD)
    {
        file->filePtr = offset;
        return SEEK_SUCCESS;
    }
    else
    {
        return SEEK_FAILURE;
    }
}
int tfs_rename(fileDescriptor FD, char *newName)
{
    SysResource *file;
    InodeBlock *inode;
    block *superblock;
    file = searchFD(FD);
    inode = calloc(1, BLOCKSIZE);
    int curInode;

    if (file != NULL && file->fd == FD)
    {

        if (readBlock(curDisk, file->inodeNum, inode) < 0)
        {
            return READBLOCK_FAILED;
        }
        strcpy(inode->name, newName);
        if (writeBlock(curDisk, file->inodeNum, inode) < 0)
        {
            return WRITEBLOCK_FAILED;
        }
        return RENAME_SUCCESS;
    }
    else
    {
        return FILE_NOT_FOUND;
    }
}
void tfs_readdir()
{
    //printf("in here\n");
    InodeBlock *inode;
    block *superblock;
    inode = calloc(1, BLOCKSIZE);
    superblock = calloc(1, BLOCKSIZE);
    int curInode;
    if (readBlock(curDisk, 0, superblock) < 0)
    {
        fprintf(stderr, "read block failed");
    }
    curInode = superblock->next;
    while (curInode != 0)
    {
        if (readBlock(curDisk, curInode, inode) < 0)
        {
            fprintf(stderr, "read block failed");
        }
        printf("fileName : %s \n", inode->name);
        curInode = inode->next;
    }
}
int tfs_readFileInfo(fileDescriptor FD)
{
    SysResource *file;
    InodeBlock *inode;
    file = searchFD(FD);
    inode = calloc(1, BLOCKSIZE);
    if (file != NULL)
    {
        if (readBlock(curDisk, file->inodeNum, inode) < 0)
        {
            return READBLOCK_FAILED;
        }
        printf("creation time : %s\n", inode->createTime);
        printf("modification time : %s\n", inode->createTime);
        printf("access time : %s\n", inode->accessTime);
    }
    else
    {
        return FILE_NOT_FOUND;
    }
    return INFO_SUCCESS;
}