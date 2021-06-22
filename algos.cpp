// Implementations of algorithms described in `algos.h`

#include "algos.h"

// Exponentiation by Squaring
std::int_fast64_t exp_by_sqr( int base, int exp )
{
  std::int_fast64_t result { 1 };
  while ( exp )
  {
    if ( exp & 1 )
      result *= base;

    exp >>= 1;
    base *= base;
  }

  return result;
}
