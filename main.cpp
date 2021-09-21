#include "customcast.h"
#include "fibonacci.h"
#include "sort.h"
#include "structs.h"

#include <cstdlib>          // EXIT_SUCCESS
#include <functional>       // function
#include <iostream>         // cout

int main()
{
  //using testFunction = std::function<void()>;
  //using testFunction = void(*)();             // type alias for function pointers
  //void (*p)() { &testArray2d };
  //testFunction p { &testArray2d };
  //auto ptr { &testIntRange };
  //std::function<void()> ptr { &testIntRange };

  // what even is this??
  // int& x();
  // std::cout << x;

  //testArray2d();
  //testCustomCast();
  //testFibonacci( fibonacci_mat, 11 );
  testSort();
  return EXIT_SUCCESS;
}
