#include "room.h"
#include "rat.h"
#include "vb.h"
#include "stack.h"

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

#define MAXRATS 5
#define MAXROOMS 8

int numRats;
int numRooms;
char algorithm;

int nonBlockingMode = 0;
int totalTraversalTime = 0;
int VisitorCount[MAXROOMS];
time_t programStartTime;
sem_t roomSem[MAXROOMS];
sem_t roomVBSem[MAXROOMS][MAXRATS];
sem_t visitorCountMutex[MAXROOMS];

int ratTraversalTime[MAXRATS];

struct vbentry RoomVB[MAXROOMS][MAXRATS];
int VisitorCount[MAXROOMS];
int totalTraversalTime;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <num_rats> <algorithm> <non_blocking_mode>\n", argv[0]);
        return 1;
    }

    numRats = atoi(argv[1]);
    algorithm = argv[2][0];

    if (numRats <= 0 || numRats > MAXRATS)
    {
        printf("Number of rats must be between 1 and %d\n", MAXRATS);
        return 1;
    }

    if (algorithm != 'i' && algorithm != 'd')
    {
        printf("Algorithm must be 'i' (in-order) or 'd' (distributed)\n");
        return 1;
    }

    if (argc > 3 && strcmp(argv[3], "n") == 0)
    {
        nonBlockingMode = 1;
    }

    numRooms = parse_rooms_config("rooms.txt");

    for (int i = 0; i < numRooms; i++)
    {
        sem_init(&roomSem[i], 0, roomconfigs[i].capacity);
        int sem_value;
        sem_getvalue(&roomSem[i], &sem_value);
        sem_init(&visitorCountMutex[i], 0, 1);

        for (int j = 0; j < numRats; j++)
        {
            sem_init(&roomVBSem[i][j], 0, 1);
        }
    }

    programStartTime = time(NULL);

    pthread_t ratThreads[numRats];
    for (long i = 0; i < numRats; i++)
    {
        int rc = pthread_create(&ratThreads[i], NULL, Rat, (void *)i);

        if (rc != 0)
        {
            printf("Error; return code from pthread_create() is %d\n", rc);
        }
    }

    for (int i = 0; i < numRats; i++)
    {
        pthread_join(ratThreads[i], NULL);
        printf("Rat %d completed maze in %d seconds\n", i, ratTraversalTime[i]);
    }

    for (int i = 0; i < numRooms; i++)
    {
        printf("Room %d [%d, %d]: ", i, roomconfigs[i].capacity, roomconfigs[i].time);
        for (int j = 0; j < numRats; j++)
        {
            printf(" %d %d %d;", RoomVB[i][j].iRat, RoomVB[i][j].tEntry, RoomVB[i][j].tDep);
        }
        printf("\n");
    }

    totalTraversalTime = 0;
    
    for (int i = 0; i < numRats; i++)
    {
        totalTraversalTime += ratTraversalTime[i];
    }
    
    printf("Total Traversal Time: %d seconds, ", totalTraversalTime);
    int idealTraversalTime = 0;
    
    for (int i = 0; i < numRooms; i++)
    {
        idealTraversalTime += roomconfigs[i].time;
    }
    
    printf("compared to ideal time: %d seconds\n", idealTraversalTime * numRats);

    for (int i = 0; i < MAXROOMS; i++)
    {
        sem_destroy(&roomSem[i]);
        sem_destroy(&visitorCountMutex[i]);
        for (int j = 0; j < MAXRATS; j++)
        {
            sem_destroy(&roomVBSem[MAXROOMS][MAXRATS]);
        }
    }
    return 0;
}