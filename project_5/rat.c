#include <stdlib.h>
#include "rat.h"
#include "vb.h"
#include "room.h"
#include "stack.h"


void *Rat(void *arg)
{
    int index = (int)arg;
    int currentRoom = algorithm == 'i' ? 0 : index % numRooms;
    int roomsVisited = 0;

    Stack unvisitedRooms;
    StackInit(&unvisitedRooms);

    while (roomsVisited < numRooms)
    {
        if (nonBlockingMode == 1)
        {
            if (TryToEnterRoom(index, currentRoom) == -1)
            {
                // failed to enter the room, push the info to stack to try later
                UnvisitedRoom roomToPush;
                roomToPush.iRat = index;
                roomToPush.iRoom = currentRoom;
                StackPush(&unvisitedRooms, roomToPush);

                // increment currentRoom and roomVisited to avoid infinity loop
                currentRoom = (currentRoom + 1) % numRooms;
                roomsVisited++;
                continue;
            }
        }
        else
        {
            EnterRoom(index, currentRoom);
        }
        sleep(roomconfigs[currentRoom].time);
        LeaveRoom(index, currentRoom, RoomVB[currentRoom][index].tEntry);
        currentRoom = (currentRoom + 1) % numRooms;
        roomsVisited++;
    }

    // retry the failed rooms earlier
    attemptRevisitedTheRoom(&unvisitedRooms);

    ratTraversalTime[index] = difftime(time(NULL), programStartTime);
    int *exit_status = 0;
    pthread_exit((void *)exit_status);
}

void EnterRoom(int iRat, int iRoom)
{
    sem_wait(&roomSem[iRoom]);
    sem_wait(&roomVBSem[iRoom][iRat]);
    sem_wait(&visitorCountMutex[iRoom]);

    RoomVB[iRoom][iRat].iRat = iRat;
    RoomVB[iRoom][iRat].tEntry = (int)difftime(time(NULL), programStartTime);
    VisitorCount[iRoom]++;
    
    sem_post(&visitorCountMutex[iRoom]);
    sem_post(&roomVBSem[iRoom][iRat]);
}

int TryToEnterRoom(int iRat, int iRoom)
{
    int sem_value;
    sem_getvalue(&roomSem[iRoom], &sem_value);

    if (sem_value <= 0)
    {
        fflush(stdout);
        return -1;
    }
    else
    {
        EnterRoom(iRat, iRoom);
        return 0;
    }
}

void LeaveRoom(int iRat, int iRoom, int tEnter)
{
    sem_wait(&roomVBSem[iRoom][iRat]);

    RoomVB[iRoom][iRat].tDep = (int)difftime(time(NULL), programStartTime);

    sem_post(&roomVBSem[iRoom][iRat]);
    sem_post(&roomSem[iRoom]);
}