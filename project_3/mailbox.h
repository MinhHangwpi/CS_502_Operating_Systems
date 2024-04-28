#ifndef MAILBOX_H_
#define MAILBOX_H_

#include <semaphore.h>
#include <stdlib.h>
#define MAXTHREADS 10
#define REQUEST 1
#define REPLY 2

struct msg
{
    int iFrom;
    int value;
    int cnt;
    int tot;
};

struct mailbox
{
    int totalSum;
    struct msg message;
    int cnt;
    int tot;
};

struct undeliveredMessage {
    int iTo;
    struct msg message;
};


// Global variables

extern int numThreads;
extern struct mailbox mailboxes[MAXTHREADS + 1];
extern sem_t mutex[MAXTHREADS + 1];
extern sem_t full[MAXTHREADS + 1];
extern sem_t empty[MAXTHREADS + 1];

void SendMsg(int iTo, struct msg *pMsg);
void RecvMsg(int iFrom, struct msg *pMsg);
int NBSendMsg(int iTo, struct msg *pMsg);
void *addit(void *arg);
void InitMailBox();
void processParentMessages();

#endif