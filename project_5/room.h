#ifndef ROOM_H_
#define ROOM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define MAXROOMS 8

struct roomconfig
{
    int capacity;
    int time;
};

extern struct roomconfig roomconfigs[MAXROOMS]; // storing room configurations read from `rooms.txt`
extern int numRooms;
extern sem_t roomSem[MAXROOMS];

int parse_rooms_config(const char *filename);

#endif