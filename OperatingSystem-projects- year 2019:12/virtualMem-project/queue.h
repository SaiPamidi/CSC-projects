#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    unsigned int data;
    struct Node *next;
    struct Node *prev;
} Node;
typedef struct Queue
{
    Node *head;
    Node *tail;
    int size;
} Queue;
void enqueue(Queue *, unsigned int);
unsigned int dequeue(Queue *);
void print(Queue *);