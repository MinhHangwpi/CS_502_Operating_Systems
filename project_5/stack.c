#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "stack.h"
#include "rat.h"
#include "room.h"
#include "vb.h"

void StackInit(Stack *s)
{
    s->capacity = MAXRATS;
    s->size = 0;
    s->items = malloc(sizeof(UnvisitedRoom) * s->capacity);
}

void StackPush(Stack *s, UnvisitedRoom item)
{
    if (s->size >= s->capacity)
    {
        s->capacity *= 2;
        s->items = realloc(s->items, sizeof(UnvisitedRoom) * s->capacity);
    }
    s->items[s->size++] = item;
}

int StackPop(Stack *s, UnvisitedRoom *item)
{
    if (s->size == 0)
    {
        return -1; // Stack is empty
    }
    *item = s->items[--s->size];
    return 0; // Success
}

int StackIsEmpty(Stack *s)
{
    return s->size == 0;
}

void StackFree(Stack *s)
{
    free(s->items);
}

void attemptRevisitedTheRoom(Stack *unvisitedRooms)
{
    int attemptAvailable = 20;

    while (!StackIsEmpty(unvisitedRooms) && (attemptAvailable > 0))
    {
        UnvisitedRoom roomToPop;
        if (StackPop(unvisitedRooms, &roomToPop) == 0)
        {
            if (TryToEnterRoom(roomToPop.iRat, roomToPop.iRoom) == -1)
            {
                StackPush(unvisitedRooms, roomToPop);
                attemptAvailable--;
            }

            sleep(roomconfigs[roomToPop.iRoom].time);
            LeaveRoom(roomToPop.iRat, roomToPop.iRoom, RoomVB[roomToPop.iRoom][roomToPop.iRat].tEntry);
        }
    }
}
