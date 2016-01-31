/*********************************************************\
* Author:	David Rigert
* Class:	CS344 Winter 2016
* Assignment:	Program 2 - adventure
\*********************************************************/
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*========================================================*
 * Macro definitions
 *
 * Define constants here for easy modification and to avoid magic numbers.
 *========================================================*/
#define CONNECTIONS_MAX 6	/* maximum connections in each room */
#define CONNECTIONS_MIN 3	/* minimum connections in each room */
#define MAX_PATH_LEN 255	/* maximum length of a file path */
#define ROOM_NAME_LEN_MAX 11/* maximum string length for room names */
#define ROOM_NAME_COUNT 10	/* number of room names available */
#define ROOM_TYPE_LEN_MAX 11/* maximum string length for room types */
#define ROOM_TYPE_COUNT 3	/* number of room types */
#define ROOM_COUNT 7		/* number of rooms to generate */
#define USERNAME "rigertd"	/* <username> part of room path */


/*========================================================*
 * Structs and enumerations
 *========================================================*/

/**
 * Represents a room in memory. Use ints instead of char arrays
 * for easier comparison and to save memory.
 */
typedef struct Room {
    int name;
    int connections[CONNECTIONS_MAX];
	int connectionCount;
    int type;
} Room;

/* Use this array to display the int representing a room name as a string */
const char RoomNameStrings[ROOM_NAME_COUNT][ROOM_NAME_LEN_MAX] = 
    {"Melee", "Hook", "Plunder", "Lucre", "Jambalaya", 
     "Scabb", "Phatt", "Skull", "Spittle", "Pinchpenny"};


/*
 * Enumerates the different room types.
 * Using enums instead of numbers improves code readability.
 */
enum RoomType {
    RoomType_START_ROOM,
    RoomType_MID_ROOM,
    RoomType_END_ROOM
};

/* Use this array to display a RoomType enumerator as a string */
const char RoomTypeStrings[ROOM_TYPE_COUNT][ROOM_TYPE_LEN_MAX] = 
    {"START_ROOM", "MID_ROOM", "END_ROOM"};

/*========================================================*
 * Function prototypes 
 *========================================================*/
void shuffleIntArray(int[], int);
void swap(int*, int*);
int createRooms(Room[], int);
int saveRooms(Room[], int);

/*========================================================*
 * main function 
 *========================================================*/
int main() {
	/* declare array of Room structs to hold rooms */
	Room rooms[ROOM_COUNT];
	
	/* seed the random number generator */
	srand((unsigned int)time(0));
	
	createRooms(rooms, ROOM_COUNT);
	saveRooms(rooms, ROOM_COUNT);
	
	return 0;
}

/*========================================================*
 * Function declarations 
 *========================================================*/
/*
 * Randomly generates the rooms and their connections.
 */
int createRooms(Room rooms[], int count) {
	int indices[ROOM_NAME_COUNT];
	int i, j;
	int randIdx;
	
	/* create an array of ints, 1 for each room name */
	for (i = 0; i < ROOM_NAME_COUNT; ++i) {
		indices[i] = i;
	}
	
	/* shuffle the indices */
	shuffleIntArray(indices, ROOM_NAME_COUNT);
	
	/* initialize rooms arrray and populate with the first 7 indices */
	for (i = 0; i < ROOM_COUNT; ++i) {
		rooms[i].name = indices[i];
		rooms[i].type = RoomType_MID_ROOM;
		/* initialize connections to -1 */
		for (j = 0; j < CONNECTIONS_MAX; ++j) {
			rooms[i].connections[j] = -1;
		}
		rooms[i].connectionCount = 0;
	}
	
	/* Randomly assign the start and end rooms */
	rooms[rand() % ROOM_COUNT].type = RoomType_START_ROOM;
	
	/* Make sure END_ROOM does not overwrite START_ROOM */
	do {
		randIdx = rand() % ROOM_COUNT;
	} while (rooms[randIdx].type == RoomType_START_ROOM);
	rooms[randIdx].type = RoomType_END_ROOM;

}

/*
 * Saves the rooms to files.
 */
int saveRooms(Room rooms[], int count) {
    int i;
    int fd;						/* file descriptor */
	char roomPath[MAX_PATH_LEN];/* path to store room files in */
	
    /* Build path to room directory of <username>.rooms.<pid> */
	snprintf(roomPath, sizeof(roomPath), "%s.rooms.%d", USERNAME, getpid());
	
    /*for (i = 0; i < ROOM_COUNT; ++i) {
        fd = open(
    }*/
	for (i = 0; i < ROOM_COUNT; ++i) {
		printf("ROOM NAME: %s\nROOM TYPE: %s\n\n", 
			RoomNameStrings[rooms[i].name], 
			RoomTypeStrings[rooms[i].type]);
	}
}

/*
 * Randomly shuffles the values in an integer array.
 */
void shuffleIntArray(int vals[], int size) {
	int i;
	int j;
	
	/* shuffle the values in the array */
	for (i = 0; i < size; ++i) {
		j = rand() % size;
		if (i != j)
			swap(&vals[i], &vals[j]);
	}
}

/*
 * Swaps two integer values.
 */
void swap(int* a, int* b) {
	int c;
	c = *a;
	*a = *b;
	*b = c;
}
