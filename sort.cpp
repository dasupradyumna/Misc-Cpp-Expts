#include "sort.h"

#include <algorithm>      // sort
#include <array>
#include <iostream>       // cin, cout
#include <iterator>       // iterator_traits
#include <ctime>          // time
#include <vector>

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
  diff_t n_iters { end - begin - 1 };
  for ( diff_t i { -1 }; ++i < n_iters; )
  {
    bool noswap { true };
    _Iter _end { end - i };
    for ( _Iter j { begin }; ++j != _end; )
      if ( pred( *j, *(j - 1) ) )
      {
        std::iter_swap( j - 1, j );
        noswap = false;
      }

    if ( noswap ) break;
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
  for ( _Iter i { begin }; ++i != end; )
  {
    _Iter key { i };
    _Iter j { i };
    while ( j != begin && pred( *j, *(j - 1) ) )
    {
      std::iter_swap( j, j - 1 );
      --j;
    }
  }
}

template<typename _Iter, typename _Pred>
void sort::operator()( _Iter begin, _Iter end, _Pred pred )
{
  if ( begin != end )
    switch ( __type )
    {
      case SortType::Bubble: __bubble( begin, end, pred ); break;
      case SortType::Selection: __selection( begin, end, pred ); break;
      case SortType::Insertion: __insertion( begin, end, pred ); break;
      case SortType::STD: std::sort( begin, end, pred ); break;
    }
}

void testSort()
{
  srand( static_cast<unsigned int>(time( nullptr )) );

  constexpr size_t N { 50 };
  //int A[N] { };
  //for ( int i { -1 }; ++i < N; )
  //  A[i] = rand() % N;
  //std::array<int, N> A { };
  std::vector<int> A( N );
  for ( auto& el : A )
    el = rand() % N;
  std::cout << std::boolalpha << sort::check( A.begin(), A.end() ) << '\n';

  sort sorter { SortType::Selection };
  sorter( A.begin(), A.end() );
  std::cout << std::boolalpha << sort::check( A.begin(), A.end() ) << '\n';

  for ( const auto& el : A )
    std::cout << el << ' ';
  std::cout << '\n';
}
