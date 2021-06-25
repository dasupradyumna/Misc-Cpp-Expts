// Implementations of data structures described in `structs.h`

#include "structs.h"

#include <algorithm>        // std::swap
#include <cstdint>          // std::int_fast16_t, std::int_fast32_t, std::int_fast64_t
#include <cstdlib>          // std::abs, std::srand, std::rand
#include <ctime>            // std::time
#include <iostream>         // std::cin, std::cout, std::cerr
#include <stdexcept>        // std::invalid_argument, std::out_of_range

//////////////////////////////////// 2D Flat Rectangular Array //////////////////////////////////////

////////// Array2d::Row //////////

// Basic constructor.
template<typename _NumericType>
Array2d<_NumericType>::Row::Row( const Array2d* link ) :
  __row { nullptr },
  __link { link }
{}

// Returns value by indexing the input column in current Row object.
template<typename _NumericType>
_NumericType& Array2d<_NumericType>::Row::operator[]( const size_t col ) const
{
  if ( col >= __link->__cols )
    throw std::out_of_range { "error: column index out of bounds.\n" };

  return __row[col];
}

////////// Array2d //////////

// Basic constructor.
template<typename _NumericType>
Array2d<_NumericType>::Array2d( const size_t rows, const size_t cols ) :
  __rows { rows },
  __cols { cols },
  __data { new _NumericType[rows * cols] { 0 } },
  __ptrRow { new Row { this } }
{}

/* Custom copy constructor, called whenever object is passed by value.
 * Default copy constructor would copy the pointers directly.
 * So when the copy goes out of scope, its destructor will (wrongly) delete the pointers allocated by original object.
 * Later, when the original goes out of scope, its destructor will try to delete the dangling pointer and crash.
 * This constructor creates a deep copy to prevent the above issue, i.e copy has distinct pointers.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
Array2d<_NumericType>::Array2d( const Array2d& copy ) :
  Array2d { copy.__rows, copy.__cols }
{
  for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
    __data[i] = copy.__data[i];           // copying only the data pointed by original's pointer to copy's distinct pointer
}

/* Custom move constructor, called whenever an Array2d R-value is used to initiate an Array2d object.
 * Its purpose is the same as the copy constructor, but it is faster than a copy constructor initialized by an
 * Array2d R-value because it directly takes ownership of all assets of Array2d parameter, leaving it in an
 * effectively "useless state". Whereas, copy constructor does not affect data owned by the Array2d parameter and
 * copies all the data to the current object.
 * Time complexity ~ O(1).
 */
template<typename _NumericType>
Array2d<_NumericType>::Array2d( Array2d&& temp ) :
  Array2d { temp.__rows, temp.__cols }
{
  std::swap( __data, temp.__data );       // swapping pointers since temp is a temporary object, destroyed at function end
}

// Basic destructor.
template<typename _NumericType>
Array2d<_NumericType>::~Array2d()
{
  delete[] __data;
  delete __ptrRow;
}

// Returns the value of rows attribute.
template<typename _NumericType>
const size_t Array2d<_NumericType>::rows() const { return __rows; }

// Returns the value of columns attribute.
template<typename _NumericType>
const size_t Array2d<_NumericType>::cols() const { return __cols; }

/* Returns a non-const iterator so that the loop variable inside a range-based `for` loop
 * has the option to be modifiable or read-only, depending on the type of the iterator.
 * for (auto x : Arr) - elements of Arr are read-only.
 * for (auto& x : Arr) - elements of Arr are referenced and modified directly.
 */
template<typename _NumericType>
_NumericType* Array2d<_NumericType>::begin() const { return __data; }

// Returns a const iterator to the end of the array, since it is used only for bound-checking.
template<typename _NumericType>
const _NumericType* const Array2d<_NumericType>::end() const { return __data + __rows * __cols; }

/* This function is called for the first index (rows) of the array.
 * It returns a reference to a `Row` object, which calls the subscript method for the
 * second index (columns). Finally, it returns the desired value from the array.
 */
template<typename _NumericType>
typename Array2d<_NumericType>::Row& Array2d<_NumericType>::operator[]( const size_t row ) const
{
  if ( row < __rows )
    __ptrRow->__row = &__data[row * __cols];
  else
    throw std::out_of_range { "error: row index out of bounds.\n" };

  return *__ptrRow;
}

