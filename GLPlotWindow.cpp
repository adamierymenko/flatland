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

#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>
#include "CartesianCoord.hpp"
#include "GLPlotWindow.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

GLPlotWindow::GLPlotWindow() :
  points(),
  lines(),
  sphereListId(glGenLists(1))
{
}

GLPlotWindow::~GLPlotWindow()
{
}

void GLPlotWindow::addPoint(const CartesianCoord &c,const float r,const float g,const float b)
{
  points.push_back(Point(c,__RGB(r,g,b)));
}

void GLPlotWindow::addLine(const CartesianCoord &start,const CartesianCoord &end,const float r,const float g,const float b)
{
  lines.push_back(Line(LineEndpoints(start,end),__RGB(r,g,b)));
}

void GLPlotWindow::clear()
{
  points.clear();
  lines.clear();
}

void GLPlotWindow::draw(const float pointSize) const
{
  double maxX = 0.0;
  double maxY = 0.0;
  for(Points::const_iterator i=points.begin();i!=points.end();++i) {
    maxX = std::max(maxX,fabs(i->first.x));
    maxY = std::max(maxY,fabs(i->first.y));
  }
  maxX += 0.01;
  maxY += 0.01;

  if (lines.size()) {
    glLoadIdentity();
    glBegin(GL_LINES);
    glLineWidth(10.0);
    for(Lines::const_iterator i=lines.begin();i!=lines.end();++i) {
      double x1 = i->first.first.x / maxX;
      double y1 = i->first.first.y / maxY;
      double x2 = i->first.second.x / maxX;
      double y2 = i->first.second.y / maxY;
      if (!std::isnormal(x1)) x1 = 0.0;
      if (!std::isnormal(y1)) y1 = 0.0;
      if (!std::isnormal(x2)) x2 = 0.0;
      if (!std::isnormal(y2)) y2 = 0.0;
      glColor4f(i->second.red,i->second.green,i->second.blue,0.5f);
      glVertex3d(x1,y1,0.0);
      glVertex3d(x2,y2,0.0);
    }
    glEnd();
  }

  if (points.size()) {
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    for(Points::const_iterator i=points.begin();i!=points.end();++i) {
      double x = i->first.x / maxX;
      double y = i->first.y / maxY;
      if (!std::isnormal(x)) x = 0.0;
      if (!std::isnormal(y)) y = 0.0;
      glColor4f(i->second.red,i->second.green,i->second.blue,0.5f);
      glVertex3d(x,y,0.0);
    }
    glEnd();
  }
}
