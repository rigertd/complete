#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "dynamicArray.h"

/* define pi and e */
#define PI 3.1415926535897932384626433832795
#define E 2.7182818284590452353602874713527

/* param: s the string
   param: num a pointer to double
   returns: true (1) if s is a number else 0 or false.
   postcondition: if it is a number, num will hold
   the value of the number
*/
int isNumber(char *s, double *num)
{
	char *end;
	double returnNum;

	if(strcmp(s, "0") == 0)
	{
		*num = 0;
		return 1;
	}
	else 
	{
		returnNum = strtod(s, &end);
		/* If there's anythin in end, it's bad */
		if((returnNum != 0.0) && (strcmp(end, "") == 0))
		{
			*num = returnNum;
			return 1;
		}
	}
	return 0;  //if got here, it was not a number
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	their sum is pushed back onto the stack.
*/
void add (struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operands found for + operator\n");
        exit(1);
    }
    
    /* pop right-hand value from stack */
    TYPE rhs = topDynArr(stack);
    popDynArr(stack);
    
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: Second operand not found for + operator\n");
        exit(1);
    }

    /* pop left-hand value from stack */
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    
    /* add values and push result onto stack */
    pushDynArr(stack, lhs + rhs);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	their difference is pushed back onto the stack.
*/
void subtract(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operands found for - operator\n");
        exit(1);
    }
    
    /* pop right-hand value from stack */
    TYPE rhs = topDynArr(stack);
    popDynArr(stack);
    
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: Second operand not found for - operator\n");
        exit(1);
    }

    /* pop left-hand value from stack */
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    
    /* subtract values and push result onto stack */
    pushDynArr(stack, lhs - rhs);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	their quotient is pushed back onto the stack.
*/
void divide(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operands found for / operator\n");
        exit(1);
    }
    
    /* pop right-hand value from stack */
    TYPE rhs = topDynArr(stack);
    popDynArr(stack);
    
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: Second operand not found for / operator\n");
        exit(1);
    }
    
    /* pop left-hand value from stack */
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    
    /* test for divide by 0 error */
    if (rhs == 0) {
        printf("Invalid operand: Cannot divide %f by 0\n", lhs);
        exit(1);
    }
    
    /* divide values and push result onto stack */
    pushDynArr(stack, lhs / rhs);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	their product is pushed back onto the stack.
*/
void multiply(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operands found for x operator\n");
        exit(1);
    }
    
    /* pop right-hand value from stack */
    TYPE rhs = topDynArr(stack);
    popDynArr(stack);
    
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: Second operand not found for x operator\n");
        exit(1);
    }
    
    /* pop left-hand value from stack */
    TYPE lhs = topDynArr(stack);
    popDynArr(stack);
    
    /* multiply values and push result onto stack */
    pushDynArr(stack, lhs * rhs);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least two elements
	post: the top two elements are popped and 
	the second raised to the first is pushed back onto the stack.
*/
void power(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operands found for ^ operator\n");
        exit(1);
    }
    
    /* pop exponent value from stack */
    TYPE ex = topDynArr(stack);
    popDynArr(stack);
    
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: Second operand not found for ^ operator\n");
        exit(1);
    }
    
    /* pop base value from stack */
    TYPE base = topDynArr(stack);
    popDynArr(stack);
    
    /* raise base to the exponent and push result onto stack */
    pushDynArr(stack, pow(base, ex));
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	the value squared is pushed back onto the stack.
*/
void squared(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operand found for ^2 operator\n");
        exit(1);
    }
    
    /* pop value from stack */
    TYPE val = topDynArr(stack);
    popDynArr(stack);
    
    /* square the value and push result onto stack */
    pushDynArr(stack, val * val);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	the value cubed is pushed back onto the stack.
*/
void cubed(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operand found for ^3 operator\n");
        exit(1);
    }
    
    /* pop value from stack */
    TYPE val = topDynArr(stack);
    popDynArr(stack);
    
    /* cube the value and push result onto stack */
    pushDynArr(stack, val * val * val);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	the absolute value is pushed back onto the stack.
*/
void absolute(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operand found for abs operator\n");
        exit(1);
    }
    
    /* pop value from stack */
    TYPE val = topDynArr(stack);
    popDynArr(stack);
    
    /* multiply value by -1 if negative and push result onto stack */
    pushDynArr(stack, val < 0 ? -1 * val : val);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one non-negative element
	post: the top element is popped and 
	the square root is pushed back onto the stack.
