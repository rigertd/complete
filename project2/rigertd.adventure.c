/*********************************************************\
* Author:	David Rigert
* Class:	CS344 Winter 2016
* Assignment:	Program 2 - adventure
\*********************************************************/
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define CONNECTIONS_MAX 6
#define ROOM_NAME_LEN_MAX 11
#define ROOM_NAME_COUNT 10
#define ROOM_TYPE_LEN_MAX 11
#define ROOM_TYPE_COUNT 3
#define ROOM_COUNT 7

/*
 * Represents a room in memory.
 * The use of enumerations saves memory.
 */
typedef struct Room {
    int Name;
    int Connections[CONNECTIONS_MAX];
    int Type;
} Room;

/*
 * Enumerates the different room names.
 * Using an enum saves memory and makes the values easier to handle.
 * Only convert the enumerators to strings when they must be displayed.
 */
enum RoomName {
    MELEE,
    HOOK,
    PLUNDER,
    LUCRE,
    JAMBALAYA,
    SCABB,
    PHATT,
    SKULL,
    SPITTLE,
    PINCHPENNY
};

/* Use this array to convert a RoomName enumerator to a C-style string */
const char RoomNameStrings[ROOM_NAME_COUNT][ROOM_NAME_LEN_MAX] = 
    {"Melee", "Hook", "Plunder", "Lucre", "Jambalaya", 
     "Scabb", "Phatt", "Skull", "Spittle", "Pinchpenny"};


/*
 * Enumerates the different room types.
 */
enum RoomType {
    START_ROOM,
    MID_ROOM,
    END_ROOM
};

/* Use this array to convert a RoomType enumerator to a C-style string */
const char RoomTypeStrings[ROOM_TYPE_COUNT][ROOM_TYPE_LEN_MAX] = 
    {"START_ROOM", "MID_ROOM", "END_ROOM"};

int saveRooms(Room[] rooms) {
    int i;
    int fd;
    // need to concat path to filename
    for (i = 0; i < ROOM_COUNT; ++i) {
        fd = open(
    }
}

int main() {
    int i;
    for (i = 0; i < ROOM_TYPE_COUNT; ++i) {
        printf("%s\n", RoomTypeStrings[i]);
    }
    for (i = 0; i < ROOM_NAME_COUNT; ++i) {
        printf("%s\n", RoomNameStrings[i]);
    }
}