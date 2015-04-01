/* CS261- Assignment 1 - Q.3*/
/* Name: David Rigert
 * Date: March 31, 2015
 * Solution description: Displays a randomly generated array of 20 ints,
 *                       sorts it with selection sort, and displays the
 *                       sorted array.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void sort(int* number, int n){
    /*Sort the given array number , of length n*/
    int i = 0;
    /* loop through every element from i to n-2 */
    while (i < n - 1) {
        /* find index of minimum value from i+1 to n-1 */
        int j = i + 1;
        /* assume i is smallest */
        int minIndex = i;
        while (j < n) {
            /* set minIndex to j if value is less than current minimum */
            if (number[j] < number[minIndex]) {
                minIndex = j;
            }
            ++j;
        }
        /* minIndex is minimum value from i+1 to n-1 */
        
        /* swap element i with minIndex if i is smaller */
        if (number[minIndex] < number[i]) {
            int temp = number[i];
            number[i] = number[minIndex];
            number[minIndex] = temp;
        }
        /* element i contains minimum value from i to n-1 */
        /* values in elements 0 to i-1 are sorted */
        
        ++i;
    }
}

int main(){
    /*Declare an integer n and assign it a value of 20.*/
    int n = 20;
    
    /*Allocate memory for an array of n integers using malloc.*/
    int *pArray = malloc(n * sizeof(int));
    
    /*Fill this array with random numbers, using rand().*/
    srand(time(NULL));
    int i;
    for (i = 0; i < n; ++i) {
        pArray[i] = rand();
    }
    
    /*Print the contents of the array.*/
    printf("Unsorted array:\n");
    for (i = 0; i < n; ++i) {
        printf("%5d   ", pArray[i]);
    }

    /*Pass this array along with n to the sort() function of part a.*/
    sort(pArray, n);
    
    /*Print the contents of the array.*/    
    printf("\nSorted array:\n");
    for (i = 0; i < n; ++i) {
        printf("%5d   ", pArray[i]);
    }
    
    return 0;
}
