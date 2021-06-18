// Implementations of algorithms described in `fibonacci.h`

#include "fibonacci.h"

#include <vector>

int fibonacci_rdp( int count )
{
    static std::vector<int> series { 0, 1 };

    if ( count >= series.size() )
        series.push_back(
            fibonacci_rdp( count - 1 ) +
            fibonacci_rdp( count - 2 )
        );

    return series[count];
}

int fibonacci_idp( int count )
{
    // if ( count == 0 ) return 0;
    static std::vector<int> series { 0, 1 };

    // int A { 0 };
    // int B { 1 };

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

int fibonacci_mat( int count )
{
    if ( count == 0 ) return 0;

    auto mat_mult_accumulate = [ ] ( int A[2][2], int B[2][2] )
    {
        A[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
        A[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
        A[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
        A[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];
    };

    int M[2][2] {
        { 1, 1 },
        { 1, 0 }
    };
    int result[2][2] { 0 };

    while ( count )
    {
        if ( count & 1 )
            mat_mult_accumulate( result, M );

        count >>= 1;

        mat_mult_accumulate( M, M );
    }

    return M[0][0];
}

int fibonacci_fd( int count )
{
    return 0;
}
