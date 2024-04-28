// queue.h
#ifndef QUEUE_H
#define QUEUE_H
#include "mailbox.h"

typedef struct
{
    int iTo;
    struct msg message;
} UndeliveredMessage;

typedef struct
{
    UndeliveredMessage *items;
    int capacity;
    int size;
} Queue;

void QueueInit(Queue *q); 
void QueuePush(Queue *q, UndeliveredMessage item);
int QueuePop(Queue *q, UndeliveredMessage *item);
int QueueIsEmpty(Queue *q);
void QueueFree(Queue *q);
void attemptDeliveryOfUndeliveredMessages(Queue *q, int before);


#endif // QUEUE_H
