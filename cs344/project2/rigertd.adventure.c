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
#define CONNECT_MAX 6           // maximum connections in each room
#define CONNECT_MIN 3           // minimum connections in each room
#define MAX_PATH_LEN 255        // maximum length of a file path
#define ROOM_NAME_LEN_MAX 11    // maximum string length for room names
#define ROOM_NAME_COUNT 10      // number of room names available
#define ROOM_TYPE_LEN_MAX 11    // maximum string length for room types
#define ROOM_TYPE_COUNT 3       // number of room types
#define ROOM_COUNT 7            // number of rooms to generate
#define USERNAME "rigertd"      // <username> part of room path
#define BUFFER_SIZE 255         // must be longer than longest output line
#define MAX_MOVES 255           // max number of moves that can be stored

/*========================================================*
 * Structs and enumerations
 *========================================================*/

/**
 * Enumerates the different room types.
 * Using enums instead of numbers improves code readability.
 */
enum RoomType {
    RoomType_START_ROOM,
    RoomType_MID_ROOM,
    RoomType_END_ROOM
};

/**
 * Represents a room in memory. Use ints instead of char arrays
 * for easier comparison and to save memory.
 */
typedef struct Room {
    int name;                   // index of the room name in RoomNameStrings
    int connects[CONNECT_MAX];  // array of indices of connected rooms
    int connectCnt;             // actual number of connected rooms
    enum RoomType type;         // enum representing the room type
} Room;

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
int contains(int[], int, int);
void createRooms(Room[], int);
void getInput(char*, int);
void getStartRoom(char*, Room*);
void initializeIntArray(int[], int, int);
int loadRoom(Room*, int, char*);
int parseRoomName(char*);
int parseRoomType(char*);
void readLine(int, char*, size_t);
void saveRooms(Room[], int, char*);
void shuffleIntArray(int[], int);
void startGame(char*);
void swap(int*, int*);

/*========================================================*
 * main function
 *========================================================*/
int main() {
    // declare array of Room structs on stack to hold rooms
    Room rooms[ROOM_COUNT];

    // seed the random number generator
    srand((unsigned int)time(0));

    // root directory for rooms as <username>.rooms.<pid>
    char root[MAX_PATH_LEN];
    snprintf(root, sizeof(root), "%s.rooms.%d", USERNAME, getpid());

    // create the room data in memory
    createRooms(rooms, ROOM_COUNT);

    // save the room data to disk
    saveRooms(rooms, ROOM_COUNT, root);
    Room tmp;
    loadRoom(&tmp, 0, root);
    
    // start the game loop
    startGame(root);

    // return 0 because game ended successfully
    return 0;
}

/*========================================================*
 * Function definitions
 *========================================================*/
/**
 * Randomly generates the rooms and their connections.
 *
 *  rooms       Array to store rooms in. Must already be allocated.
 *  count       Number of rooms to store in the array.
 */
