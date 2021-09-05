#ifndef __fibonacci_h__
#define __fibonacci_h__

#include <functional>       // std::function

using size_t = std::size_t;

// recursive function with dynamic programming
int fibonacci_rdp( size_t count );

// iterative function with dynamic programming
int fibonacci_idp( size_t count );

// iterative function with exponentiation of fibonacci matrix 
int fibonacci_mat( size_t count );

// recursive function using fast doubling relation of fibonacci formula
int fibonacci_fd( size_t count );

// simple test function
void testFibonacci( std::function<int( size_t )> fibonacci, int count );

#endif