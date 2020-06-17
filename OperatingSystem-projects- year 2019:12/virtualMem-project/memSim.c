#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include <unistd.h>
#define tlbSize 16
#define ptSize 256
typedef struct Stats
{
    int numAddr;
    int pageFaults;
    int tlbHits;
} Stats;
typedef struct PageTableUnit
{
    int frameNo;
    unsigned int loadBit;
} PageTableUnit;
typedef struct TLBUnit
{
    int pageNo;
    int frameNo;
    char valid;
} TLBUnit;
typedef struct allMem
{
    PageTableUnit *pageTable;
    void *physicalMem;
    TLBUnit *tlb;
    int tlbHead;
    unsigned long phySpace;
    char *alg;
    Node *optCurr;
} allMem;
void getData(allMem *, unsigned int, unsigned int, int, Queue *, Stats *);
int checkPageTable(allMem *, unsigned int, unsigned int, int, Queue *, Stats *);
int pageFault(allMem *, unsigned int, Queue *, int);
int checkTlb(allMem *, unsigned int);
unsigned int getReplacement(Queue *, allMem *);
void getFrombackup(allMem *, void *, unsigned int);
void printStats(Stats);
void printFrames(unsigned char *);
void lruDel(Queue *, int);
void updateTlb(allMem *, int, int);
int findOptChoice(allMem *, Queue *);
int findDist(Node *, int);
int main(int argc, char *argv[])
{
    unsigned int logicAddr;
    unsigned int pageNo = 0;
    unsigned int pageOffset = 0;
    unsigned int offsetMask = 0xff;
    unsigned int pageMask = 0xff00;
    void *physicalMem;
    TLBUnit *tlb;
    Stats stats;
    allMem mem;
    Queue queue;
    PageTableUnit *pageTable;
    Node *temp;
    int numFrames;
    if (argc < 2 || argc > 4)
    {
        fprintf(stderr, "invalid number of arguments\n");
        exit(0);
    }
    if (argc > 2)
    {
        numFrames = strtol(argv[2], NULL, 10);
    }
    else
    {
        numFrames = 256;
    }

    pageTable = (PageTableUnit *)calloc(ptSize, sizeof(PageTableUnit));
    if (pageTable == NULL)
    {
        printf("malloc failed!");
        perror(NULL);
    }
    physicalMem = calloc(numFrames, 256);
    if (physicalMem == NULL)
    {
        printf("malloc failed!");
        perror(NULL);
    }
    tlb = (TLBUnit *)calloc(tlbSize, sizeof(TLBUnit));
    if (tlb == NULL)
    {
        printf("malloc failed!");
        perror(NULL);
    }
    if (argc <= 3)
    {
        mem.alg = "fifo";
    }
    else if (argc == 4)
    {
        mem.alg = argv[3];
    }
    queue.head = NULL;
    queue.tail = NULL;
    queue.size = 0;
    mem.pageTable = pageTable;
    mem.physicalMem = physicalMem;
    mem.tlb = tlb;
    mem.tlbHead = 0;
    mem.phySpace = 0;
    stats.numAddr = 0;
    stats.pageFaults = 0;
    stats.tlbHits = 0;
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf(" file not opening !");
        perror(NULL);
    }
    if (strcmp("opt", mem.alg) == 0)
    {
        while (fscanf(fp, "%u", &logicAddr) != EOF)
        {
            pageNo = logicAddr & pageMask;
            pageNo = pageNo >> 8;
            enqueue(&queue, pageNo);
        }
        mem.optCurr = queue.head;
        fseek(fp, 0, SEEK_SET);
    }
    while (fscanf(fp, "%u", &logicAddr) != EOF)
    {
        printf("%d, ", logicAddr);
        pageOffset = logicAddr & offsetMask;
        pageNo = logicAddr & pageMask;
        pageNo = pageNo >> 8;
        //printf("pageNo %u pageOffset %u ", pageNo, pageOffset);
        (stats.numAddr) += 1;
        getData(&mem, pageNo, pageOffset, numFrames, &queue, &stats);
    }
    printStats(stats);
    while (queue.head != NULL)
    {
        dequeue(&queue);
    }
    free(mem.tlb);
    free(mem.pageTable);
    free(mem.physicalMem);
    fclose(fp);
    return 0;
}
void getData(allMem *mem, unsigned int pageNo, unsigned int pageOffset, int numFrames, Queue *queue, Stats *stats)
{

    int frameNo;
    void *physicalAddress;
    if (strcmp("fifo", mem->alg) != 0)
    {
        if (strcmp("lru", mem->alg) == 0)
        {
            enqueue(queue, pageNo);
            lruDel(queue, pageNo);
        }
        else if (strcmp("opt", mem->alg) == 0)
        {
            mem->optCurr = (mem->optCurr)->next;
        }
    }
    if ((frameNo = checkTlb(mem, pageNo)) >= 0) //tlb hit
    {
        stats->tlbHits += 1;
        physicalAddress = (frameNo << 8) + mem->physicalMem + pageOffset;
        printf("%d, ", *((char *)(physicalAddress)));
        printf("%d, ", frameNo);
        printFrames((frameNo << 8) + mem->physicalMem);
        printf("\n");
    }
    else
    {
        frameNo = checkPageTable(mem, pageNo, pageOffset, numFrames, queue, stats);
        (mem->tlb)[mem->tlbHead].pageNo = pageNo;
        (mem->tlb)[mem->tlbHead].frameNo = frameNo;
        (mem->tlb)[mem->tlbHead].valid = 1;
        mem->tlbHead = (mem->tlbHead) + 1;
        if (mem->tlbHead >= 16)
        {
            mem->tlbHead = 0;
        }
    }
}
int checkTlb(allMem *mem, unsigned int pageNo)
{
    int i = 0;
    for (i = 0; i < 16; i++)
    {
        if ((mem->tlb)[i].valid)
        {
            if (pageNo == (mem->tlb)[i].pageNo)
            {
                return (mem->tlb)[i].frameNo;
            }
        }
    }
    return -1;
}
int checkPageTable(allMem *mem, unsigned int pageNo, unsigned int pageOffset, int numFrames, Queue *queue, Stats *stats)
{
    int frameNo;
    void *physicalAddress;
    if ((mem->pageTable)[pageNo].loadBit == 1)
    {
        frameNo = (mem->pageTable)[pageNo].frameNo;
        physicalAddress = (frameNo << 8) + mem->physicalMem + pageOffset;
        printf("%d, ", *((char *)(physicalAddress)));
        printf("%d, ", frameNo);
        printFrames((frameNo << 8) + mem->physicalMem);
        printf("\n");
    }
    else
    {
        stats->pageFaults += 1;
        frameNo = pageFault(mem, pageNo, queue, numFrames);
        (mem->pageTable)[pageNo].frameNo = frameNo; //updating frame in page table
        (mem->pageTable)[pageNo].loadBit = 1;
        physicalAddress = (frameNo << 8) + mem->physicalMem + pageOffset;
        printf("%d, ", *((char *)(physicalAddress)));
        printf("%d, ", frameNo);
        printFrames((frameNo << 8) + mem->physicalMem);
        printf("\n");
        updateTlb(mem, frameNo, pageNo); // at frame no. makes tlb entry invalid invalid
    }
    return frameNo;
}
int pageFault(allMem *mem, unsigned int pageNo, Queue *queue, int numFrames)
{
    unsigned int repPage;
    unsigned int offset;
    int frameNo;
    void *frameIndex;
    if (strcmp("fifo", mem->alg) == 0)
    {
        enqueue(queue, pageNo);
    }
    (mem->phySpace) += 1;
    if (numFrames < (mem->phySpace))
    {
        repPage = getReplacement(queue, mem); //write to physical memory using replacement logic
        frameNo = (mem->pageTable)[repPage].frameNo;
        (mem->pageTable)[repPage].loadBit = 0; //remove from pageTable
        frameIndex = (frameNo << 8) + (mem->physicalMem);
    }
    else
    {
        //only when memory hasn't filled up
        frameNo = (mem->phySpace) - 1;
        offset = 256 * ((mem->phySpace) - 1);
        frameIndex = mem->physicalMem + offset;
    }
    getFrombackup(mem, frameIndex, pageNo);
    return frameNo;
}
unsigned int getReplacement(Queue *queue, allMem *mem)
{
    if (strcmp(mem->alg, "fifo") == 0 || strcmp(mem->alg, "lru") == 0)
    {
        return dequeue(queue);
    }
    else
    {
        return findOptChoice(mem, queue);
    }
}
void getFrombackup(allMem *mem, void *frameIndex, unsigned int pageNo)
{
    int res;
    FILE *backup = fopen("BACKING_STORE.bin", "rb");
    if (backup == NULL)
    {
        printf("opening gone wrong !\n");
        perror(NULL);
    }
    res = fseek(backup, pageNo * 256, SEEK_SET);
    if (res != 0)
    {
        printf("seeking gone wrong!");
    }
    fread(frameIndex, 256, 1, backup);
    fclose(backup);
}
void printFrames(unsigned char *physAddr)
{
    int i = 0;
    for (i = 0; i < 256; i++)
    {
        printf("%.2X", *(physAddr + i));
    }
}
void printStats(Stats stats)
{
    printf("Number of Translated Addresses = %d\n", stats.numAddr);
    printf("Page Faults = %d\n", stats.pageFaults);
    printf("Page Fault Rate = %.3f\n", (double)(stats.pageFaults) / (double)(stats.numAddr));
    printf("TLB Hits = %d\n", stats.tlbHits);
    printf("TLB Misses = %d\n", (stats.numAddr) - (stats.tlbHits));
    printf("TLB Hit Rate = %.3f\n", (double)(stats.tlbHits) / (double)(stats.numAddr));
}
void lruDel(Queue *queue, int pageNo)
{
    Node *curr = queue->tail->prev;
    Node *temp;
    while (curr != NULL)
    {
        if (curr->data == pageNo)
        {
            if (curr->prev == NULL)
            {
                dequeue(queue);
                break;
            }
            else
            {
                (curr->prev)->next = (curr->next);
                (curr->next)->prev = curr->prev;
                free(curr);
                break;
            }
        }
        curr = (curr->prev);
    }
}
void updateTlb(allMem *mem, int frameNo, int pageNo)
{
    int i;
    for (i = 0; i < 16; i++)
    {
        if ((mem->tlb)[i].valid)
        {
            if ((mem->tlb)[i].frameNo == frameNo)
            {
                (mem->tlb)[i].valid = 0;
            }
        }
    }
}
int findOptChoice(allMem *mem, Queue *queue)
{
    int i = 0;
    int max[2];
    int val = 0;
    max[0] = 0;
    max[1] = 0;
    Node *temp = queue->head;
    Node *curr = mem->optCurr;
    for (i = 0; i < 256; i++)
    {
        if ((mem->pageTable)[i].loadBit == 1)
        {
            val = findDist(curr, i);
            if (max[0] < val)
            {
                max[0] = val;
                max[1] = i;
            }
        }
    }
    return max[1];
}
int findDist(Node *curr, int i)
{
    Node *temp = curr;
    int dist = 0;
    while (temp != NULL)
    {
        if (temp->data == i)
        {
            return dist;
        }
        dist++;
        temp = temp->next;
    }
    return dist;
}