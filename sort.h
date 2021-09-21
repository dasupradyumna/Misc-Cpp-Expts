#ifndef __sort_h__
#define __sort_h__

#include <functional>     // std::less

enum class SortType
{
  Bubble,
  Selection,
  Insertion,
  STD
};

class sort
{
  SortType __type { };

  template<typename _Iter, typename _Pred>
  void __bubble( _Iter begin, _Iter end, _Pred pred );
  template<typename _Iter, typename _Pred>
  void __selection( _Iter begin, _Iter end, _Pred pred );
  template<typename _Iter, typename _Pred>
  void __insertion( _Iter begin, _Iter end, _Pred pred );

public:

  sort() = delete;
  sort( SortType type = SortType::STD );
  template<typename _Iter, typename _Pred = std::less<>>
  static bool check( _Iter begin, _Iter _end, _Pred pred = std::less<> {} );

  template<typename _Iter, typename _Pred = std::less<>>
  void operator()( _Iter begin, _Iter _end, _Pred pred = std::less<> {} );
};

void testSort();

#endif