void createRooms(Room rooms[], int count) {
    int indices[ROOM_NAME_COUNT];
    int i, j;
    int randIdx;
    int randCnt;

    // create an array of ints, 1 for each room name
    for (i = 0; i < ROOM_NAME_COUNT; ++i) {
        indices[i] = i;
    }

    // shuffle the indices
    shuffleIntArray(indices, ROOM_NAME_COUNT);

    // initialize rooms arrray and populate with the first 7 indices
    for (i = 0; i < ROOM_COUNT; ++i) {
        rooms[i].name = indices[i];
        rooms[i].type = RoomType_MID_ROOM;
        // initialize connection indices to -1
        for (j = 0; j < CONNECT_MAX; ++j) {
            rooms[i].connects[j] = -1;
        }
        rooms[i].connectCnt = 0;
    }

    // randomly assign the start and end rooms
    rooms[rand() % ROOM_COUNT].type = RoomType_START_ROOM;

    // randomize index for END_ROOM until it does not overwrite START_ROOM
    do {
        randIdx = rand() % ROOM_COUNT;
    } while (rooms[randIdx].type == RoomType_START_ROOM);
    rooms[randIdx].type = RoomType_END_ROOM;

    // randomly assign between CONNECT_MIN and CONNECT_MAX links
    for (i = 0; i < ROOM_COUNT; ++i) {
        randCnt = rand() % (CONNECT_MAX - CONNECT_MIN + 1) + CONNECT_MIN;
        // only add more if randCnt not already reached
        while (rooms[i].connectCnt < randCnt) {
            randIdx = rand() % ROOM_COUNT;
            // only add connection if target is not self,
            // if target connection count not > CONNECT_MAX,
            // and if target is not already a target
            if (rooms[randIdx].name != rooms[i].name
                && rooms[randIdx].connectCnt < CONNECT_MAX
                && !contains(rooms[i].connects, rooms[i].connectCnt, rooms[randIdx].name)) {
                rooms[i].connects[rooms[i].connectCnt++] = rooms[randIdx].name;
                rooms[randIdx].connects[rooms[randIdx].connectCnt++] = rooms[i].name;
            }
        }
    }
}

/**
 * Determines whether the specified array contains the specified int.
 *
 *  arr     Integer array to check.
 *  size    Number of elements in the array.
 *
 * Returns 1 if the array contains val; or 0 otherwise.
 */
int contains(int arr[], int size, int val) {
    int i;
    for (i = 0; i < size; ++i) {
        if (arr[i] == val) return 1;
    }
    return 0;
}

/**
 * Gets user input from stdin and strips the trailing line feed.
 *
 *  buf     Buffer to store the input in.
 *  size    Maximum amount of input to get. Buffer stores up to size - 1.
 */
void getInput(char* buf, int size) {
    int i;
    char c;

    // get user input
    fgets(buf, size, stdin);

    // convert first newline to null and return
    for (i = 0; i < size; ++i) {
        if (buf[i] == '\n') {
            buf[i] = '\0';
            return;
        }
    }

    // if execution reaches here, no newline was encountered
    // consume any remaining data in stdin
    do {
        c = fgetc(stdin);
    } while (c != '\n' && c != EOF);
}

/**
 * Finds and loads the start room in the specified directory.
 * Terminates the program with exit code 1 if the start room cannot be found.
 *
 *  root    Path to the directory containing the room data files.
 *  start   Pointer to an allocated Room struct to store the data in.
 */
void getStartRoom(char* root, Room* start) {
    int i;
    for (i = 0; i < ROOM_NAME_COUNT; ++i) {
        if (loadRoom(start, i, root) && start->type == RoomType_START_ROOM) return;
    }

    // if execution reaches here, no start room was found
    fprintf(stderr, "Start room not found. Exiting.\n");
    exit(1);
}

/**
 * Sets all of the values in an integer array to the specified value.
 *
 *  arr     Integer array to initialize.
 *  size    Number of elements in the array.
 *  val     Value to set each element to.
 */
void initializeIntArray(int arr[], int size, int val) {
    int i;
    for (i = 0; i < size; ++i)
        arr[i] = val;
}

/**
 * Loads the specified room from a file.
 * Terminates the program with exit code 1 if invalid room data is found.
 *
 *  room    Pointer to allocated Room struct to store room data in.
 *  roomNo  Index of room to load.
 *  root    Path to the directory containing the room data.
 *
 * Returns 1 if the room is successfully loaded; or 0 otherwise.
 */
