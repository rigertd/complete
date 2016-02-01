/*********************************************************\
* Author:     David Rigert
* Class:      CS344 Winter 2016
* Assignment: Program 2 - adventure
\*********************************************************/
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*========================================================*
 * Macro definitions
 *
 * Define constants here for easy modification and to avoid magic numbers.
 *========================================================*/
#define CONNECT_MAX 6   /* maximum connections in each room */
#define CONNECT_MIN 3   /* minimum connections in each room */
#define MAX_PATH_LEN 255    /* maximum length of a file path */
#define ROOM_NAME_LEN_MAX 11/* maximum string length for room names */
#define ROOM_NAME_COUNT 10  /* number of room names available */
#define ROOM_TYPE_LEN_MAX 11/* maximum string length for room types */
#define ROOM_TYPE_COUNT 3   /* number of room types */
#define ROOM_COUNT 7        /* number of rooms to generate */
#define USERNAME "rigertd"  /* <username> part of room path */
#define BUFFER_SIZE 255     /* must be longer than longest output line */

/*========================================================*
 * Structs and enumerations
 *========================================================*/

/**
 * Represents a room in memory. Use ints instead of char arrays
 * for easier comparison and to save memory.
 */
typedef struct Room {
    int name;
    int connects[CONNECT_MAX];
    int connectCnt;
    int type;
} Room;

/*
 * Enumerates the different room types.
 * Using enums instead of numbers improves code readability.
 */
enum RoomType {
    RoomType_START_ROOM,
    RoomType_MID_ROOM,
    RoomType_END_ROOM
};

/*========================================================*
 * Constant definitions
 *========================================================*/
/* Use this array to display a RoomType enumerator as a string */
const char RoomTypeStrings[ROOM_TYPE_COUNT][ROOM_TYPE_LEN_MAX] = 
    {"START_ROOM", "MID_ROOM", "END_ROOM"};

/* Use this array to display the int representing a room name as a string */
const char RoomNameStrings[ROOM_NAME_COUNT][ROOM_NAME_LEN_MAX] = 
    {"Melee", "Hook", "Plunder", "Lucre", "Jambalaya", 
     "Scabb", "Phatt", "Skull", "Spittle", "Pinchpenny"};


/*========================================================*
 * Function prototypes 
 *========================================================*/
void shuffleIntArray(int[], int);
void swap(int*, int*);
int contains(int[], int, int);
void createRooms(Room[], int);
void saveRooms(Room[], int, char*);
void startGame(char*, int);

/*========================================================*
 * main function 
 *========================================================*/
int main() {
    /* declare array of Room structs to hold rooms */
    Room rooms[ROOM_COUNT];
    
    /* seed the random number generator */
    srand((unsigned int)time(0));
    
    /* root directory for rooms as <username>.rooms.<pid> */
    char root[MAX_PATH_LEN];
    snprintf(root, sizeof(root), "%s.rooms.%d", USERNAME, getpid());
    
    /* create the room data in memory */
    createRooms(rooms, ROOM_COUNT);
    
    /* save the room data to disk */
    saveRooms(rooms, ROOM_COUNT, root);
    
    /* start the game loop */
    startGame(root, ROOM_COUNT);
    
    return 0;
}

/*========================================================*
 * Function definitions
 *========================================================*/
/**
 * Randomly generates the rooms and their connections.
 */
void createRooms(Room rooms[], int count) {
    int indices[ROOM_NAME_COUNT];
    int i, j;
    int randIdx;
    int randCnt;
    
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
        for (j = 0; j < CONNECT_MAX; ++j) {
            rooms[i].connects[j] = -1;
        }
        rooms[i].connectCnt = 0;
    }
    
    /* randomly assign the start and end rooms */
    rooms[rand() % ROOM_COUNT].type = RoomType_START_ROOM;
    
    /* make sure END_ROOM does not overwrite START_ROOM */
    do {
        randIdx = rand() % ROOM_COUNT;
    } while (rooms[randIdx].type == RoomType_START_ROOM);
    rooms[randIdx].type = RoomType_END_ROOM;
    
    /* randomly assign CONNECT_MIN to CONNECT_MAX links */
    for (i = 0; i < ROOM_COUNT; ++i) {
        randCnt = rand() % (CONNECT_MAX - CONNECT_MIN + 1) + CONNECT_MIN;
        /* only add more if randCnt not already reached */
        while (rooms[i].connectCnt < randCnt) {
            randIdx = rand() % ROOM_COUNT;
            /* only add connection if target is not self,
               if target connection count not > CONNECT_MAX,
               and if target is not already a target */
            if (randIdx != i && rooms[randIdx].connectCnt < CONNECT_MAX
                && !contains(rooms[i].connects, rooms[i].connectCnt, randIdx)) {
                rooms[i].connects[rooms[i].connectCnt++] = randIdx;
                rooms[randIdx].connects[rooms[randIdx].connectCnt++] = i;
            }
        }
    }
}

/**
 * Determines whether the specified array contains the specified int.
 */
int contains(int arr[], int count, int val) {
    int i;
    for (i = 0; i < count; ++i) {
        if (arr[i] == val) return 1;
    }
    return 0;
}

/**
 * Saves the rooms to files.
 */
void saveRooms(Room rooms[], int count, char* root) {
    int i, j;
    int fd;                     /* file descriptor */
    char roomPath[MAX_PATH_LEN];/* path to store room files in */
    char buffer[BUFFER_SIZE];   /* stores file data before writing */
    
    /* try to make the root directory */
    if (mkdir(root) < 0 && errno != EEXIST) {
        /* failed to create directory and it does not already exist */
        fprintf(stderr, "Error creating directory: %s (errno %d)", roomPath, errno);
        exit(1);
    }
    
    for (i = 0; i < count; ++i) {
        /* build path to filename of <username>.rooms.<pid>/<room_no> */
        snprintf(roomPath, sizeof(roomPath), "%s/room%d", root, i + 1);
        
        /* open the generated file path for writing */
        fd = open(roomPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        
        /* verify file handle and write to stderr if invalid */
        if (fd == -1) {
            fprintf(stderr, "Error writing to file: %s (errno %d)", roomPath, errno);
            exit(1);
        }
        
        /* write the room data to the buffer and then to the file */
        snprintf(buffer, sizeof(buffer), "ROOM NAME: %s\n", 
            RoomNameStrings[rooms[i].name]);
        if (!write(fd, buffer, strlen(buffer))) {
            fprintf(stderr, "Error writing to file: %s (errno %d)", roomPath, errno);
            exit(1);
        }
        for (j = 0; j < rooms[i].connectCnt; ++j) {
            snprintf(buffer, sizeof(buffer), "CONNECTION %d: %s\n",
            j + 1,
            RoomNameStrings[rooms[i].connects[j]]);
            if (!write(fd, buffer, strlen(buffer))) {
                fprintf(stderr, "Error writing to file: %s (errno %d)", roomPath, errno);
                exit(1);
            }
        }
        snprintf(buffer, sizeof(buffer), "ROOM TYPE: %s\n",
            RoomTypeStrings[rooms[i].type]);
            if (!write(fd, buffer, strlen(buffer))) {
                fprintf(stderr, "Error writing to file: %s (errno %d)", roomPath, errno);
                exit(1);
            }
        
        /* close the file descriptor and flush the cache */
        close(fd);
    }
}

/**
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

/**
 * Starts the main game loop.
 */
void startGame(char* root, int count) {
    return;
}

/**
 * Swaps two integer values.
 */
void swap(int* a, int* b) {
    int c;
    c = *a;
    *a = *b;
    *b = c;
}
