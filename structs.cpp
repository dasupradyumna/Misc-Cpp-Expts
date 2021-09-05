// Implementations of data structures described in `structs.h`

#include "structs.h"

#include <algorithm>        // std::copy, std::swap
#include <cstdint>          // std::int_fast16_t, std::int_fast32_t, std::int_fast64_t
#include <cstdlib>          // std::abs, std::srand, std::rand
#include <ctime>            // std::time
#include <iostream>         // std::cin, std::cout, std::cerr
#include <stdexcept>        // std::invalid_argument, std::out_of_range

//////////////////////////////////// 2D Flat Rectangular Array //////////////////////////////////////

////////// Matrix::Row //////////

// Basic constructor.
template<typename _NumericType>
Matrix<_NumericType>::Row::Row( _NumericType* const row, const size_t cols ) :
  __row { row },
  __cols { cols }
{}

// Returns value by indexing the input column in current Row object.
template<typename _NumericType>
_NumericType& Matrix<_NumericType>::Row::operator[]( const size_t col ) const
{
  if ( col >= __cols )
    throw std::out_of_range { "error: column index out of bounds.\n" };

  return *(__row + col);
}

////////// Matrix //////////

// Basic constructor.
template<typename _NumericType>
Matrix<_NumericType>::Matrix( const size_t rows, const size_t cols ) :
  __rows { rows },
  __cols { cols },
  __data { (rows * cols) ? new _NumericType[rows * cols] { 0 } : nullptr }
{
  if ( !__data ) throw std::invalid_argument { "error: matrix has either 0 rows or 0 columns or both.\n" };
}

/* This constructor accepts the dimensions of the matrix and an initializer list to fill in.
 * It checks if the initializer list shape exceeds that of the matrix to throw an error.
 * In case of list being smaller than the matrix, the remaining values are left zero-initialized.
 */
template<typename _NumericType>
Matrix<_NumericType>::Matrix( const size_t rows, const size_t cols, InitializerList2D list ) :
  Matrix { rows, cols }
{
  size_t list_row { 0ULL };
  for ( auto innerlist : list )
  {
    if ( list_row >= rows )
      throw std::invalid_argument { "error: too many row values to unpack into Matrix.\n" };

    size_t list_col { 0ULL };
    for ( _NumericType value : innerlist )
    {
      if ( list_col >= cols )
        throw std::invalid_argument { "error: too many column values to unpack into Matrix.\n" };

      __data[list_row * __cols + list_col] = value;
      ++list_col;
    }
    ++list_row;
  }
}

/* Custom copy constructor, called whenever object is passed by value.
 * Default copy constructor would copy the pointers directly.
 * So when the copy goes out of scope, its destructor will (wrongly) delete the pointers allocated by original object.
 * Later, when the original goes out of scope, its destructor will try to delete the dangling pointer and crash.
 * This constructor creates a deep copy to prevent the above issue, i.e copy has distinct pointers.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
Matrix<_NumericType>::Matrix( const Matrix& copy ) :
  Matrix { copy.__rows, copy.__cols }
{
  // copying only the data pointed by original's pointer to copy's distinct pointer
  std::copy( copy.begin(), copy.end(), this->begin() );
}

/* Custom move constructor, called whenever an Matrix R-value is used to initiate an Matrix object.
 * Its purpose is the same as the copy constructor, but it is faster than a copy constructor initialized by an
 * Matrix R-value because it directly takes ownership of all assets of Matrix parameter, leaving it in an
 * effectively "useless state". Whereas, copy constructor does not affect data owned by the Matrix parameter and
 * copies all the data to the current object.
 * Time complexity ~ O(1).
 */
template<typename _NumericType>
Matrix<_NumericType>::Matrix( Matrix&& temp ) noexcept :
  Matrix { temp.__rows, temp.__cols }
{
  std::swap( __data, temp.__data );       // swapping pointers since temp is a temporary object, destroyed at function end
}

// Basic destructor.
template<typename _NumericType>
Matrix<_NumericType>::~Matrix()
{
  delete[] __data;
}

// Returns the value of rows attribute.
template<typename _NumericType>
inline
const size_t Matrix<_NumericType>::rows() const { return __rows; }

// Returns the value of columns attribute.
template<typename _NumericType>
inline
const size_t Matrix<_NumericType>::cols() const { return __cols; }

/* Returns a non-const iterator to the start of the array so that the loop variable inside a range-based `for` loop
 * has the option to be modifiable or read-only, depending on the type of the iterator.
 * for (auto x : Arr) - elements of Arr are read-only.
 * for (auto& x : Arr) - elements of Arr are referenced and modified directly.
 */
template<typename _NumericType>
inline
_NumericType* Matrix<_NumericType>::begin() const { return __data; }

