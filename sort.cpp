#include "sort.h"

#include <algorithm>      // sort
#include <array>
#include <ctime>          // time
#include <iostream>       // cin, cout
#include <iterator>       // iterator_traits, distance
#include <utility>        // pair

sort::sort( SortType type ) :
  __type { type }
{ }

template<typename _Iter, typename _Pred>
static bool sort::check( _Iter begin, _Iter end, _Pred pred )
{
  for ( _Iter i { begin }; ++i != end; )
    if ( pred( *i, *(i - 1) ) )
      return false;

  return true;
}

template<typename _Iter, typename _Pred>
void sort::__bubble( _Iter begin, _Iter end, _Pred pred )
{
  using diff_t = typename std::iterator_traits<_Iter>::difference_type;
  diff_t nIters { std::distance( begin, end ) - 1 };
  for ( diff_t i { -1 }; ++i < nIters; )
  {
    bool noSwaps { true };
    _Iter _end { end - i };
    for ( _Iter j { begin }; ++j != _end; )
      if ( pred( *j, *(j - 1) ) )
      {
        std::iter_swap( j - 1, j );
        noSwaps = false;
      }

    if ( noSwaps ) break;
  }
}

template<typename _Iter, typename _Pred>
void sort::__selection( _Iter begin, _Iter end, _Pred pred )
{
  for ( _Iter i { begin }; i != end; ++i )
  {
    _Iter key { i };
    for ( _Iter j { i }; ++j != end; )
      if ( pred( *j, *key ) )
        key = j;

    std::iter_swap( i, key );
  }
}

template<typename _Iter, typename _Pred>
void sort::__insertion( _Iter begin, _Iter end, _Pred pred )
{
  for ( _Iter current { begin }; ++current != end; )
  {
    _Iter j { current };
    while ( j != begin && pred( *j, *(j - 1) ) )
    {
      std::iter_swap( j, j - 1 );
      --j;
    }
  }
}

template<typename _Iter, typename _Pred>
void sort::__merge( _Iter begin, _Iter end, _Pred pred )
{
  using diff_t = typename std::iterator_traits<_Iter>::difference_type;
  using value_t = typename std::iterator_traits<_Iter>::value_type;
  using buf_iter = typename std::vector<value_t>::iterator;

  diff_t conSize { std::distance( begin, end ) };
  std::vector<value_t> buffer( conSize );
  for ( int mergeSize { 2 }; mergeSize >> 1 < conSize; mergeSize <<= 1 )
  {
    _Iter subCon { begin };
    buf_iter subBuf { buffer.begin() };
    for ( ; ; subCon += mergeSize, subBuf += mergeSize )
    {
      bool isLastMerge { std::distance( subCon, end ) <= mergeSize };

      _Iter merger { subCon };
      const _Iter mergeEnd { isLastMerge ? end : subCon + mergeSize };
      std::copy( subCon, mergeEnd, subBuf );

      buf_iter first { subBuf };
      const buf_iter bufMid {
        (std::distance( subBuf, buffer.end() ) <= mergeSize >> 1)
        ? buffer.end()
        : subBuf + mergeSize >> 1
      };
      buf_iter second { bufMid };
      const buf_iter bufEnd { isLastMerge ? buffer.end() : subBuf + mergeSize };

      for ( ; merger != mergeEnd; ++merger )
        if ( first != bufMid && second != bufEnd )
          std::iter_swap( merger, pred( *first, *second ) ? first++ : second++ );
        else if ( first != bufMid )
          std::iter_swap( merger, first++ );
        else if ( second != bufEnd )
          std::iter_swap( merger, second++ );

      if ( isLastMerge ) break;
    }
  }
}

template<typename _Iter, typename _Pred>
void sort::operator()( _Iter begin, _Iter end, _Pred pred )
{
  if ( begin == end || begin + 1 == end )
    return;

  switch ( __type )
  {
    case SortType::Bubble: __bubble( begin, end, pred ); break;
    case SortType::Selection: __selection( begin, end, pred ); break;
    case SortType::Insertion: __insertion( begin, end, pred ); break;
    case SortType::Merge: __merge( begin, end, pred ); break;
    case SortType::STD: std::sort( begin, end, pred ); break;
  }
}

void testSort()
{
  srand( static_cast<unsigned int>(time( nullptr )) );

  constexpr size_t N { 37 };
  //int A[N] { };
  //for ( int i { -1 }; ++i < N; )
  //  A[i] = rand() % N;
  //std::array<int, N> A { };
  std::vector<int> A( N );
  for ( auto& el : A )
    el = rand() % N;
  std::cout << std::boolalpha << sort::check( A.begin(), A.end() ) << '\n';
  for ( const auto& el : A )
    std::cout << el << ' ';
  std::cout << "\n\n";

  sort sorter { SortType::Merge };
  sorter( A.begin(), A.end() );
  std::cout << std::boolalpha << sort::check( A.begin(), A.end() ) << '\n';

  for ( const auto& el : A )
    std::cout << el << ' ';
  std::cout << '\n';
}