/* Copy assignment operator for Array2d.
 * Rows and columns of LHS and RHS are expected to be equal before assignment.
 * All the data pointed by RHS pointer is copied to LHS pointer.
 * Pointers themselves are not copied to avoid multiple references to same data.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
void Array2d<_NumericType>::operator=( const Array2d& other )
{
  if ( this->__rows != other.__rows || this->__cols != other.__cols )
    throw std::invalid_argument { "error: source and target matrix dimensions do not match.\n" };

  for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
    __data[i] = other.__data[i];
}

/* Move assignment operator for Array2d.
 * Rows and columns of LHS and RHS are expected to be equal before assignment.
 * Pointers are swapped with the temporary object, since it is destroyed at end of this function.
 * Time complexity ~ O(1).
 */
template<typename _NumericType>
void Array2d<_NumericType>::operator=( Array2d&& temp )
{
  if ( this->__rows != temp.__rows || this->__cols != temp.__cols )
    throw std::invalid_argument { "error: source and target matrix dimensions do not match.\n" };

  std::swap( __data, temp.__data );
}

/* Overload for adding 2 matrices of same dimensions.
 * Rows and columns of LHS and RHS are expected to be equal before addition.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
Array2d<_NumericType> Array2d<_NumericType>::operator+( const Array2d& other ) const
{
  if ( this->__rows != other.__rows || this->__cols != other.__cols )
    throw std::invalid_argument { "error: dimensions of addend matrices do not match.\n" };

  Array2d<_NumericType> result { this->__rows, other.__cols };
  for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
    result.__data[i] = this->__data[i] + other.__data[i];

  return result;
}

/* Overload for shorthand addition, straightforward implementation using addition overload.
 * Time complexity is same as addition ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
void Array2d<_NumericType>::operator+=( const Array2d& other ) { *this = *this + other; }

/* Overload for inverting the sign of all elements of a matrix.
 * Time complexity is same as multiplication with a scalar ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
Array2d<_NumericType> Array2d<_NumericType>::operator-() const { return *this * -1; }

/* Overload for subtraction between 2 matrices of same dimensions.
 * Rows and columns of LHS and RHS are expected to be equal before subtraction.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
Array2d<_NumericType> Array2d<_NumericType>::operator-( const Array2d& other ) const
{
  if ( this->__rows != other.__rows || this->__cols != other.__cols )
    throw std::invalid_argument { "error: minuend and subtrahend matrix dimensions do not match.\n" };

  Array2d<_NumericType> result { this->__rows, other.__cols };
  for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
    result.__data[i] = this->__data[i] - other.__data[i];

  return result;
}

/* Overload for shorthand subtraction, straightforward implementation using subtraction overload.
 * Time complexity is same as subtraction ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
void Array2d<_NumericType>::operator-=( const Array2d& other ) { *this = *this - other; }

/* Overload for multiplication of two Array2d objects.
 * Checks the matrix multiplication dimensions prerequisite.
 * Returns the resultant Array2d object.
 * Naive algorithm, time complexity ~ O(n^3) ~ O(rows1*cols1*cols2).
 */
template<typename _NumericType>
Array2d<_NumericType> Array2d<_NumericType>::operator*( const Array2d& other ) const
{
  if ( this->__cols != other.__rows )
    throw std::invalid_argument { "error: the matrices cannot be multipled "
    "since no. of columns of first matrix and no. of rows of second matrix are not equal.\n" };

  Array2d<_NumericType> result { this->__rows, other.__cols };
  for ( size_t m { 0ULL }; m < result.__rows; ++m )
    for ( size_t n { 0ULL }; n < result.__cols; ++n )
      for ( size_t p { 0ULL }; p < this->__cols; ++p )
        result[m][n] += (*this)[m][p] * other[p][n];

  return result;
}

/* Overload for multiplying every element of matrix with a scalar value.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
Array2d<_NumericType> Array2d<_NumericType>::operator*( const _NumericType value ) const
{
  Array2d<_NumericType> result { __rows, __cols };
  for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
    result.__data[i] = this->__data[i] * value;

  return result;
}

/* Overload for shorthand multiplication, straightforward implementation using multiplication overload.
 * Time complexity is same as multiplication ~ O(n^3) ~ O(rows1*cols1*cols2).
 */
