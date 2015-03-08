/*************************************************************************
 * Author:          David Rigert
 * Date Created:    3/7/2015
 * Last Modified:   3/7/2015
 * Assignment:      Lab 10
 * Filename:        lab10.cpp
 *
 * Description:     Compares the execution time of a recursive Fibonacci
 *                  function and an iterative Fibonacci function.
 *                  Then compares recursion and tail recursion versions of
 *                  a factorial function.
 * Input:           A series of integers, until a non-integer is entered
 * Output:          The same series of integers reversed for the stack and
 *                  in the same order from the queue.
 ************************************************************************/
#include <iostream>     // std::cin, std::cout, std::getline
#include <string>       // std::string
#include <chrono>

// typedef to make high_resolution_clock shorter
typedef std::chrono::high_resolution_clock hrc;

// function prototypes
long RecursionFib(int);
long TailRecursionFib(int);
long TailRecursionFib(int, long, long);
long IterationFib(int);
long rfactorial(int);
long factorialHelper(int, long);
long factorial(int);

int main()
{
    std::cout << "This program compares the execution time of recursive,\n"
              << "tail recursive, and iterative Fibonacci functions.\n"
              << "Then it compares the execution time of recursive and\n"
              << "tail recursive factorial functions.\n";
    
    long result;
    
    /****************************************************************
        Fibonacci number comparison
        Start with 20 and then double it to 40. 
    ****************************************************************/
    std::cout << "\nFinding the 20th Fibonacci number...\n\n";

    // recursive Fibonacci function
    hrc::time_point t1 = hrc::now();    // save clock value at start
    result = RecursionFib(20);
    hrc::time_point t2 = hrc::now();    // save clock value at stop
    
    std::cout << "RecursionFib found " << result << " for the 20th Fibonacci number.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";
    
    // tail recursive Fibonacci function
    t1 = hrc::now();    // save clock value at start
    result = TailRecursionFib(20);
    t2 = hrc::now();    // save clock value at stop
    
    std::cout << "TailRecursionFib found " << result << " for the 20th Fibonacci number.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";

    // iterative Fibonacci function
    t1 = hrc::now();    // save clock value at start
    result = IterationFib(20);
    t2 = hrc::now();    // save clock value at stop
    
    std::cout << "IterationFib found " << result << " for the 20th Fibonacci number.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";

    std::cout << "\nFinding the 40th Fibonacci number...\n\n";

    // recursive Fibonacci function
    t1 = hrc::now();    // save clock value at start
    result = RecursionFib(40);
    t2 = hrc::now();    // save clock value at stop
    
    std::cout << "RecursionFib found " << result << " for the 40th Fibonacci number.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";
    
    // tail recursive Fibonacci function
    t1 = hrc::now();    // save clock value at start
    result = TailRecursionFib(40);
    t2 = hrc::now();    // save clock value at stop
    
    std::cout << "TailRecursionFib found " << result << " for the 40th Fibonacci number.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";

    // iterative Fibonacci function
    t1 = hrc::now();    // save clock value at start
    result = IterationFib(40);
    t2 = hrc::now();    // save clock value at stop
    
    std::cout << "IterationFib found " << result << " for the 40th Fibonacci number.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";

    /****************************************************************
        Factorial comparison
        Start with 8 and then double it to 16. 
    ****************************************************************/
    std::cout << "\nFinding 8!\n\n";

    // recursive Factorial function
    t1 = hrc::now();    // save clock value at start
    result = rfactorial(8);
    t2 = hrc::now();    // save clock value at stop
    
    std::cout << "Recursive factorial found " << result << " for 8!.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";
    
    // tail recursive factorial function
    t1 = hrc::now();    // save clock value at start
    result = factorial(8);
    t2 = hrc::now();    // save clock value at stop
    
    std::cout << "Tail recursive factorial found " << result << " for 8!.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";

    
    std::cout << "\nFinding 16!\n\n";

    // recursive Factorial function
    t1 = hrc::now();    // save clock value at start
    result = rfactorial(16);
    t2 = hrc::now();    // save clock value at stop
    
    std::cout << "Recursive factorial found " << result << " for 16!.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";
    
    // tail recursive factorial function
    t1 = hrc::now();    // save clock value at start
    result = factorial(16);
    t2 = hrc::now();    // save clock value at stop
    
    std::cout << "Tail recursive factorial found " << result << " for 16!.\n";
    std::cout << "This took "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() 
              << " microseconds\n";

    return 0;
}

/**********************************************************
 *  long RecursionFib(int n)
 *
 *  From CS161 Assignment 5
 *
 *  Purpose: This function recursively gets the nth Fibonacci number.
 *
 *  Preconditions: n >= 0
 *   
 *  Postconditions: Returns the nth Fibonacci number
 *********************************************************/
long RecursionFib(int n)
{
    if (n < 2) // Base case
    {
        return 1;
    }
    else        // Recursive case
    {
        return RecursionFib(n - 2) + RecursionFib(n - 1);
    }
}

// Gets the nth Fibonacci number using tail recursion
long TailRecursionFib(int n)
{
    return TailRecursionFib(n, 1, 1);
}

long TailRecursionFib(int n, long current, long previous)
{
    if (n == 0) // Base case
    {
        return previous;
    }
    else if (n == 1)
    {
        return current;
    }
    else       // Recursive case
    {
        return TailRecursionFib(n - 1, current + previous, current);
    }
}

// Gets the nth Fibonacci number using an iterative approach
long IterationFib(int n)
{
    long n1 = 1;
    long n2 = 1;
    long result = 0;
    for (int i = 1; i < n; ++i)
    {
        result = n1 + n2;
        n1 = n2;
        n2 = result;
    }
    return result;
}

// The following functions are from Lab10.pdf
// Not Tail Recursive
long rfactorial (int n)
{
    if (n == 1)
        return 1;
    else
        return n * rfactorial(n-1);
}   
// Tail Recursive
long factorialHelper (int n, long result)
{
    if (n == 1)
        return result;
    else
        return factorialHelper(n-1,n*result);
}
long factorial(int n) 
{
    return factorialHelper(n,1); 
}
