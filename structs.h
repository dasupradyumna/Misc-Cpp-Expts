#ifndef __structs_h__
#define __structs_h__

#include <cstddef>          // std::size_t

using size_t = std::size_t;

/*///////////////////////////// Generic 2D Rectangular Array, using dynamic contiguous memory ///////////////////////////////
 *
 * Instead of using an array of dynamically allocated pointers for each row, which can result in
 * each row being physically separated in the heap, we allocate a single chunk of contiguous memory
 * to hold the entire array.
 * Subscript operator is then overloaded to index into the 1D memory chunk using the traditional 2D
 * subscripts [][]. Support is provided for the range-based for loop iteration as well.
 */
template<typename _NumericType>
class Array2d
{
  class Row
  {
    /* This class is created for the sole purpose of bounds checking while indexing columns
     * for each row. If the bounds check for columns is not necessary, this class can be removed
     * and appropriate changes to `Array2d` attributes and method return types should work fine.
     * This class cannot be accessed outside Array2d (except by friends) since it is private.
     */
    const Array2d* const __link;                          // nested class can access parent's private attributes (since C++11)

  public:

    // mutable attribute for subscripting const Array2d objects
    // public attribute for access in Array2d methods
    mutable _NumericType* __row;                          // pointer to first element of current row

    Row( const Array2d* link );                           // initializes `__row` to nullptr, links to an `Array2d` instance
    _NumericType& operator[]( const size_t col ) const;   // bounds check and returns reference to desired element
  };

  const size_t __rows;                                    // number of rows in the 2d array
  const size_t __cols;                                    // number of columns in the 2d array
  _NumericType* __data;                                   // the actual data stored in the 2d array
  Row* __ptrRow;                                             // pointer to `Row` object for indexing columns

public:

  //  TODO  add support for initializer list in constructor (for const Array2d objects)
  // constructors always zero-initialize the data array
  Array2d( const size_t rows, const size_t cols );        // initializes `rows` and `cols`, allocates memory for `data`
  Array2d( const Array2d& copy );                         // custom copy constructor to make sure pointers are not copied directly
  Array2d( Array2d&& temp );                              // custom move constructor to make sure pointers are not copied directly
  ~Array2d();                                             // deallocates memory held by `data`
  const size_t rows() const;                              // returns the number of rows in the 2d array
  const size_t cols() const;                              // returns the number of columns in the 2d array
  _NumericType* begin() const;                            // returns iterator to the start of the array
  const _NumericType* const end() const;                  // returns iterator to one position after the end of the array

  Row& operator[]( const size_t row ) const;              // bounds check and returns reference to desired row
  void operator=( const Array2d& other );                 // custom copy assignment operator
  void operator=( Array2d&& temp );                       // custom move assignment operator
  Array2d operator+( const Array2d& other ) const;        // add 2 matrices, if their dimensions are valid
  void operator+=( const Array2d& other );                // overloading shorthand operator (addition)
  Array2d operator-() const;                              // flip the signs of all elements of matrix
  Array2d operator-( const Array2d& other ) const;        // subtract 2 matrices, if their dimensions are valid
  void operator-=( const Array2d& other );                // overloading shorthand operator (subtraction)
  Array2d operator*( const Array2d& other ) const;        // multiply 2 matrices, if their dimensions are valid
  Array2d operator*( const _NumericType value ) const;    // multiply scalar to every element of the matrix
  void operator*=( const Array2d& other );                // overloading shorthand operator (multiplication)

  void view() const;                                      // prints the contents of the array
};

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