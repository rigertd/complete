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
 ************************************************************************/
#include <iostream>     // std::cin, std::cout, std::getline
#include <iomanip>      // std::setw
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
              << "tail recursive factorial functions.\n\n";
    
    long result;        // holds result
    hrc::time_point t1; // holds start time
    hrc::time_point t2; // holds stop time
    
    
    /****************************************************************
        Fibonacci number comparison
        Start with 30 and then increment 1 at a time up to 40. 
    ****************************************************************/
    // display header row
    std::cout << "Execution Times of Fibonacci Functions (in microseconds)\n\n";
    std::cout << std::setw(5) << "n" << std::setw(20) << "RecursionFib"
              << std::setw(20) << "TailRecursionFib"
              << std::setw(20) << "IterationFib" << std::endl;
              
    for (int i = 30; i <= 40; i++)
    {
        std::cout << std::setw(5) << i;
        
        // recursive Fibonacci function
        t1 = hrc::now();    // save clock value at start
        result = RecursionFib(i);
        t2 = hrc::now();    // save clock value at stop
        
        std::cout << std::setw(20) 
                  << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        
        // tail recursive Fibonacci function
        t1 = hrc::now();    // save clock value at start
        result = TailRecursionFib(20);
        t2 = hrc::now();    // save clock value at stop
        
        std::cout << std::setw(20) 
                  << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        // iterative Fibonacci function
        t1 = hrc::now();    // save clock value at start
        result = IterationFib(20);
        t2 = hrc::now();    // save clock value at stop
        
        std::cout << std::setw(20) 
                  << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
                  
        std::cout << std::endl;
    }

    /****************************************************************
        Factorial comparison
        Start with 100 and increment by 100 up to 1000.
    ****************************************************************/
    std::cout << "\nExecution Times of Factorial Functions (in microseconds)\n\n";
    // display header row
    std::cout << std::setw(5) << "n" << std::setw(20) << "rfactorial"
              << std::setw(20) << "factorial" << std::endl;

    for (int i = 100; i <= 1000; i += 100)
    {
        std::cout << std::setw(5) << i;
        
        // recursive Factorial function
        t1 = hrc::now();    // save clock value at start
        result = rfactorial(i);
        t2 = hrc::now();    // save clock value at stop
        
        std::cout << std::setw(20) 
                  << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        
        // tail recursive factorial function
        t1 = hrc::now();    // save clock value at start
        result = factorial(i);
        t2 = hrc::now();    // save clock value at stop
        
        std::cout << std::setw(20) 
                  << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        
        std::cout << std::endl;
    }

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
