#ifndef __fibonacci_h__
#define __fibonacci_h__

// recursive function with dynamic programming
int fibonacci_rdp( int count );

// iterative function with dynamic programming
int fibonacci_idp( int count );

// iterative function with exponentiation of fibonacci matrix 
int fibonacci_mat( int count );

// recursive function using fast doubling relation of fibonacci formula
int fibonacci_fd( int count );

#endif