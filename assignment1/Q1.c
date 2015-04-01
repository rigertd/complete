/* CS261- Assignment 1 - Q.1*/
/* Name: David Rigert
 * Date: March 31, 2015
 * Solution description: Allocates a dynamic array of 10 student structs.
 *                       Generates a unique ID from 1 to 10 for each struct.
 *                       Assigns a random score between 0 and 100.
 *                       Displays the ID and score of each struct.
 *                       Displays the minimum/maximum/average score.
 *                       Deallocates the array.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct student{
    int id;
    int score;
};

struct student* allocate(){
    /*Allocate memory for ten students*/
    struct student *array = malloc(10 * sizeof(struct student));
    
    /*return the pointer*/
    return array;
}

void generate(struct student* students){
    /*Generate random ID and scores for ten students, ID being between 1 and 10, scores between 0 and 100*/
    srand(time(NULL));    /* seed the random number generator */
    unsigned i, j;
    
    for (i = 0; i < 10; ++i) {
        students[i].id = rand() % 10 + 1;    /* between 1 and 10 */
        
        /* make sure id is unique */
        for (j = 0; j < i; ++j) {
            if (students[i].id == students[j].id) {
                --i;    /* reassign current id */
                break;
            }
        }
        
        students[i].score = rand() % 101;    /* between 0 and 100 */
    }
}

void output(struct student* students){
    /*Output information about the ten students in the format:
             ID1 Score1
             ID2 score2
             ID3 score3
             ...
             ID10 score10*/
    unsigned i;
    for (i = 0; i < 10; ++i) {
        printf("%2d %3d\n", students[i].id, students[i].score);
    }
}

void summary(struct student* students){
    /*Compute and print the minimum, maximum and average scores of the ten students*/
    int minimum = 100;  /* set initial minimum to max score */
    int maximum = 0;    /* set initial maximum to min score */
    int total = 0;
    float avg = 0.0;
    unsigned i;
    
    for (i = 0; i < 10; ++i) {
        minimum = minimum < students[i].score ? minimum : students[i].score;
        maximum = maximum > students[i].score ? maximum : students[i].score;
        total += students[i].score;
    }
    
    /* compute the average */
    avg = (float)total / 10.;
    
    /* print the results */
    printf("Minimum score: %d, maximum score: %d, average score: %.1f\n", minimum, maximum, avg);
}

void deallocate(struct student* stud){
    /*Deallocate memory from stud*/
    if (stud) {
        free(stud);
    }
}

int main(){
    struct student* stud = NULL;
    
    /*call allocate*/
    stud = allocate();
    
    /*call generate*/
    generate(stud);
    
    /*call output*/
    output(stud);
    
    /*call summary*/
    summary(stud);
    
    /*call deallocate*/
    deallocate(stud);

    return 0;
}
