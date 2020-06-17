#include "queue.h"

void enqueue(Queue *queue, unsigned int data)
{
    Node *node = (Node *)calloc(1, sizeof(Node));
    node->next = NULL;
    node->prev = NULL;
    node->data = data;
    queue->size = (queue->size) + 1;
    if (node == NULL)
    {
        printf("malloc failed!\n");
        perror(NULL);
    }
    if (queue->head == NULL)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        node->prev = queue->tail;
        (queue->tail)->next = node;
        queue->tail = node;
    }
}
unsigned int dequeue(Queue *queue)
{
    Node *temp;
    unsigned int data;
    queue->size = (queue->size) - 1;
    if (queue->head == NULL)
    {
        printf("queue empty!\n");
    }
    temp = queue->head;
    data = (queue->head)->data;
    queue->head = (queue->head)->next;
    if (queue->head)
    {
        (queue->head)->prev = NULL;
    }
    else
    {
        queue->tail = NULL;
    }
    free(temp);
    return data;
}
void print(Queue *queue)
{
    Node *temp = queue->head;
    while (temp)
    {
        printf("%u\n", temp->data);
        temp = temp->next;
    }
}