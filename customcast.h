// test casting between 2 user-defined classes using overloading

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>         // std::cout

class polar;

class cartesian // cartesian coordinates
{
  double __x;
  double __y;

public:

  cartesian( double x, double y ) :
    __x { x },
    __y { y }
  { }
  operator polar() const;
  void view() const { std::cout << "Cartesian : ( " << __x << ' ' << __y << " )\n"; }
};

class polar // polar coordinates
{
  double __r;     // radial distance
  double __th;    // degrees

public:

  polar( double r, double th ) :
    __r { r },
    __th { th }
  {}
  operator cartesian() const
  {
    return cartesian { __r * std::cos( __th * M_PI / 180.0 ), __r * std::sin( __th * M_PI / 180.0 ) };
  }
  void view() const { std::cout << "Polar : ( " << __r << ' ' << __th << "\370 )\n"; }  // 370 is code for degree symbol
};

inline
cartesian::operator polar() const
{
  auto theta { (__x == 0) ? M_PI_2 * ((__y > 0) - (0 > __y)) : std::atan( __y / __x ) };
  return polar
  {
    std::sqrt( __x * __x + __y * __y ),
    theta * 180.0 / M_PI
  };
}

inline
void testCustomCast()
{
  cartesian point { 8.0, 6.0 };
  point.view();
  auto point2 { static_cast<polar>(point) };
  point2.view();
  std::cout << '\n';
  point2 = { 6, 60 };
  point2.view();
  point = static_cast<cartesian>(point2);
  point.view();
}