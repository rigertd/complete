/* CS261- Assignment 1 - Q.4*/
/* Name: David Rigert
 * Date: April 1, 2015
 * Solution description: Creates an array of students with randomly generated
 *                       IDs and scores, displays the list, sorts it by score
 *                       in ascending order, and displays the sorted list.
 */
 
#include <stdio.h>
#include <stdlib.h>

struct student{
	int id;
	int score;
};

void sort(struct student* students, int n){
    /*Sort the n students based on their score*/     
    int i = 0;
    /* loop through every element from i to n-2 */
    while (i < n - 1) {
        /* find index of minimum value from i+1 to n-1 */
        int j = i + 1;
        /* assume i is smallest */
        int minIndex = i;
        while (j < n) {
            /* set minIndex to j if value is less than current minimum */
            if (students[j].score < students[minIndex].score) {
                minIndex = j;
            }
            ++j;
        }
        /* minIndex is minimum value from i+1 to n-1 */
        
        /* swap element i with minIndex if i is smaller */
        if (students[minIndex].score < students[i].score) {
            struct student temp = students[i];
            students[i] = students[minIndex];
            students[minIndex] = temp;
        }
        /* element i contains minimum value from i to n-1 */
        /* values in elements 0 to i-1 are sorted */
        
        ++i;
    }
}

void generate(struct student* students, int n){
    /*Generate random ID and scores for ten students, ID being between 1 and 10, scores between 0 and 100*/
    srand(time(NULL));    /* seed the random number generator */
    int i, j;
    
    for (i = 0; i < n; ++i) {
        students[i].id = rand() % n + 1;    /* between 1 and n */
        
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

void output(struct student* students, int n) {
    /*Output information about n students in the format:
             ID1 Score1
             ID2 score2
             ID3 score3
             ...
             ID_n score_n*/
    int i;
    for (i = 0; i < n; ++i) {
        printf("%2d %3d\n", students[i].id, students[i].score);
    }
}

int main(){
    /*Declare an integer n and assign it a value.*/
    int n = 12;
    
    /*Allocate memory for n students using malloc.*/
    struct student *students = malloc(n * sizeof(struct student));
    
    /*Generate random IDs and scores for the n students, using rand().*/
    generate(students, n);
    
    /*Print the contents of the array of n students.*/
    printf("ID Score (Unsorted)\n");
    output(students, n);

    /*Pass this array along with n to the sort() function*/
    sort(students, n);
    
    /*Print the contents of the array of n students.*/
    printf("\nID Score (Sorted)\n");
    output(students, n);
    
    /*Free the memory allocated above*/
    free(students);
    students = NULL;
    
    return 0;
}