// Returns a const iterator to the end of the array, since it is used only for bound-checking.
template<typename _NumericType>
inline
_NumericType* const Matrix<_NumericType>::end() const { return __data + __rows * __cols; }

/* This function is called for the first index (rows) of the array.
 * It returns a reference to a `Row` object, which calls the subscript method for the
 * second index (columns). Finally, it returns the desired value from the array.
 */
template<typename _NumericType>
typename Matrix<_NumericType>::Row Matrix<_NumericType>::operator[]( const size_t row ) const
{
  if ( row >= __rows )
    throw std::out_of_range { "error: row index out of bounds.\n" };

  return Row { __data + row * __cols, __cols };
}

/* Copy assignment operator for Matrix.
 * Rows and columns of LHS and RHS are expected to be equal before assignment.
 * All the data pointed by RHS pointer is copied to LHS pointer.
 * Pointers themselves are not copied to avoid multiple references to same data.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
 //template<typename _NumericType>
 //void Matrix<_NumericType>::operator=( const Matrix& copy )
 //{
 //  if ( this->__rows != copy.__rows || this->__cols != copy.__cols )
 //    throw std::invalid_argument { "error: source and target matrix dimensions do not match.\n" };
 //
 //  std::copy( copy.begin(), copy.end(), this->begin() );
 //}

 /* Move assignment operator for Matrix.
  * Rows and columns of LHS and RHS are expected to be equal before assignment.
  * Pointers are swapped with the temporary object, since it is destroyed at end of this function.
  * Time complexity ~ O(1).
  */
  //template<typename _NumericType>
  //void Matrix<_NumericType>::operator=( Matrix&& temp ) noexcept
  //{
  //  if ( this->__rows != temp.__rows || this->__cols != temp.__cols )
  //    throw std::invalid_argument { "error: source and target matrix dimensions do not match.\n" };
  //
  //  std::swap( __data, temp.__data );
  //}

  /* Copy-and-Swap idiom (both Move and Copy assignment)
  /// If this operator is uncommented, comment both copy and move assignment operator overloads above; they will be unnecessary. ///
  /// This implementation is better in the general case of assigning (replacing) Matrix objects even if they have different sizes. ///
  /// Utilizes the respective constructor to make the necessary argument object, removing any possibility of bugs in this operator. ///
   * If an r-value Matrix is passed as an argument, then argument `mat` will be constructed using the move constructor from an r-value.
   * If an l-value Matrix is passed as an argument, then argument `mat` will be constructed using the copy constructor from an l-value.
   * Time complexity depends on the type of constructor used for argument `mat`.
   * Time complexity ~ O(1) [move constructed] OR O(n^2) [copy constructed]
   */
template<typename _NumericType>
void Matrix<_NumericType>::operator=( Matrix mat )
{
  std::swap( __data, mat.__data );
}

/* Overload for adding 2 matrices of same dimensions.
 * Rows and columns of LHS and RHS are expected to be equal before addition.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
Matrix<_NumericType> Matrix<_NumericType>::operator+( const Matrix& other ) const
{
  if ( this->__rows != other.__rows || this->__cols != other.__cols )
    throw std::invalid_argument { "error: dimensions of addend matrices do not match.\n" };

  Matrix result { this->__rows, other.__cols };
  for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
    result.__data[i] = this->__data[i] + other.__data[i];

  return result;
}

/* Overload for shorthand addition, straightforward implementation using addition overload.
 * Time complexity is same as addition ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
inline
void Matrix<_NumericType>::operator+=( const Matrix& other ) { *this = *this + other; }

/* Overload for inverting the sign of all elements of a matrix.
 * Time complexity is same as multiplication with a scalar ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
inline
Matrix<_NumericType> Matrix<_NumericType>::operator-() const { return *this * -1; }

/* Overload for subtraction between 2 matrices of same dimensions.
 * Rows and columns of LHS and RHS are expected to be equal before subtraction.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
Matrix<_NumericType> Matrix<_NumericType>::operator-( const Matrix& other ) const
{
  if ( this->__rows != other.__rows || this->__cols != other.__cols )
    throw std::invalid_argument { "error: minuend and subtrahend matrix dimensions do not match.\n" };

  Matrix result { this->__rows, other.__cols };
  for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
    result.__data[i] = this->__data[i] - other.__data[i];

  return result;
}

/* Overload for shorthand subtraction, straightforward implementation using subtraction overload.
 * Time complexity is same as subtraction ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
inline
void Matrix<_NumericType>::operator-=( const Matrix& other ) { *this = *this - other; }

/* Overload for multiplication of two Matrix objects.
 * Checks the matrix multiplication dimensions prerequisite.
 * Returns the resultant Matrix object.
 * Naive algorithm, time complexity ~ O(n^3) ~ O(rows1*cols1*cols2).
 */
