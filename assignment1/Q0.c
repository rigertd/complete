/* CS261- Assignment 1 - Q. 0*/
/* Name: David Rigert
 * Date: 03/31/2015
 * Solution description: 
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>

void fooA(int* iptr){
    /*Print the value pointed to by iptr*/
    printf("In fooA, iptr points to a value of %d ", *iptr);
    
    /*Print the address pointed to by iptr*/
    printf("at address %p.\n", iptr);
    
    /*Print the address of iptr itself*/
    printf("The address of iptr itself is %p.\n", &iptr);
}

int main(){
    
    /*declare an integer x--uninitialized variables are bad*/
    int x = 42;
    
    /*print the address of x*/
    printf("In main, the address of x is %p.\n", &x);
    
    /*Call fooA() with the address of x*/
    fooA(&x);
    
    /*print the value of x*/
    printf("Back in main, the value of x is %d.\n", x);
    
    return 0;
}