int loadRoom(Room* room, int roomNo, char* root) {
    int i;
    int fd;
    char buffer[BUFFER_SIZE];
    char roomPath[MAX_PATH_LEN];

    // build path to room file from root and roomNo
    snprintf(roomPath, sizeof(roomPath), "%s/room%d", root, roomNo);

    // open the room file read only
    fd = open(roomPath, O_RDONLY);
    if (fd == -1) {
        // return false if the room file was not found
        return 0;
    }

    // read the room name and convert to index
    readLine(fd, buffer, sizeof(buffer));
    if (strncmp(buffer, "ROOM NAME", 9) == 0) {
        room->name = parseRoomName(&buffer[11]);
    }
    else {
        fprintf(stderr, "Unknown room name %s found in %s.\n",
                &buffer[11], roomPath);
        exit(1);
    }

    // Read the connections and convert to indices
    readLine(fd, buffer, sizeof(buffer));
    i = 0;
    while (strncmp(buffer, "CONNECTION", 10) == 0) {
        room->connects[i] = parseRoomName(&buffer[14]);

        // print error message if room name not found
        if (room->connects[i] == -1) {
            fprintf(stderr, "Connection to unknown room %s found in %s.\n",
                    &buffer[14], roomPath);
            exit(1);
        }

        ++i;

        // read the next line for next iteration of loop
        readLine(fd, buffer, sizeof(buffer));
    }

    // set connection count based on number of loop iterations
    room->connectCnt = i;

    // the room type should be in the buffer now
    if (strncmp(buffer, "ROOM TYPE", 9) == 0) {
        room->type = parseRoomType(&buffer[11]);
    }
    else {
        fprintf(stderr, "Unrecognized room type %s found in %s.\n",
                &buffer[11], roomPath);
        exit(1);
    }

    // close the file descriptor
    close(fd);

    // return true because the load succeeded
    return 1;
}

/**
 * Converts a room name from a string to an index in RoomNameStrings.
 *
 *  input   Room name string to convert.
 *
 * Returns the index of the specified room name, or -1 if unknown.
 */
int parseRoomName(char* input) {
    int i;

    for (i = 0; i < ROOM_NAME_COUNT; ++i) {
        if (strcmp(input, RoomNameStrings[i]) == 0) return i;
    }

    /* if no matching room is found, return -1 */
    return -1;
}

/**
 * Converts a room type from a string to an index in RoomTypeStrings.
 *
 *  input   Room type string to convert.
 *
 * Returns the index of the specified room type, or -1 if unknown.
 */
int parseRoomType(char* buf) {
    int i;
    for (i = 0; i < ROOM_TYPE_COUNT; ++i) {
        if (strcmp(buf, RoomTypeStrings[i]) == 0) return i;
    }

    /* if no matching room type found, return -1 to indicate invalid type */
    return -1;
}

/**
 * Reads from fd until the first newline or EOF and writes it to buf.
 * Points fd to next unread non-whitespace char.
 * Terminates the program with exit code 1 if cannot read from file.
 *
 *  fd      File descriptor to read from. Must be open and readable.
 *  buf     Buffer to store line of text into.
 *  size    Maximum amount of data to read into buffer.
 */
void readLine(int fd, char* buf, size_t size) {
    int i;
    ssize_t bytes = read(fd, buf, size);

    if (bytes < 0) {
        fprintf(stderr, "Error reading from file. (errno %d)", errno);
        exit(1);
    }
    // search string for newline and replace with NULL terminator,
    // then count until non-whitespace char is found
    // and set the file offset to that character.
    for (i = 0; i < bytes; ++i) {
        if (buf[i] == '\n' || buf[i] == '\r') {
            buf[i] = '\0';
            while (++i < bytes
                && (buf[i] == ' '
                    || buf[i] == '\n'
                    || buf[i] == '\t'
                    || buf[i] == '\r')
                ) {
                buf[i] = '\0';
            }
            lseek(fd, -(bytes - i), SEEK_CUR);
            return;
        }
    }

}

/**
 * Saves the rooms array to files in the specified directory.
 * Terminates the program with exit code 1 if the write fails.
 *
 *  rooms   Populated array of room data to save.
 *  count   Number of rooms stored in the array.
 *  root    Path to store the room data in.
 */