*/
void squareroot(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operand found for sqrt operator\n");
        exit(1);
    }
    
    /* pop value from stack */
    TYPE val = topDynArr(stack);
    popDynArr(stack);
    
    /* test for negative number */
    if (val < 0) {
        printf("Invalid operand: Cannot find square root of negative number %f\n", val);
        exit(1);
    }
    
    /* find square root and push result onto stack */
    pushDynArr(stack, sqrt(val));
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one element
	post: the top element is popped and 
	e raised to the value is pushed back onto the stack.
*/
void exponent(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operand found for exp operator\n");
        exit(1);
    }
    
    /* pop exponent from stack */
    TYPE ex = topDynArr(stack);
    popDynArr(stack);
    
    /* push E and exponent on stack */
    pushDynArr(stack, E);
    pushDynArr(stack, ex);
    
    /* call power function */
    power(stack);
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one non-negative element
	post: the top element is popped and 
	the natural log is pushed back onto the stack.
*/
void log_n(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operand found for ln operator\n");
        exit(1);
    }
    
    /* pop value from stack */
    TYPE val = topDynArr(stack);
    popDynArr(stack);
    
    /* test for negative value */
    if (val < 0) {
        printf("Invalid operand: Cannot find natural log of negative number %f\n", val);
        exit(1);
    }
    
    /* find natural log and push result onto stack */
    pushDynArr(stack, log(val));
}

/*	param: stack the stack being manipulated
	pre: the stack contains at least one non-negative element
	post: the top element is popped and 
	the base-10 log is pushed back onto the stack.
*/
void log_10(struct DynArr *stack)
{
    /* test for empty stack */
    if (isEmptyDynArr(stack)) {
        printf("Invalid input: No operand found for log operator\n");
        exit(1);
    }
    
    /* pop value from stack */
    TYPE val = topDynArr(stack);
    popDynArr(stack);
    
    /* test for negative value */
    if (val < 0) {
        printf("Invalid operand: Cannot find base-10 log of negative number %f\n", val);
        exit(1);
    }
    
    /* divide values and push result onto stack */
    pushDynArr(stack, log10(val));
}

double calculate(int numInputTokens, char **inputString)
{
	int i;
	double result = 0.0;
	char *s;
	struct DynArr *stack;

	//set up the stack
	stack = createDynArr(20);

	// start at 1 to skip the name of the calculator calc
	for(i=1;i < numInputTokens;i++) 
	{
		s = inputString[i];

		// Hint: General algorithm:
		// (1) Check if the string s is in the list of operators.
		//   (1a) If it is, perform corresponding operations.
		//   (1b) Otherwise, check if s is a number.
		//     (1b - I) If s is not a number, produce an error.
		//     (1b - II) If s is a number, push it onto the stack

		if(strcmp(s, "+") == 0)
			add(stack);
		else if(strcmp(s,"-") == 0)
			subtract(stack);
		else if(strcmp(s, "/") == 0)
			divide(stack);
		else if(strcmp(s, "x") == 0)
			multiply(stack);
		else if(strcmp(s, "^2") == 0)
            squared(stack);
		else if(strcmp(s, "^3") == 0)
            cubed(stack);
		else if(strcmp(s, "^") == 0)
            power(stack);
		else if(strcmp(s, "abs") == 0)
            absolute(stack);
		else if(strcmp(s, "sqrt") == 0)
            squareroot(stack);
		else if(strcmp(s, "exp") == 0)
			exponent(stack);
		else if(strcmp(s, "ln") == 0)
			log_n(stack);
		else if(strcmp(s, "log") == 0)
			log_10(stack);
		else 
		{
            if (strcmp(s, "e") == 0) {
                /* found e */
                pushDynArr(stack, E);
            } else if (strcmp(s, "pi") == 0) {
                /* found pi */
                pushDynArr(stack, PI);
            } else if (isNumber(s, &result)) {
                /* found a number */
                pushDynArr(stack, result);
            } else {
                /* invalid input */
                printf("Invalid input: %s is not a valid number or operator\n", s);
                exit(1);
            }
			// FIXME: You need to develop the code here (when s is not an operator)
			// Remember to deal with special values ("pi" and "e")
			
		}
	}	//end for 

	/* check for result in stack */
    if (isEmptyDynArr(stack)) {
        printf("Error: Result not found\n");
        exit(1);
    }
    result = topDynArr(stack);
    
    /* pop result from stack and make sure stack is empty */
    popDynArr(stack);
    if (!isEmptyDynArr(stack)) {
        /* stack is not empty, get remaining value and display error */
        result = topDynArr(stack);
        printf("Error: Found %f still on the stack\n", result);
        exit(1);
    }
    
    /* stack is empty; return the result */
	return result;
}

int main(int argc , char** argv)
{
	// assume each argument is contained in the argv array
	// argc-1 determines the number of operands + operators
	if (argc == 1)
		return 0;

	printf("%f\n", calculate(argc,argv));
	return 0;
}
