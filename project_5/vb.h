#ifndef VB_H_
#define VB_H_

#include <time.h>
#include <semaphore.h>

#define MAXROOMS 8
#define MAXRATS 5

struct vbentry
{
    int iRat;
    int tEntry;
    int tDep; 
};

extern time_t programStartTime;
extern int totalTraversalTime;
extern struct vbentry RoomVB[MAXROOMS][MAXRATS];
extern int VisitorCount[MAXROOMS];

extern sem_t roomVBSem[MAXROOMS][MAXRATS];
extern sem_t visitorCountMutex[MAXROOMS];


#endif