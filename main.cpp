#include "structs.h"

#include <algorithm>
#include <cstdlib>          // EXIT_SUCCESS
#include <functional>       // std::function
#include <iostream>         // std::cout

int main()
{
  // using testFunction = std::function<void()>;
  // using testFunction = void(*)();             // type alias for function pointers
  // // void (*p)() { &testArray2d };
  // testFunction p { &testArray2d };
  // auto ptr { &testIntRange };
  // // std::function<void()> ptr { &testIntRange };

  // what even is this??
  // int& x();
  // std::cout << x;

  testArray2d();
  return EXIT_SUCCESS;
}
