#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include "mailbox.h"
#include "queue.h"
#include <stdbool.h>
#include <string.h>

#define MAXTHREADS 10
#define REQUEST 1
#define REPLY 2
#define MAX_LINE_LENGTH 100

struct mailbox mailboxes[MAXTHREADS + 1];
sem_t mutex[MAXTHREADS + 1];
sem_t full[MAXTHREADS + 1];
sem_t empty[MAXTHREADS + 1];
int numThreads;

int main(int argc, char *argv[])
{
    int nonBlockingMode = 0;
    int val, iTo;
    char buffer[MAX_LINE_LENGTH];

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s number_of_threads\n", argv[0]);
        return 1;
    }

    if (argc > 2 && strcmp(argv[2], "nb") == 0)
    {
        nonBlockingMode = 1;
    }

    numThreads = atoi(argv[1]);

    if (numThreads < 1 || numThreads > MAXTHREADS)
    {
        fprintf(stderr, "The number of threads should be between 1 and %d.\n", MAXTHREADS);
        return 1;
    };

    InitMailBox();
    pthread_t threads[numThreads + 1];

    for (long i = 1; i < numThreads + 1; i++)
    {
        int rc = pthread_create(&threads[i], NULL, addit, (void *)i);
        if (rc == 0)
        {
            printf("Successfully created thread %ld with ID %lu\n", i, (unsigned long)threads[i]);
        }
        else
        {
            printf("Error; return code from pthread_create() is %d\n", rc);
        }
    }

    printf("This is %s mode. Please enter the inputs: \n", nonBlockingMode == 1 ? "Non-blocking": "Normal");
    fflush(stdout);

    if (nonBlockingMode == 1)
    {
        Queue undeliveredMessages;
        QueueInit(&undeliveredMessages);

        while (fgets(buffer, MAX_LINE_LENGTH, stdin) != NULL)
        {
            if (sscanf(buffer, "%d %d", &val, &iTo) == 2)
            {
                if (val == -1)
                    continue;

                struct msg message;
                message.iFrom = 0;
                message.value = val;
                message.cnt = 0;
                message.tot = 0;

                if (NBSendMsg(iTo, &message) == -1)
                {
                    UndeliveredMessage undelivered;
                    undelivered.iTo = iTo;
                    undelivered.message = message;
                    QueuePush(&undeliveredMessages, undelivered);
                }
            }
            else
            {
                fprintf(stderr, "Invalid input format.\n");
                fflush(stdout);
                break;
            }
        }
        if (feof(stdin))
        {
            printf("End of input detected.\n");
            fflush(stdout);
        }
        else if (ferror(stdin))
        {
            perror("Error reading input");
            fflush(stdout);
        }

        // Attempt to re-deliver messages BEFORE termination signal
        attemptDeliveryOfUndeliveredMessages(&undeliveredMessages);

        for (int i = 1; i < numThreads + 1; i++)
        {
            struct msg termMsg = {0, -1, 0, 0};
            if (NBSendMsg(i, &termMsg) == -1)
            {
                UndeliveredMessage undelivered = {i, termMsg};
                QueuePush(&undeliveredMessages, undelivered);
            }
        }

        // attempt to re-deliver message AFTER termination signal
        attemptDeliveryOfUndeliveredMessages(&undeliveredMessages);

        QueueFree(&undeliveredMessages);
    }
    else
    {
        while (fgets(buffer, MAX_LINE_LENGTH, stdin) != NULL)
        {
            if (sscanf(buffer, "%d %d", &val, &iTo) == 2)
            {
                if (val == -1)
                    continue;

                struct msg message;
                message.iFrom = 0;
                message.value = val;
                message.cnt = 0;
                message.tot = 0;

                SendMsg(iTo, &message);
            }
            else
            {
                fprintf(stderr, "Invalid input format.\n");
                fflush(stdout);
                break;
            }
        }

        if (feof(stdin))
        {
            printf("End of input detected.\n");
            fflush(stdout);
        }
        else if (ferror(stdin))
        {
            perror("Error reading input");
            fflush(stdout);
        }

        for (int i = 1; i < numThreads + 1; i++)
        {
            struct msg termMsg = {0, -1, 0, 0};
            SendMsg(i, &termMsg);
        }
    }

    processParentMessages();

    // wait for all threads to exit, except for parent
    for (int j = 1; j < numThreads + 1; j++)
    {
        printf("Parent thread waiting for thread %d to stop \n", j);
        fflush(stdout);
        void *exit_status = 0;
        pthread_join(threads[j], NULL);
        printf("Child thread %d exited with status %ld.\n", j, (long)exit_status);
    }

    pthread_exit(NULL);

    return 0;
}
