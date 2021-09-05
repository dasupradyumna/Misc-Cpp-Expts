#ifndef __structs_h__
#define __structs_h__

#include <cstddef>          // std::size_t
#include <initializer_list> // std::initializer_list

using size_t = std::size_t;

/*///////////////////////////// Generic 2D Rectangular Array (Matrix), using dynamic contiguous memory ///////////////////////////////
 *
 * Instead of using an array of dynamically allocated pointers for each row, which can result in
 * each row being physically separated in the heap, we allocate a single chunk of contiguous memory
 * to hold the entire array.
 * Subscript operator is then overloaded to index into the 1D memory chunk using the traditional 2D
 * subscripts [][]. Support is provided for the range-based for loop iteration as well.
 */
template<typename _NumericType>
class Matrix
{
  using InitializerList2D = std::initializer_list<std::initializer_list<_NumericType >>;
  class Row
  {
    /* This class is created for the sole purpose of bounds checking while indexing columns
     * for each row. If the bounds check for columns is not necessary, this class can be removed
     * and appropriate changes to `Matrix` attributes and method return types should work fine.
     * This class cannot be accessed outside Matrix (except by friends) since it is private.
     */
    const size_t __cols;
    _NumericType* const __row;                            // pointer to first element of current row

  public:

    Row( _NumericType* const row, const size_t cols );    // initializes `__row` to nullptr, links to an `Matrix` instance
    Row( const Row& ) = delete;                           // disabling copy semantics
    Row( Row&& ) noexcept = default;                      // enabling move semantics
    _NumericType& operator[]( const size_t col ) const;   // bounds check and returns reference to desired element
  };

  const size_t __rows;                                    // number of rows in the 2d array
  const size_t __cols;                                    // number of columns in the 2d array
  _NumericType* __data;                                   // the actual data stored in the 2d array

public:

  // constructors always zero-initialize the data array, unless otherwise specified
  Matrix( const size_t rows, const size_t cols );         // initializes `rows` and `cols`, allocates memory for `data`
  Matrix( const size_t rows, const size_t cols,
          InitializerList2D list );                       // constructs an empty `Matrix` and fills it using initializer list
  Matrix( const Matrix& copy );                           // custom copy constructor to prevent shallow copy of pointers
  Matrix( Matrix&& temp ) noexcept;                       // custom move constructor to prevent shallow copy of pointers
  ~Matrix();                                              // deallocates memory held by `data`
  const size_t rows() const;                              // returns the number of rows in the 2d array
  const size_t cols() const;                              // returns the number of columns in the 2d array
  _NumericType* begin() const;                            // returns iterator to the start of the array
  _NumericType* const end() const;                        // returns iterator to one position after the end of the array

  Row operator[]( const size_t row ) const;               // bounds check and returns reference to desired row
  //void operator=( const Matrix& copy );                   // custom copy assignment operator
  //void operator=( Matrix&& temp ) noexcept;               // custom move assignment operator
  void operator=( Matrix mat );                           // handles both move and copy assignment (copy-and-swap idiom)
  Matrix operator+( const Matrix& other ) const;          // add 2 matrices, if their dimensions are valid
  void operator+=( const Matrix& other );                 // overloading shorthand operator (addition)
  Matrix operator-() const;                               // flip the signs of all elements of matrix
  Matrix operator-( const Matrix& other ) const;          // subtract 2 matrices, if their dimensions are valid
  void operator-=( const Matrix& other );                 // overloading shorthand operator (subtraction)
  Matrix operator*( const Matrix& other ) const;          // multiply 2 matrices, if their dimensions are valid
  Matrix operator*( const _NumericType value ) const;     // multiply scalar to every element of the matrix
  void operator*=( const Matrix& other );                 // overloading shorthand operator (multiplication)

  void view() const;                                      // prints the contents of the array
};

// to make scalar matrix multiplication operation commutative
template<typename _NumericType>
Matrix<_NumericType> operator*( const _NumericType value, const Matrix<_NumericType>& mat );

void testArray2d();                                       // demo function

/*/////////////////////////////////////// Python-like Range iterator in for-each loop /////////////////////////////////////////
 *
 * We store the ending and starting values, where the starting value gets wrapped by an iterator and
 * compared to the ending value by the for-each loop internally. We create an iterator class because
 * the internal implementation uses indirection operator on the iterator. Except for this one operation,
 * all other operations are compatible with integral types, i.e this iterator "wrapper" is created only
 * for the sake of indirection operation.
 * And we also overload all the necessary operators that are internally used by the for-each
 * implementation. ( Under-the-hood working : https://en.cppreference.com/w/cpp/language/range-for )
 * A `const IntRange` object makes no sense.
 */
template<typename _Integer>
class IntRange
{
  class Iterator                                          // wrapper class for the iterating variable
  {
    _Integer __value;
    const _Integer __step;

  public:

    Iterator( const _Integer value, const _Integer step );
    bool operator!=( const _Integer other );
    _Integer operator*();
    Iterator& operator++();
  };

  const _Integer __begin;                                 // holds the starting value
  const _Integer __end;                                   // holds the halting value
  const _Integer __step;                                  // holds the step value to move the iterator

public:

  // specify default argument either in declaration or in definition, but not both 
  // (for forward-declared functions) - counts as a redeclaration of default argument
  IntRange( const _Integer begin, const _Integer end, const _Integer step = 1 );    // initializes the 2 attributes
  IntRange( const _Integer end );                         // overload for missing `begin`, no step parameter to avoid ambiguous call
  Iterator begin();                                       // returns an iterator to the start point
  const _Integer end();                                   // returns the ending value
};

void testIntRange();                                      // demo function

#endif