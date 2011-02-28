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

#ifndef _FLATLAND_STARCOORD_HPP
#define _FLATLAND_STARCOORD_HPP

#include <math.h>
#include <vector>
#include "CartesianCoord.hpp"

/**
 * N-dimensional down-projector using the star coordinates algorithm
 */
class StarCoord
{
public:
  StarCoord() :
    theta()
  {
  }

  StarCoord(const unsigned long n) :
    theta(n,0.0)
  {
    double w = M_PI / (double)n;
    double t = 0.0;
    for(unsigned long i=0;i<n;++i) {
      theta[i] = t;
      t += w;
    }
  }

  /**
   * @param vb Start of vector of doubles
   * @param ve End of vector of doubles
   * @return Cartesian coordinates of point projected onto star plot
   */
  template<typename I>
  inline CartesianCoord operator()(const I &vb,const I &ve) const
  {
    double x = 0.0;
    double y = 0.0;
    std::vector<double>::const_iterator a(theta.begin());
    I i(vb);
    while ((i != ve)&&(a != theta.end())) {
      x += (cos(*a) * (*i));
      y += (sin(*a) * (*i));
      ++i;
      ++a;
    }
    return CartesianCoord(x,y);
  }

  /**
   * Theta (angle in radians) for each dimension's axis
   *
   * The size of this vector is the number of dimensions that this instance
   * is set up to down-project.
   */
  std::vector<double> theta;
};

#endif