template<typename _NumericType>
void Array2d<_NumericType>::operator*=( const Array2d& other ) { *this = *this * other; }

/* Prints the contents of the array in its given shape.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
void Array2d<_NumericType>::view() const
{
  for ( size_t row { 0ULL }; row < __rows; ++row )
  {
    for ( size_t col { 0ULL }; col < __cols; ++col )
      std::cout << *(__data + row * __cols + col) << ' ';
    std::cout << '\n';
  }
  std::cout << '\n';
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
template class Array2d<long double>;


// Simple test function for `Array2d` demo.
void testArray2d()
{
  std::srand( static_cast<unsigned int>(std::time( nullptr )) );

  // const Array2d<double> A { 3, 2 };

  // while ( true )                              // testing traditional [][] indexing with bounds check
  // {
  //     int r { };
  //     size_t c { };
  //     std::cout << "Enter row and column indices (negative input to quit) : ";
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

  Array2d<int> x { 3, 3 };
  Array2d<int> y { 3, 3 };

  for ( auto& el : x )
    el = std::rand() % 15 - 7;
  for ( auto& el : y )
    el = std::rand() % 15 - 7;

  // testing view() method
  x.view();
  y.view();

  Array2d<int> z { x + y };           // testing addition overload
  z.view();
  z = x - y;                          // testing subtraction overload
  z.view();
  z = x * y;                          // testing multiplication overload
  z.view();
  z = Array2d<int> { 3, 3 };          // move constructor
  z.view();
  z += x;                             // testing shorthand addition
  z.view();
  z -= y;                             // testing shorthand subtraction
  z.view();
  z *= z;                             // testing shorthand multiplication
  z.view();
}


///////////////////////////////// Python-like Range-based Iterator ///////////////////////////////////

////////// IntRange::Iterator //////////

template<typename _Integer>
IntRange<_Integer>::Iterator::Iterator( const _Integer value, const _Integer step ) :
  __value { value },
  __step { step }
{}

/* A hack taking advantage of the internal mechanism of for-each loops.
 * Checking equality `__value` and `other` to terminate the iteration will break down in case of "end" value
 * being a distance of non-exact multiple of step away from "begin" value, for instance `IntRange{10, 200, 30}`.
 * Hence, to account for this, we check if we have already crossed the "end" value, and terminate the iteration.
 * This is done using the direction of `__step` and current `__value` relative to "end" value.
 */
template<typename _Integer>
bool IntRange<_Integer>::Iterator::operator!=( const _Integer other )
{
  return (other - __value) * __step > 0;
}

// Indirection should yield the value contained, not the address of wrapper
template<typename _Integer>
_Integer IntRange<_Integer>::Iterator::operator*() { return __value; }

// `typename` keyword is necessary for compiler because the name of the return type is dependent on the template
template<typename _Integer>
typename IntRange<_Integer>::Iterator& IntRange<_Integer>::Iterator::operator++()
{
  __value += __step;
  return *this;
}

////////// IntRange //////////

template<typename _Integer>
IntRange<_Integer>::IntRange( const _Integer begin, const _Integer end, const _Integer step ) :
  __begin { begin },
  __end { end },
  // correctly initializes step value irrespective of sign mismatch by the user
  __step { (__begin < __end) ? std::abs( step ) : -std::abs( step ) }
{}

template<typename _Integer>
IntRange<_Integer>::IntRange( const _Integer end ) :
  IntRange { 0, end, 1 }    // calling the overloaded constructor with all parameters
{ }

// `typename` keyword is necessary for compiler because the name of the return type is dependent on the template
template<typename _Integer>
typename IntRange<_Integer>::Iterator IntRange<_Integer>::begin() { return Iterator { __begin, __step }; }

template<typename _Integer>
const _Integer IntRange<_Integer>::end() { return __end; }

// Template class instantiations for desired datatypes
template class IntRange<std::int_fast32_t>;
template class IntRange<std::int_fast64_t>;


// Simple test function for `IntRange` demo
void testIntRange()
{
  int count { 0 };
  for ( const auto num : IntRange<int> { 2'000, 30, -100 } )
  {
    std::cout << num << '\t';
    if ( ++count % 10 == 0 ) std::cout << '\n';
  }
}
