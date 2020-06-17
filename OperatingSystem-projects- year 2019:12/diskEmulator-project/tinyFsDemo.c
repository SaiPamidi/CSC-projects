#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tinyFS.h"
#include "libTinyFS.h"
#include "tinyFS_errno.h"
#include <time.h>

/* simple helper function to fill Buffer with as many inPhrase strings as possible before reaching size */
int fillBufferWithPhrase(char *inPhrase, char *Buffer, int size)
{
  int index = 0, i;
  if (!inPhrase || !Buffer || size <= 0 || size < strlen(inPhrase))
    return -1;

  while (index < size)
  {
    for (i = 0; inPhrase[i] != '\0' && (i + index < size); i++)
      Buffer[i + index] = inPhrase[i];
    index += i;
  }
  Buffer[size - 1] = '\0'; /* explicit null termination */
  return 0;
}

int main(int argc, char const *argv[])
{
  int aFD;
  int bFD;
  int afileSize = 200;
  int bfileSize = 1000;
  char *afileContent, *bfileContent; /* buffers to store file content */
  char phrase1[] = "hello world from (a) file ";
  char phrase2[] = "(b) file content ";
  char buffer;

  tfs_mkfs(DEFAULT_DISK_NAME, DEFAULT_DISK_SIZE); /* then make a new disk */
  tfs_mount(DEFAULT_DISK_NAME);

  aFD = tfs_openFile("afile"); /* open a file*/

  bFD = tfs_openFile("yoyo"); /* open a file*/

  tfs_openFile("ygoh"); /* open another file */

  afileContent = (char *)malloc(afileSize * sizeof(char)); /* fill file with content*/
  bfileContent = (char *)malloc(bfileSize * sizeof(char));

  tfs_readdir(); /* list existing directories */

  if (fillBufferWithPhrase(phrase1, afileContent, afileSize) < 0)
  {
    perror("failed");
    return -1;
  }

  if (fillBufferWithPhrase(phrase2, bfileContent, bfileSize) < 0)
  {
    perror("failed");
    return -1;
  }

  if (tfs_writeFile(aFD, afileContent, afileSize) < 0) /* write to afile */
  {
    perror("tfs_writeFile failed");
  }
  else
  {
    printf("Successfully written to afile\n\n");
  }

  if (tfs_writeFile(aFD, afileContent, afileSize) < 0) /* rewrite to aflie */
  {
    perror("tfs_writeFile failed");
  }
  else
  {
    printf("Successfully written to afile\n\n");
  }

  if (tfs_writeFile(bFD, bfileContent, bfileSize) < 0) /* write to bfile */
  {
    perror("tfs_writeFile failed");
  }
  else
  {
    printf("Successfully written to bfile\n\n");
  }

  if (tfs_writeFile(bFD, bfileContent, bfileSize) < 0) /* overwrite to bfile */
  {
    perror("tfs_writeFile failed");
  }
  else
  {
    printf("Successfully written to bfile\n\n");
  }
  //tfs_deleteFile(aFD);
  if (tfs_writeFile(bFD, bfileContent, 200) < 0) /* overwrite bfile with a smaller content*/
  {
    perror("tfs_writeFile failed");
  }
  else
  {
    printf("Successfully written to bfile\n\n");
  }

  if (tfs_writeFile(bFD, bfileContent, bfileSize) < 0) /* overwrite bfile with bigger content*/
  {
    perror("tfs_writeFile failed");
  }
  else
  {
    printf("Successfully written to bfile\n\n");
  }

  if (tfs_readByte(aFD, &buffer) < 0) /* read the first byte of afile */
  {
    printf("read byte failed\n");
  }
  printf("byte from b:%c\n", buffer);
  if (tfs_seek(aFD, 2) < 0) /* seek to the third byte afile */
  {
    printf("seek failed\n");
  }
  tfs_readByte(aFD, &buffer); /* read the third byte of afile */
  printf("byte from b:%c\n", buffer);
  tfs_readByte(bFD, &buffer);
  if (tfs_rename(bFD, "cfile") < 0) /* rename bfile file*/
  {
    printf("renaming failed\n");
  }
  if (tfs_deleteFile(aFD) < 0) /* delete afile */
  {
    printf("deletion failed\n");
  }
  if (tfs_readFileInfo(bFD) < 0) /* get different access times of the file */
  {
    printf("getting file info failed\n");
  }
  if (tfs_closeFile(bFD) < 0)
  {
    printf("closing file failed");
  }
  if (tfs_unmount() < 0)
  {
    printf("unmounting failed");
  }
  return 0;
}
