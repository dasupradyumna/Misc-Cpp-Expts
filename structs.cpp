// Implementations of data structures described in `structs.h`

#include "structs.h"

#include <cstdlib>          // std::srand and std::rand
#include <cstdint>          // std::int_fast16_t, std::int_fast32_t, std::int_fast64_t
#include <ctime>            // std::time
#include <iostream>         // std::cin, std::cout, std::cerr
#include <stdexcept>        // std::out_of_range

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

/* Custom copy constructor, called whenever object is passed by value.
 * Default copy constructor would copy the pointers directly.
 * So when the copy goes out of scope, its destructor will delete the pointers allocated by original object.
 * Later, when the original goes out of scope, its destructor will try to delete the dangling pointer and crash.
 * This constructor creates a deep copy to prevent the above issue.
 */
template<typename NUM_TYPE>
Array2d<NUM_TYPE>::Array2d( const Array2d& copy ) :
    __rows { copy.__rows },
    __cols { copy.__cols },
    __data { new NUM_TYPE[__rows * __cols] { 0 } },     // would be __data { copy.__data } in default copy constructor
    __ref { new Row { this } }                          // would be __row { copy.__row} in default copy constructor
{
    for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
        __data[i] = copy.__data[i];                     // copying only the data from original's pointer to copy's distinct pointer
}

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

/* Overload of assignment operator for Array2d.
 * Rows and columns of LHS and RHS are expected to be equal before assignment.
 * All the data pointed by RHS pointer is copied to LHS pointer, takes O(n) time.
 * Pointers themselves are not copied to avoid multiple references to same data.
 */
template<typename NUM_TYPE>
void Array2d<NUM_TYPE>::operator=( const Array2d<NUM_TYPE> other )
{
    if ( this->__rows != other.__rows || this->__cols != other.__cols )
        throw std::invalid_argument { "error: source and target matrix dimensions do not match.\n" };

    // deep copying the data from RHS to LHS
    for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
        __data[i] = other.__data[i];
}

/* Overload for multiplication of two Array2d objects.
 * Checks the matrix multiplication condition of equal dimensions.
 * Naive matrix multiplication algorithm, and return the resultant Array2d object.
 */
template<typename NUM_TYPE>
Array2d<NUM_TYPE> Array2d<NUM_TYPE>::operator*( const Array2d<NUM_TYPE> other ) const
{
    if ( this->__cols != other.__rows )
        throw std::invalid_argument { "error: the matrices cannot be multipled "
        "since no. of columns of first matrix and no. of rows of second matrix are not equal.\n" };

    Array2d<NUM_TYPE> result { this->__rows, other.__cols };
    for ( size_t m { 0ULL }; m < result.__rows; ++m )
        for ( size_t n { 0ULL }; n < result.__cols; ++n )
            for ( size_t p { 0ULL }; p < this->__cols; ++p )
                result[m][n] += (*this)[m][p] * other[p][n];

    return result;
}

// Overload for shorthand multiplication, straightforward implementation
template<typename NUM_TYPE>
void Array2d<NUM_TYPE>::operator*=( const Array2d<NUM_TYPE> other )
{
    if ( this->__cols != other.__rows )
        throw std::invalid_argument { "error: the matrices cannot be multipled "
        "since the dimensions do not match as required.\n" };

    *this = *this * other;
}

// Prints the contents of the array in its given shape
template<typename NUM_TYPE>
void Array2d<NUM_TYPE>::view() const
{
    for ( size_t row { 0ULL }; row < __rows; ++row )
    {
        for ( size_t col { 0ULL }; col < __cols; ++col )
            std::cout << *(__data + row * __cols + col) << ' ';
        std::cout << '\n';
    }
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


// Simple test function for `Array2d` demo
void testArray2d()
{
    std::srand( static_cast<unsigned int>(std::time( nullptr )) );

    const Array2d<double> A { 3, 2 };

    // for ( auto& el : A )                        // testing for-each loop implementation
    //     el = std::rand() / 1000.0;

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

    // testing rows(), cols() and view() methods
    std::cout << "Shape of A : " << A.rows() << ' ' << A.cols() << "\n\n";
    A.view();
    std::cout << '\n';

    Array2d<int> x { 2, 2 };
    Array2d<int> y { 2, 2 };

    for ( auto& el : x )
        el = std::rand() % 10 - 5;
    for ( auto& el : y )
        el = std::rand() % 10 - 5;

    x.view();
    std::cout << '\n';
    y.view();
    std::cout << '\n';

    Array2d<int> z { x * y };       // testing multiplication overload
    z *= x;                         // testing shorthand overload
    z.view();
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

// Template class instantiations for desired datatypes
template class IntRange<std::int_fast32_t>;
template class IntRange<std::int_fast64_t>;


// Simple test function for `IntRange` demo
void testIntRange()
{
    int count { 0 };
    for ( const auto num : IntRange<int> { 2'000, 5 } )
    {
        std::cout << num << '\t';
        if ( ++count % 10 == 0 ) std::cout << '\n';
    }
}
