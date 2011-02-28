//
// Flatland: a minimal n-dimensional command line plot utility
// Copyright (C) 2008 Adam Ierymenko <adam.ierymenko@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _FLATLAND_CARTESIANCOORD_HPP
#define _FLATLAND_CARTESIANCOORD_HPP

/**
 * Simple container for X,Y coordinates
 */
class CartesianCoord
{
public:
  CartesianCoord() :
    x(0),
    y(0)
  {
  }

  CartesianCoord(const double x_,const double y_) :
    x(x_),
    y(y_)
  {
  }

  CartesianCoord operator+(const double sc) const
  {
    return CartesianCoord(x + sc,y + sc);
  }
  CartesianCoord operator-(const double sc) const
  {
    return CartesianCoord(x - sc,y - sc);
  }
  CartesianCoord operator*(const double sc) const
  {
    return CartesianCoord(x * sc,y * sc);
  }
  CartesianCoord operator/(const double sc) const
  {
    return CartesianCoord(x * sc,y * sc);
  }

  CartesianCoord &operator+=(const double sc)
  {
    x += sc;
    y += sc;
    return *this;
  }
  CartesianCoord &operator-=(const double sc)
  {
    x -= sc;
    y -= sc;
    return *this;
  }
  CartesianCoord &operator*=(const double sc)
  {
    x *= sc;
    y *= sc;
    return *this;
  }
  CartesianCoord &operator/=(const double sc)
  {
    x /= sc;
    y /= sc;
    return *this;
  }

  inline CartesianCoord operator-() const
  {
    return CartesianCoord(-x,-y);
  }

  double x;
  double y;
};

#endif
