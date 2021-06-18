#ifndef __structs_h__
#define __structs_h__

#include <cstddef>          // for `std::size_t`

using size_t = std::size_t;

/*///////////////////////////// Generic 2D Rectangular Array, using dynamic contiguous memory ///////////////////////////////
 *
 * Instead of using an array of dynamically allocated pointers for each row, which can result in
 * each row being physically separated in the heap, we allocate a single chunk of contiguous memory
 * to hold the entire array.
 * Subscript operator is then overloaded to index into the 1D memory chunk using the traditional 2D
 * subscripts [][]. Support is provided for the range-based for loop iteration as well.
 */
template<typename NUM_TYPE>
class Array2d
{
    class Row
    {
        /* This class is created for the sole purpose of bounds checking while indexing columns
         * for each row. If the bounds check for columns is not necessary, this class can be removed
         * and appropriate changes to `Array2d` attributes and method return types should work fine.
         */
        const Array2d* const __link;                        // nested class can access `Array2d` private attributes (since C++11)

    public:

        // mutable attribute for subscripting const Array2d objects
        mutable NUM_TYPE* __row;                            // pointer to first element of current row

        Row( const Array2d* link );                         // initializes `__row` to nullptr, links to an `Array2d` instance
        NUM_TYPE& operator[]( const size_t col ) const;     // bounds check and returns reference to desired element
    };

    const size_t __rows;                                    // number of rows in the 2d array
    const size_t __cols;                                    // number of columns in the 2d array
    NUM_TYPE* const __data;                                 // the actual data stored in the 2d array
    Row* const __ref;                                       // pointer to `Row` object for indexing columns

public:

    //  TODO  add support for initializer list in constructor (for const Array2d objects)
    Array2d( const size_t rows, const size_t cols );        // initializes `rows` and `cols`, allocates memory for `data`
    ~Array2d();                                             // deallocates memory held by `data`
    const size_t rows() const;                              // returns the number of rows in the 2d array
    const size_t cols() const;                              // returns the number of columns in the 2d array
    NUM_TYPE* begin() const;                                // returns iterator to the start of the array
    const NUM_TYPE* const end() const;                      // returns iterator to one position after the end of the array
    Row& operator[]( const size_t row ) const;              // bounds check and returns reference to desired row
};

void testArray2d();                                         // demo function

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
template<typename INT_TYPE>
class IntRange
{
    class Iterator                                          // wrapper class for the iterating variable
    {
        INT_TYPE __value;
        const INT_TYPE __step;

    public:

        Iterator( const INT_TYPE value, const INT_TYPE step );
        bool operator!=( const INT_TYPE other );
        INT_TYPE operator*();
        Iterator& operator++();
    };

    const INT_TYPE __begin;                                 // holds the starting value
    const INT_TYPE __end;                                   // holds the halting value
    const INT_TYPE __step;                                  // holds the step value to move the iterator

public:

    // specify default argument either in declaration or in definition, but not both 
    // (for forward-declared functions) - counts as a redeclaration of default argument
    IntRange( const INT_TYPE begin, const INT_TYPE end, const INT_TYPE step = 1 );    // initializes the 2 attributes
    IntRange( const INT_TYPE end );                         // overload for missing `begin`, no step parameter to avoid ambiguous call
    Iterator begin();                                       // returns an iterator to the start point
    const INT_TYPE end();                                   // returns the ending value
};

void testIntRange();                                        // demo function

#endif