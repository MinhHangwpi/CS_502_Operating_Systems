#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct
{
    int iRat;
    int iRoom;
} UnvisitedRoom;

typedef struct
{
    int capacity;
    int size;
    UnvisitedRoom *items;
} Stack;

void StackInit(Stack *s);

void StackPush(Stack *s, UnvisitedRoom item);

int StackPop(Stack *s, UnvisitedRoom *item);

int StackIsEmpty(Stack *s);

void StackFree(Stack *s);

void attemptRevisitedTheRoom(Stack *unvisitedRooms);

#endif