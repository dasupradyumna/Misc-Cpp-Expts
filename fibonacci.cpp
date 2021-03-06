// Implementations of algorithms described in `fibonacci.h`

#include "fibonacci.h"
#include "structs.h"        // Matrix

#include <iostream>         // std::cout
#include <vector>           // std::vector

int fibonacci_rdp( size_t count )  // exponential time
{
  // stores the calculated values of the fibonacci series
  static std::vector<int> series { 0, 1 };

  // if value has not been calculated previously, add it
  if ( count >= series.size() )
    series.push_back(
      fibonacci_rdp( count - 1 ) +
      fibonacci_rdp( count - 2 )
    );

  return series[count];
}

// commented code for non-memoized algorithm
int fibonacci_idp( size_t count )  // linear time
{
  // if ( count == 0 ) return 0;
  // stores the calculated values of the fibonacci series
  static std::vector<int> series { 0, 1 };

  // int A { 0 };
  // int B { 1 };

  // iteratively calculate fibonacci series, if not in stored vector
  auto N { series.size() };
  while ( count >= N ) // ( --count )
  {
    series.push_back(
      series[N - 1] + series[N - 2]
    );
    ++N;
    // B = A + B;
    // A = B - A;
  }

  return series[count]; // B;
}

int fibonacci_mat( size_t count )
{
  // return count as is for 0, 1
  if ( count < 2 ) return static_cast<int>(count);

  // array that contains the fibonacci matrix to be exponentiated
  Matrix<int> M { 2, 2, {
    { 1, 1 },
    { 1, 0 }
  } };

  // array that contains the result matrix
  Matrix<int> R { 2, 2, {
    { 1, 0 },
    { 0, 1 }
  } };

  --count;                // Fibonacci(count) = (M ^ count-1)[0][0]
  while ( count )         // exponentiate M to count, but in log(count) time
  {                       // using exponentiation by squaring algorithm
    if ( count & 1 )
      R *= M;

    count >>= 1;
    M *= M;
  }

  // first element is required fibonacci number
  return R[0][0];
}

/* Calculate recursively fibonacci series, with memoization
 * F(2k) = F(k)^2 + 2*F(k)*F(k-1), even doubling
 * F(2k+1) = F(k)^2 + F(k+1)^2, odd doubling
 */
int fibonacci_fd( size_t count )
{
  // stores the calculated values of the fibonacci series
  static std::vector<int> series { 0, 1 };

  // if value has not been calculated previously, add it
  if ( count >= series.size() )
  {
    size_t k = count >> 1;
    series.push_back(
      // doubling formula obtained from fibonacci series
      fibonacci_fd( k ) * fibonacci_fd( k ) +
      (count & 1
        ? fibonacci_fd( k + 1 ) * fibonacci_fd( k + 1 )
        : fibonacci_fd( k ) * fibonacci_fd( k - 1 ) << 1)
    );
  }

  return series[count];
}

void testFibonacci( std::function<int( size_t )> fibonacci, int count )
{
  for ( int i { 0 }; i < count; i++ )
    std::cout << fibonacci( i ) << '\n';
  std::cout << '\n';
}
