#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "mailbox.h"
#include "queue.h"

void QueueInit(Queue *q)
{
    q->capacity = 10; // Initial capacity
    q->size = 0;
    q->items = malloc(sizeof(UndeliveredMessage) * q->capacity);
}

void QueuePush(Queue *q, UndeliveredMessage item)
{
    if (q->size >= q->capacity)
    {
        q->capacity *= 2;
        q->items = realloc(q->items, sizeof(UndeliveredMessage) * q->capacity);
    }
    q->items[q->size++] = item;
}

int QueuePop(Queue *q, UndeliveredMessage *item)
{
    if (q->size == 0)
    {
        return -1; // Queue is empty
    }
    *item = q->items[0];
    for (int i = 1; i < q->size; i++)
    {
        q->items[i - 1] = q->items[i]; // Shift items left
    }
    q->size--;
    return 0; // Success
}

int QueueIsEmpty(Queue *q)
{
    return q->size == 0;
}

void QueueFree(Queue *q)
{
    free(q->items);
}

void attemptDeliveryOfUndeliveredMessages(Queue *undeliveredMessages)
{
    int attemptAvailable = 20; // allowing 20 consecutive retries

    while (!QueueIsEmpty(undeliveredMessages) && (attemptAvailable > 0))
    {
        UndeliveredMessage messageToPop;
        if (QueuePop(undeliveredMessages, &messageToPop) == 0)
        {
            printf("Attempted to redeliver message to %d {iFrom=%d, value=%d, cnt=%d, tot=%d}\n", messageToPop.iTo, messageToPop.message.iFrom,
                   messageToPop.message.value, messageToPop.message.cnt, messageToPop.message.tot);
            
            sleep(3); // wait for 3 seconds before redelivery
            
            if (NBSendMsg(messageToPop.iTo, &messageToPop.message) == -1)
            {
                QueuePush(undeliveredMessages, messageToPop);
                attemptAvailable--;
            }
        }
    }
}