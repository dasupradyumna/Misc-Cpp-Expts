// Implementations of data structures described in `structs.h`

#include "structs.h"

#include <cstdint>          // for `std::int_fast16_t`, `std::int_fast32_t` and `std::int_fast64_t`
#include <iostream>         // for `std::cin`, `std::cout` and `std::cerr`
#include <stdexcept>        // for `std::out_of_range`

//////////////////////////////////// 2D Flat Rectangular Array //////////////////////////////////////

////////// Array2d::Row //////////

template<typename NUM_TYPE>
Array2d<NUM_TYPE>::Row::Row( const Array2d* link ) :
    __row { nullptr },
    __link { link }
{}

template<typename NUM_TYPE>
NUM_TYPE& Array2d<NUM_TYPE>::Row::operator[]( const size_t col ) const
{
    if ( col >= __link->__cols )
        throw std::out_of_range { "error: column index out of bounds.\n" };

    return __row[col];
}

////////// Array2d //////////

template<typename NUM_TYPE>
Array2d<NUM_TYPE>::Array2d( const size_t rows, const size_t cols ) :
    __rows { rows },
    __cols { cols },
    __data { new NUM_TYPE[rows * cols] { 0 } },
    __ref { new Row { this } }
{}

template<typename NUM_TYPE>
Array2d<NUM_TYPE>::~Array2d()
{
    delete[] __data;
    delete __ref;
}

template<typename NUM_TYPE>
const size_t Array2d<NUM_TYPE>::rows() const { return __rows; }

template<typename NUM_TYPE>
const size_t Array2d<NUM_TYPE>::cols() const { return __cols; }

/* Returns a non-const iterator so that the loop variable inside a range-based for loop
 * has the option to be modifiable or read-only, depending on its type.
 */
template<typename NUM_TYPE>
NUM_TYPE* Array2d<NUM_TYPE>::begin() const { return __data; }

// Returns a const iterator to the end of the array, since it is used only for bound-checking
template<typename NUM_TYPE>
const NUM_TYPE* const Array2d<NUM_TYPE>::end() const { return __data + __rows * __cols; }

/* This function is called for the first index (rows) of the array.
 * It returns a pointer to a `NUM_TYPE` value, which calls the subscript method with the
 * second index (columns). Finally, it returns the desired value from the array.
 */
template<typename NUM_TYPE>
typename Array2d<NUM_TYPE>::Row& Array2d<NUM_TYPE>::operator[]( const size_t row ) const
{
    if ( row < __rows )
        __ref->__row = &__data[row * __cols];
    else
        throw std::out_of_range { "error: row index out of bounds.\n" };

    return *__ref;
}

/* Instantiating template classes for the datatypes that can be used with the `Array2d` class.
 * Not doing this will throw a linker error since it can not find the type-specific implementation
 * of the methods it sees in template class anywhere. By compiling the below type-instances, the linker
 * will be able to find the desired variation of template.
 */
template class Array2d<std::int_fast32_t>;
template class Array2d<std::int_fast64_t>;
template class Array2d<float>;
template class Array2d<double>;


// simple test function for `Array2d` demo
void testArray2d()
{
    const Array2d<double> A { 3, 2 };

    // for ( auto& el : A )                        // testing for-each loop implementation
    //     el = rand() / 1000.0;

    // while ( true )                              // testing traditional [][] indexing with bounds check
    // {
    //     int r { };
    //     size_t c { };
    //     std::cout << "Enter row and column indices : ";
    //     std::cin >> r >> c;
    //     if ( r < 0 ) break;
    //     try
    //     {
    //         std::cout << A[static_cast<size_t>(r)][c] << '\n';
    //     }
    //     catch ( std::out_of_range& exception )
    //     {
    //         std::cerr << exception.what();
    //     }
    // }

    for ( size_t i { 0ULL }; i < A.rows(); ++i )      // testing rows() and cols() methods
    {
        for ( size_t j { 0ULL }; j < A.cols(); ++j )
            std::cout << A[i][j] << ' ';
        std::cout << '\n';
    }
}


///////////////////////////////// Python-like Range-based Iterator ///////////////////////////////////

////////// IntRange::Iterator //////////

template<typename INT_TYPE>
IntRange<INT_TYPE>::Iterator::Iterator( const INT_TYPE value, const INT_TYPE step ) :
    __value { value },
    __step { step }
{}

// Compares the value of the iterator with the direct value of `__end` variable
template<typename INT_TYPE>
bool IntRange<INT_TYPE>::Iterator::operator!=( const INT_TYPE other )
{
    return this->__value != other;
}

// Indirection should yield the value contained, not the address of wrapper
template<typename INT_TYPE>
INT_TYPE IntRange<INT_TYPE>::Iterator::operator*() { return __value; }

// `typename` keyword is necessary for compiler because the name of the return type is dependent on the template
template<typename INT_TYPE>
typename IntRange<INT_TYPE>::Iterator& IntRange<INT_TYPE>::Iterator::operator++()
{
    __value += __step;
    return *this;
}

////////// IntRange //////////

template<typename INT_TYPE>
IntRange<INT_TYPE>::IntRange( const INT_TYPE begin, const INT_TYPE end, const INT_TYPE step ) :
    __begin { begin },
    __end { end },
    __step { (__begin < __end) ? step : -step }
{}

template<typename INT_TYPE>
IntRange<INT_TYPE>::IntRange( const INT_TYPE end ) :
    __begin { 0 },
    __end { end },
    __step { (__end > 0) ? 1 : -1 }
{}

// `typename` keyword is necessary for compiler because the name of the return type is dependent on the template
template<typename INT_TYPE>
typename IntRange<INT_TYPE>::Iterator IntRange<INT_TYPE>::begin() { return Iterator { __begin, __step }; }

template<typename INT_TYPE>
const INT_TYPE IntRange<INT_TYPE>::end() { return __end; }

// template class instantiations for desired datatypes
template class IntRange<std::int_fast32_t>;
template class IntRange<std::int_fast64_t>;


// simple test function for `IntRange` demo
void testIntRange()
{
    int count { 0 };
    for ( const auto num : IntRange<int> { 2'000, 5 } )
    {
        std::cout << num << '\t';
        if ( ++count % 10 == 0 ) std::cout << '\n';
    }
}
