#ifndef __sort_h__
#define __sort_h__

#include <functional>     // std::less
#include <vector>

enum class SortType
{
  Bubble,
  Selection,
  Insertion,
  Merge,
  Quick,
  Shell,
  Heap,
  STD
};

class sort
{
  template<typename T>
  class vector : public std::vector<T>
  {
    using std::vector<T>::vector;
  public:
    T pop()
    {
      T last { this->back() };
      this->pop_back();
      return last;
    }
  };

  SortType __type { };

  template<typename _Iter, typename _Pred>
  void __bubble( const _Iter begin, const _Iter end, _Pred pred );
  template<typename _Iter, typename _Pred>
  void __selection( const _Iter begin, const _Iter end, _Pred pred );
  template<typename _Iter, typename _Pred>
  void __insertion( const _Iter begin, const _Iter end, _Pred pred );
  template<typename _Iter, typename _Pred>
  void __merge( const _Iter begin, const _Iter end, _Pred pred );
  template<typename _Iter, typename _Pred>
  void __quick( const _Iter begin, const _Iter end, _Pred pred );
  template<typename _Iter, typename _Pred>
  void __shell( const _Iter begin, const _Iter end, _Pred pred );
  template<typename _Iter, typename _Pred>
  void __heap( const _Iter begin, const _Iter end, _Pred pred );
  template<typename _Iter, typename _Pred>
  void __std( const _Iter begin, const _Iter end, _Pred pred );

public:

  sort() = delete;
  sort( SortType type = SortType::STD );
  template<typename _Iter, typename _Pred = std::less<>>
  static bool check( const _Iter begin, const _Iter _end, _Pred pred = std::less<> {} );

  template<typename _Iter, typename _Pred = std::less<>>
  void operator()( const _Iter begin, const _Iter _end, _Pred pred = std::less<> {} );
};

void testSort();

#endif