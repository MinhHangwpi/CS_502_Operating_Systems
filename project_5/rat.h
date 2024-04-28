#ifndef RAT_H_
#define RAT_H_

#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

#define MAXROOMS 8
#define MAXRATS 5

extern int numRats;
extern char algorithm;
extern int nonBlockingMode;
extern pthread_t ratThreads[MAXRATS];
extern int ratTraversalTime[MAXRATS];

void *Rat(void *arg);
void EnterRoom(int iRat, int iRoom);
void LeaveRoom(int iRat, int iRoom, int tEnter);
int TryToEnterRoom(int iRat, int iRoom);

#endif