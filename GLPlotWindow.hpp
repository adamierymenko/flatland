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

#ifndef _FLATLAND_GLPLOTWINDOW_HPP
#define _FLATLAND_GLPLOTWINDOW_HPP

#include <vector>
#include <utility>
#include <map>
#include "CartesianCoord.hpp"

/**
 * 2d plotter using OpenGL
 */
class GLPlotWindow
{
public:
  /**
   * Create a new GL plotter
   *
   * The GL context must be ready to go when this constructor is called.
   */
  GLPlotWindow();

  ~GLPlotWindow();

  /**
   * @param c 2d point to plot
   * @param r Red
   * @param g Green
   * @param b Blue
   */
  void addPoint(const CartesianCoord &c,const float r,const float g,const float b);

  /**
   * @param start Staring coordinates of line
   * @param end Ending coordinates of line
   * @param r Red
   * @param g Green
   * @param b Blue
   */
  void addLine(const CartesianCoord &start,const CartesianCoord &end,const float r,const float g,const float b);

  /**
   * Clear plot
   */
  void clear();

  /**
   * Send OpenGL commands to redraw window
   *
   * This clears and redraws the current buffer, but doesn't swap buffers
   * or anything else like that.
   *
   * @param pointSize Size of each point in pixels
   */
  void draw(const float pointSize) const;

private:
  // Make class non-copyable
  GLPlotWindow(const GLPlotWindow &gw) {}
  inline GLPlotWindow &operator=(const GLPlotWindow &gw) { return *this; }

  class __RGB
  {
  public:
    __RGB() :
      red(1.0F),
      green(1.0F),
      blue(1.0F)
    {
    }

    __RGB(const __RGB &r) :
      red(r.red),
      green(r.green),
      blue(r.blue)
    {
    }

    __RGB(const float r,const float g,const float b) :
      red(r),
      green(g),
      blue(b)
    {
    }

    inline __RGB &operator=(const __RGB &r)
    {
      red = r.red;
      green = r.green;
      blue = r.blue;
      return *this;
    }

    float red;
    float green;
    float blue;
  };

  typedef std::pair< CartesianCoord,__RGB > Point;
  typedef std::vector< Point > Points;
  typedef std::pair< CartesianCoord,CartesianCoord > LineEndpoints;
  typedef std::pair< LineEndpoints,__RGB > Line;
  typedef std::vector< Line > Lines;

  Points points;
  Lines lines;
  unsigned int sphereListId;
};

#endif
