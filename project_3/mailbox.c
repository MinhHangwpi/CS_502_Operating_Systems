#include "mailbox.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

void SendMsg(int iTo, struct msg *pMsg)
{
    if (iTo < 0 || iTo > numThreads + 1)
    {
        printf("called SendMsg with invalid arguments\n");
        fflush(stdout);
        return;
    }
    sem_wait(&empty[iTo]);
    sem_wait(&mutex[iTo]);

    mailboxes[iTo].message = *pMsg;

    sem_post(&mutex[iTo]);
    sem_post(&full[iTo]);
}

void RecvMsg(int iRecv, struct msg *pMsg)
{
    if (iRecv < 0 || iRecv > numThreads + 1)
    {
        printf("called recvMsg with invalid arguments\n");
        fflush(stdout);
        return;
    }

    sem_wait(&full[iRecv]);
    sem_wait(&mutex[iRecv]);

    *pMsg = mailboxes[iRecv].message;

    sem_post(&mutex[iRecv]);
    sem_post(&empty[iRecv]);
}

// include the parent mailbox at index 0
void InitMailBox()
{
    {
        for (int i = 0; i < MAXTHREADS + 1; i++)
        {
            mailboxes[i].totalSum = 0;
            sem_init(&mutex[i], 0, 1);
            sem_init(&full[i], 0, 0);
            sem_init(&empty[i], 0, 1);
            mailboxes[i].cnt = 0;
            mailboxes[i].tot = 0;
        }
    }
}

void *addit(void *arg)
{
    long index = (long)arg;
    struct msg message;

    time_t startTime, endTime;
    double executionTime;

    startTime = time(NULL);

    while (1)
    {
        RecvMsg(index, &message);

        if (message.value < 0)
        {
            endTime = time(NULL);
            executionTime = difftime(endTime, startTime);

            struct msg summary;
            summary.iFrom = index;
            summary.value = mailboxes[index].totalSum;
            summary.tot = executionTime;
            summary.cnt = mailboxes[index].cnt;

            SendMsg(0, &summary);
            break;
        }
        else
        {
            mailboxes[index].totalSum += message.value;
            mailboxes[index].cnt += 1;
            sleep(3);
        }
    }

    int *exit_status = 0;
    pthread_exit((void *)exit_status);
}

void processParentMessages()
{
    int receivedMessages[MAXTHREADS] = {0};
    int messagesToReceive = numThreads;

    struct msg message;
    while (messagesToReceive > 0)
    {
        RecvMsg(0, &message);
        if (message.iFrom > 0 && message.iFrom <= numThreads)
        {
            if (!receivedMessages[message.iFrom - 1])
            {
                receivedMessages[message.iFrom - 1] = 1;
                messagesToReceive--;

                printf("The result from thread %d is %d from %d operations during %d secs\n",
                       message.iFrom,
                       message.value,
                       message.cnt,
                       message.tot);
            }
        }

        if (messagesToReceive == 0)
            break;
    }
}

int NBSendMsg(int iTo, struct msg *pMsg)
{
    if (iTo < 0 || iTo > numThreads)
    {
        printf("called NBSendMsg with invalid arguments\n");
        fflush(stdout);
        return -1;
    }

    if (sem_trywait(&empty[iTo]) == -1)
    {
        return -1;
    }

    sem_wait(&mutex[iTo]);

    mailboxes[iTo].message = *pMsg;

    sem_post(&mutex[iTo]);
    sem_post(&full[iTo]);

    return 0;
}
