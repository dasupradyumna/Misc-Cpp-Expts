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
  for ( int mergeSize { 2 }; mergeSize / 2 < conSize; mergeSize <<= 1 )
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
        std::distance( subBuf, buffer.end() ) <= mergeSize / 2
        ? buffer.end()
        : subBuf + mergeSize / 2
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
_Iter getPivot( _Iter first, _Iter last, _Pred pred )
{
  auto median3 = [&pred] ( _Iter a, _Iter b, _Iter c ) -> _Iter
  {
    return pred( *a, *b )
      ? (pred( *b, *c )
          ? b
          : (pred( *a, *c )
              ? c
              : a))
      : (!pred( *b, *c )
          ? b
          : (!pred( *a, *c )
              ? c
              : a));
  };

  using diff_t = typename std::iterator_traits<_Iter>::difference_type;
  diff_t width { 1 + std::distance( first, last ) };
  _Iter pivot { first + width / 2 };  // pivot = midpoint

  if ( width >= 50 )  // pivot = median of 3
  {
    if ( width >= 100 ) //  pivot = median of 9
    {
      first = median3( first, first + width / 8, first + width / 4 );
      pivot = median3( pivot - width / 8, pivot, pivot + width / 8 );
      last = median3( last - width / 4, last - width / 8, last );
    }
    pivot = median3( first, pivot, last );
  }

  return pivot;
}

template<typename _Iter, typename _Pred>
void sort::__quick( _Iter begin, _Iter end, _Pred pred )
{
  vector<std::pair<_Iter, _Iter>> stack { { begin, end } };
  while ( stack.size() != 0 )
  {
    auto [left, right] { stack.pop() };
    if ( left == right || left + 1 == right ) continue;

    std::iter_swap( left, getPivot( left, right - 1, pred ) );

    _Iter partIdx = [&]
    {
      _Iter partL { left + 1 };
      _Iter partR { right - 1 };
      while ( true )
      {
        while ( pred( *partL, *left ) )
          if ( partL + 1 == right )   // pivot is maximum value
            return partR;
          else
            ++partL;

        while ( !pred( *partR, *left ) )
          if ( partR == left + 1 )        // pivot is minimum value
            return --partR;
          else
            --partR;

        if ( partL < partR )
          std::iter_swap( partL, partR );
        else
          return partR;
      }
    }();

    std::iter_swap( left, partIdx );
    stack.push_back( { left, partIdx } );
    stack.push_back( { partIdx + 1, right } );
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
    case SortType::Quick: __quick( begin, end, pred ); break;
    case SortType::STD: std::sort( begin, end, pred ); break;
  }
}

void testSort()
{
  srand( static_cast<unsigned int>(time( nullptr )) );

  constexpr size_t N { 100 };
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

  sort sorter { SortType::Quick };
  sorter( A.begin(), A.end() );
  std::cout << std::boolalpha << sort::check( A.begin(), A.end() ) << '\n';

  for ( const auto& el : A )
    std::cout << el << ' ';
  std::cout << '\n';
}
