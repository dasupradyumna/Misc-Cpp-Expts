#ifndef __algos_h__
#define __algos_h__

#include <cstdint>          // for `std::int_fast64_t`

/* Exponentiation by Squaring - time complexity = O(log(exp))
 * Naive approach is O(exp) complexity because we multiply `base` into a result `exp` times.
 * We use the binary form of the exponent for the iterative loop. `exp` = "xyzw" (in binary)
 * `exp` = w(2^0) + z(2^1) + y(2^2) + x(2^3) implies that base ^ exp can be expanded as --
 * base ^ w * (base ^ 2) ^ z * (base ^ 4) ^ y * (base ^ 8) ^ x, where xyzw are 0/1.
 * Every iteration, `base` is squared and the corresponding bit of `exp` is used.
 */
std::int_fast64_t exp_by_sqr( int base, int exp );

#endif