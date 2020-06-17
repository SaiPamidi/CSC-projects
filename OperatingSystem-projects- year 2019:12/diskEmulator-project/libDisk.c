#include "libDisk.h"
#include "tinyFS_errno.h"
int diskNum = 0;
int capacity = 1;
/* This function opens a regular UNIX file and designates the first nBytes of it as space for the emulated disk.
If nBytes is not exactly a multiple of BLOCKSIZE then the disk size will be the closest multiple of BLOCKSIZE
that is lower than nBytes (but greater than 0) If nBytes is less than BLOCKSIZE failure should be returned.
If nBytes > BLOCKSIZE and there is already a file by the given filename, that file’s content may be overwritten.
If nBytes is 0, an existing disk is opened, and the content must not be overwritten in this function.
There is no requirement to maintain integrity of any file content beyond nBytes.
The return value is negative on failure or a disk number on success. */
int openDisk(char *filename, int nBytes)
{
	FILE *diskFile;
	int numBlocks;
	if (nBytes < BLOCKSIZE)
	{
		return -1;
	}
	numBlocks = nBytes / BLOCKSIZE;
	if (nBytes > 0)
	{
		diskFile = fopen(filename, "w+");
	}
	else
	{
		diskFile = fopen(filename, "a+");
	}
	if (diskNum == 0)
	{
		disks = malloc(capacity * sizeof(DiskInfo));
	}
	else if (diskNum == capacity)
	{
		capacity = capacity * 2;
		disks = realloc(disks, capacity * sizeof(DiskInfo));
	}
	disks[diskNum].diskPtr = diskFile;
	disks[diskNum].open = 1;
	disks[diskNum].filename = filename;
	disks[diskNum].diskNums = diskNum;
	disks[diskNum].blockNums = numBlocks;
	return diskNum++;
}

/* self explanatory */
int closeDisk(int disk)
{
	if (disks[disk].open == 0)
	{
		fclose(disks[disk].diskPtr);
	}
	else
	{
		fprintf(stderr, "disk is not open\n");
	}
	return disk; //not sure what to return
}

/* readBlock() reads an entire block of BLOCKSIZE bytes from the open disk (identified by ‘disk’) and copies
the result into a local buffer (must be at least of BLOCKSIZE bytes). The bNum is a logical block number,
which must be translated into a byte offset within the disk. The translation from logical to physical block is
straightforward: bNum=0 is the very first byte of the file. bNum=1 is BLOCKSIZE bytes into the disk, bNum=n
is n*BLOCKSIZE bytes into the disk. On success, it returns 0. -1 or smaller is returned if disk is not available
(hasn’t been opened) or any other failures. You must define your own error code system. */
int readBlock(int disk, int bNum, void *block)
{
	int res = 0;
	if (disks[disk].open == 0)
	{
		return READBLOCK_FAILED;
	}
	else
	{
		fseek(disks[disk].diskPtr, bNum * BLOCKSIZE, SEEK_SET);
		if ((res = fread(block, 1, BLOCKSIZE, disks[disk].diskPtr) != BLOCKSIZE))
		{
			fprintf(stderr, "Error Reading");
			return READBLOCK_FAILED;
		}
		/*fread(block, 1, BLOCKSIZE * bNum, disks[disk].diskPtr);*/
	}
	return READBLOCK_SUCCESS;
}

/* writeBlock() takes disk number ‘disk’ and logical block number ‘bNum’ and writes the content of the buffer
‘block’ to that location. ‘block’ must be integral with BLOCKSIZE. Just as in readBlock(), writeBlock() must
translate the logical block bNum to the correct byte position in the file. On success, it returns 0. -1 or smaller
is returned if disk is not available (i.e. hasn’t been opened) or any other failures. You must define your own error
code system. */
int writeBlock(int disk, int bNum, void *block)
{
	int res = 0;
	if (disks[disk].open == 0)
	{
		return WRITEBLOCK_FAILED;
	}
	else
	{
		fseek(disks[disk].diskPtr, bNum * BLOCKSIZE, SEEK_SET);
		if ((res = fwrite(block, 1, BLOCKSIZE, disks[disk].diskPtr) != BLOCKSIZE))
		{
			fprintf(stderr, "Error Writing");
		}
		/*fwrite(block, 1, BLOCKSIZE * bNum, disks[disk].diskPtr);*/
	}
	return WRITEBLOCK_SUCCESS;
}