void saveRooms(Room rooms[], int count, char* root) {
    int i, j;
    int fd;
    char roomPath[MAX_PATH_LEN]; // path to store room files in
    char buffer[BUFFER_SIZE];    // stores file data before writing

    // try to make the root directory
    if (mkdir(root, S_IRWXU) < 0 && errno != EEXIST) {
        // failed to create directory and it does not already exist
        fprintf(stderr, "Error creating directory: %s (errno %d)", roomPath, errno);
        exit(1);
    }

    for (i = 0; i < count; ++i) {
        // build path to filename of <username>.rooms.<pid>/room<room_no>
        snprintf(roomPath, sizeof(roomPath), "%s/room%d", root, rooms[i].name);

        // open the generated file path for writing
        fd = open(roomPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

        // verify file descriptor and write to stderr if invalid
        if (fd == -1) {
            fprintf(stderr, "Error writing to file: %s (errno %d)", roomPath, errno);
            exit(1);
        }

        // write the room data to the buffer and then to the file
        snprintf(buffer, sizeof(buffer), "ROOM NAME: %s\n",
            RoomNameStrings[rooms[i].name]);
        if (!write(fd, buffer, strlen(buffer))) {
            fprintf(stderr, "Error writing to file: %s (errno %d)", roomPath, errno);
            exit(1);
        }
        for (j = 0; j < rooms[i].connectCnt; ++j) {
            snprintf(buffer, sizeof(buffer), "CONNECTION %d: %s\n",
                j + 1, RoomNameStrings[rooms[i].connects[j]]);
            if (!write(fd, buffer, strlen(buffer))) {
                fprintf(stderr, "Error writing to file: %s (errno %d)",
                    roomPath, errno);
                exit(1);
            }
        }
        snprintf(buffer, sizeof(buffer), "ROOM TYPE: %s\n",
            RoomTypeStrings[rooms[i].type]);
            if (!write(fd, buffer, strlen(buffer))) {
                fprintf(stderr, "Error writing to file: %s (errno %d)", roomPath, errno);
                exit(1);
            }

        // close the file descriptor and flush the cache
        close(fd);
    }
}

/**
 * Randomly shuffles the values in an integer array.
 *
 *  vals    Integer array to shuffle.
 *  size    Number of elements in the integer array.
 */
void shuffleIntArray(int vals[], int size) {
    int i;
    int j;

    for (i = 0; i < size; ++i) {
        j = rand() % size;
        if (i != j)
            swap(&vals[i], &vals[j]);
    }
}

/**
 * Starts the main game loop.
 * The total path travelled must be no more than MAX_MOVES.
 *
 *  root    Path to the directory containing the room data.
 */
void startGame(char* root) {
    Room current;
    char buffer[BUFFER_SIZE];
    int path[MAX_MOVES];
    int moves = 0;
    int nextRoom;
    int i;

    // initialize path array to -1
    initializeIntArray(path, MAX_MOVES, -1);

    // find and load the start room
    getStartRoom(root, &current);

    // continually prompt the user for a room name
    // and attempt to load it until the end room is reached.
    while (current.type != RoomType_END_ROOM) {
        printf("\nCURRENT LOCATION: %s\n", RoomNameStrings[current.name]);
        printf("POSSIBLE CONNECTIONS: ");
        for (i = 0; i < current.connectCnt; ++i) {
            if (i == current.connectCnt - 1)
                printf("%s.\n", RoomNameStrings[current.connects[i]]);
            else
                printf("%s, ", RoomNameStrings[current.connects[i]]);
        }
        printf("WHERE TO? >");

        // get next line of input from user
        getInput(buffer, sizeof(buffer));

        // parse input to get room index
        nextRoom = parseRoomName(buffer);

        // if nextRoom is a valid connection, load it, store it in path,
        // and increment the path counter; otherwise, print HUH?
        if (contains(current.connects, current.connectCnt, nextRoom)
            && loadRoom(&current, nextRoom, root)) {
            path[moves++] = nextRoom;
        }
        else {
            printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
        }
    }

    // display the congratulations message and path taken
    printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
    printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", moves);
    for (i = 0; i < moves; ++i) {
        printf("%s\n", RoomNameStrings[path[i]]);
    }

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