template<typename _NumericType>
Matrix<_NumericType> Matrix<_NumericType>::operator*( const Matrix& other ) const
{
  if ( this->__cols != other.__rows )
    throw std::invalid_argument { "error: dimensions of multiplicand matrices are incompatible for multiplication.\n" };

  Matrix result { this->__rows, other.__cols };
  for ( size_t m { 0ULL }; m < result.__rows; ++m )
    for ( size_t n { 0ULL }; n < result.__cols; ++n )
      for ( size_t p { 0ULL }; p < this->__cols; ++p )
        *(result.__data + m * __cols + n) += *(__data + m * __cols + p) * *(other.__data + p * __cols + n);

  return result;
}

/* Overload for multiplying every element of matrix with a scalar value.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
Matrix<_NumericType> Matrix<_NumericType>::operator*( const _NumericType value ) const
{
  Matrix result { __rows, __cols };
  for ( size_t i { 0ULL }; i < __rows * __cols; ++i )
    result.__data[i] = this->__data[i] * value;

  return result;
}

/* Overload for shorthand multiplication, straightforward implementation using multiplication overload.
 * Time complexity is same as multiplication ~ O(n^3) ~ O(rows1*cols1*cols2).
 */
template<typename _NumericType>
inline
void Matrix<_NumericType>::operator*=( const Matrix& other ) { *this = *this * other; }

/* Prints the contents of the array in its given shape.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
void Matrix<_NumericType>::view() const
{
  for ( size_t row { 0ULL }; row < __rows; ++row )
  {
    for ( size_t col { 0ULL }; col < __cols; ++col )
      std::cout << *(__data + row * __cols + col) << ' ';
    std::cout << '\n';
  }
  std::cout << '\n';
}

/* Overload to make scalar matrix multiplication commutative.
 * Implemented using the Matrix class overload.
 * Time complexity ~ O(n^2) or O(rows*cols).
 */
template<typename _NumericType>
inline
Matrix<_NumericType> operator*( const _NumericType value, const Matrix<_NumericType>& mat ) { return mat * value; }


/* << IMPORTANT >>
 * "Explicit instantiation" : The template class implementation is stored in a source file, seperately from
 * definitions in the header. This means that only the pre-specified explicit instances of the template can be used, and
 * for every new (user-defined) datatype, we have to manually add an instance before using it.
 * Not doing this will throw a linker error since it can not find the type-specific implementation of the methods it sees
 * in template class. By compiling the explicit instances, the linker will be able to find the desired variation of template.
 *
 * "Header inclusion" : Both the definition and implementation of the class template is stored in the header file.
 * This means that every file that includes the header has full knowledge of how the class functions and therefore knows
 * how to create a template instance for a generic type (including user-defined types).
 * But this also slows down compilation process for significantly large projects because every file including the class
 * template header creates its own required template instances which the linker has to sort through to prevent ODR violation,
 * unlike explicit instantiation which is much faster with pre-defined instances.
 *
 * Below is explicit instantiation method.
 */
template class Matrix<std::int_fast32_t>;
template class Matrix<std::int_fast64_t>;
template class Matrix<float>;
template class Matrix<double>;
template class Matrix<long double>;


// Simple test function for `Matrix` demo.
void testArray2d()
{
  std::srand( static_cast<unsigned int>(std::time( nullptr )) );

  const Matrix<double> A { 3, 3, {
    { 1, 0, 1 },
    { 0, 1, 0 },
    { 1, 0, 1 }
  } };

  A.view();                               // testing view() method

  // while ( true )                          // testing traditional [][] indexing with bounds check
  // {
  //   int r { };
  //   size_t c { };
  //   std::cout << "Enter row and column indices (negative input to quit) : ";
  //   std::cin >> r;
  //   if ( r < 0 ) break;
  //   std::cin >> c;
  //   try
  //   {
  //     std::cout << A[static_cast<size_t>(r)][c] << '\n';
  //   }
  //   catch ( std::out_of_range& exception )
  //   {
  //     std::cerr << exception.what();
  //   }
  // }

  Matrix<double> B { 3, 3 };
  for ( auto& el : B )
    el = (std::rand() % 101) / 100.0;
  B.view();

  Matrix<double> C { A + B };             // testing addition overload
  C.view();
  C = A - B;                              // testing subtraction overload
  C.view();
  C = A * B;                              // testing multiplication overload
  C.view();
  C = Matrix<double> { 3, 3 };            // move constructor
  C.view();
  C += A;                                 // testing shorthand addition
  C.view();
  C -= B;                                 // testing shorthand subtraction
  C.view();
  C *= Matrix<double> { 3, 3, {           // testing shorthand multiplication
    { 0.5, 0, 0 },
    { 0, 0.5, 0 },
    { 0, 0, 0.5 }
  } };
  C.view();
  C = 2.0 * C;                            // testing scalar multiplication (commutative)
  C.view();
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
