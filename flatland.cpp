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
#include <fstream>
#include <limits>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "CartesianCoord.hpp"
#include "StarCoord.hpp"
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

#define FLATLAND_VERSION "0.5"

// Delimiters for splitting CSV data
#define CSV_DELIM "\r\n \t,|~^!$_()[]<>"

// Default window dimensions
#define DEFAULT_WINDOW_X 640
#define DEFAULT_WINDOW_Y 480

#define VALUE_COLORING_HIGHER_IS_WARMER 1
#define VALUE_COLORING_LOWER_IS_WARMER 2
#define VALUE_COLORING_LOG_HIGHER_IS_WARMER 3
#define VALUE_COLORING_LOG_LOWER_IS_WARMER 4

static GLPlotWindow *plotter = (GLPlotWindow *)0;
static bool isFullScreen = false;
static bool reverseVid = false;
static int windowSizeX = DEFAULT_WINDOW_X;
static int windowSizeY = DEFAULT_WINDOW_Y;
static float pointSize = 2.0f;

// Comparators for sorting vectors by their last value in both sort orders
struct SortByBackForwardComparator
{
  inline bool operator()(const std::vector<double> &a,const std::vector<double> &b) const
  {
    return (a.back() < b.back());
  }
};
struct SortByBackReverseComparator
{
  inline bool operator()(const std::vector<double> &a,const std::vector<double> &b) const
  {
    return (b.back() < a.back());
  }
};

// GLUT display function
extern "C" void fl_display()
{
  glMatrixMode(GL_MODELVIEW);
  if (reverseVid)
    glClearColor(1.0f,1.0f,1.0f,0.0f);
  else glClearColor(0.0f,0.0f,0.0f,0.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  plotter->draw(pointSize);
  glFlush();
}

// GLUT viewport reshape function
extern "C" void fl_reshape(int w,int h)
{
  glViewport(0,0,w,h);

  if (!isFullScreen) {
    windowSizeX = w;
    windowSizeY = h;
  }
}

// GLUT keyboard handler
extern "C" void fl_keyboard(const unsigned char key,const int x,const int y)
{
  switch(key) {
    case 70: // F
    case 102: // f
      if (isFullScreen) {
        isFullScreen = false;
        glutReshapeWindow(windowSizeX,windowSizeY);
      } else {
        isFullScreen = true;
        glutFullScreen();
      }
      break;
    case 82: // R
    case 114: // r
      reverseVid = !reverseVid;
      glutPostRedisplay();
      break;
    case 27: // escape
    case 81: // Q
    case 113: // q
      delete plotter;
      exit(0);
      break;
  }
}

static void readPlotData(std::istream &in,const long valueColoring,const bool noAxes,const long ignore,long fixedDimensionality)
{
  char buf[131072];

  if ((valueColoring > 0)&&(fixedDimensionality > 0))
    ++fixedDimensionality;

  // Read from input stream until EOF
  std::vector< std::vector<double> > data;
  std::vector<double> datum;
  unsigned long maxd = 0;
  double maxima = 0.0;
  double maxLC = 0.0;
  double minLC = std::numeric_limits<double>::max();
  long lineNo = 0;
  while (in) {
    in.getline(buf,sizeof(buf));

    if (lineNo++ < ignore)
      continue;

    for(char *d=strtok(buf,CSV_DELIM);d;d=strtok((char *)0,CSV_DELIM)) {
      if (strlen(d)) {
        double x = strtod(d,(char **)0);
        datum.push_back(x);
        maxima = std::max(maxima,fabs(x));
      }
    }

    if (datum.size()&&((fixedDimensionality <= 0)||(datum.size() == fixedDimensionality))) {
      data.push_back(datum);
      maxd = std::max(maxd,(unsigned long)datum.size());
      maxLC = std::max(maxLC,datum.back());
      minLC = std::min(minLC,datum.back());
    }
    datum.clear();
  }

  // Add data points and calculate maxima for each dimension. If we use
  // the warmer/cooler coloring methods, we sort first so that the "hottest"
  // points will get positioned last and will be the most prominent.
  StarCoord scp;
  switch(valueColoring) {
    case VALUE_COLORING_HIGHER_IS_WARMER:
      scp = StarCoord(--maxd);
      std::sort(data.begin(),data.end(),SortByBackForwardComparator());
      for(std::vector< std::vector<double> >::const_iterator i=data.begin();i!=data.end();++i)
        plotter->addPoint(scp(i->begin(),i->end() - 1),0.02f,(float)((((i->back() - minLC) / (maxLC - minLC)) * 0.95) + 0.05),0.02f);
      break;
    case VALUE_COLORING_LOWER_IS_WARMER:
      scp = StarCoord(--maxd);
      std::sort(data.begin(),data.end(),SortByBackReverseComparator());
      for(std::vector< std::vector<double> >::const_iterator i=data.begin();i!=data.end();++i)
        plotter->addPoint(scp(i->begin(),i->end() - 1),0.02f,(float)(((1.0 - ((i->back() - minLC) / (maxLC - minLC))) * 0.95) + 0.05),0.02f);
      break;
    case VALUE_COLORING_LOG_HIGHER_IS_WARMER:
      scp = StarCoord(--maxd);
      std::sort(data.begin(),data.end(),SortByBackForwardComparator());
      maxLC = log(maxLC);
      minLC = log(minLC);
      for(std::vector< std::vector<double> >::const_iterator i=data.begin();i!=data.end();++i)
        plotter->addPoint(scp(i->begin(),i->end() - 1),0.02f,(float)((((log(i->back()) - minLC) / (maxLC - minLC)) * 0.95) + 0.05),0.02f);
      break;
    case VALUE_COLORING_LOG_LOWER_IS_WARMER:
      scp = StarCoord(--maxd);
      std::sort(data.begin(),data.end(),SortByBackReverseComparator());
      maxLC = log(maxLC);
      minLC = log(minLC);
      for(std::vector< std::vector<double> >::const_iterator i=data.begin();i!=data.end();++i)
        plotter->addPoint(scp(i->begin(),i->end() - 1),0.02f,(float)(((1.0 - ((log(i->back()) - minLC) / (maxLC - minLC))) * 0.95) + 0.05),0.02f);
      break;
    default:
      scp = StarCoord(maxd);
      for(std::vector< std::vector<double> >::const_iterator i=data.begin();i!=data.end();++i)
        plotter->addPoint(scp(i->begin(),i->end()),0.5f,0.5f,0.5f);
      break;
  }

  // Add axis lines from 0 to maxima and -maxima
  if (!noAxes) {
    std::vector<double> tmp(maxd,0.0);
    for(unsigned long i=0;i<tmp.size();++i) {
      tmp[i] = maxima;
      CartesianCoord axisep = scp(tmp.begin(),tmp.end());
      plotter->addLine(CartesianCoord(),axisep,0.05f,0.05f,0.2f);
      plotter->addLine(-axisep,CartesianCoord(),0.2f,0.05f,0.05f);
      tmp[i] = 0.0;
    }
  }
}

static void showHelp()
{
  std::cout << "Flatland: A Quick N-dimensional Plotting Tool (version " << FLATLAND_VERSION << ')' << std::endl;
  std::cout << "(c)2008 Adam Ierymenko, released under the GPL v3" << std::endl;
  std::cout << std::endl;
  std::cout << "Usage: flatland [-opts] [<file>]" << std::endl;
  std::cout << std::endl;
  std::cout << "Available options:" << std::endl;
  std::cout << " -h    Display this help" << std::endl;
  std::cout << " -v M  Treat last column as a value (see below)" << std::endl;
  std::cout << " -p S  Size of each point in pixels (default: 2)" << std::endl;
  std::cout << " -d N  Plot in N dimensions, discard rows without exactly N columns" << std::endl;
  std::cout << " -r    Reverse video (white background)" << std::endl;
  std::cout << " -f    Ignore first line of input data (to ignore headers, etc.)" << std::endl;
  std::cout << " -a    Do not show axis lines" << std::endl;
  std::cout << std::endl;
  std::cout << "With -v, the following modes can be used:" << std::endl;
  std::cout << "  1    Intensity, higher is \"warmer\"" << std::endl;
  std::cout << "  2    Intensity, lower is \"warmer\"" << std::endl;
  std::cout << "  3    log(Intensity), higher is \"warmer\" (values must be >= 0)" << std::endl;
  std::cout << "  4    log(Intensity), lower is \"warmer\" (values must be >= 0)" << std::endl;
  std::cout << std::endl;
  std::cout << "Data must be in ASCII tabular form and may be delimited by white space" << std::endl;
  std::cout << "or any of the following characters: ,|~^!$_()[]<>" << std::endl;
  std::cout << std::endl;
  std::cout << "The number of dimensions is determined by the longest vector in the" << std::endl;
  std::cout << "input data. Shorter vectors have their missing dimensions filled with" << std::endl;
  std::cout << "zero. Spurious tokens and empty fields are ignored." << std::endl;
  std::cout << std::endl;
  std::cout << "While the plot is showing, the following keyboard commands may be used:" << std::endl;
  std::cout << "  f    Toggle full screen / windowed" << std::endl;
  std::cout << "  r    Toggle reverse video" << std::endl;
  std::cout << "  q    Quit (ESC quits too)" << std::endl;
}

int main(int argc,char **argv)
{
  bool noAxes = false;
  long ignore = 0;
  long valueColoring = 0;
  long fixedDimensionality = 0;
  std::istream *in = &std::cin;

  for(int i=1;i<argc;++i) {
    switch(argv[i][0]) {
      case 0:
        break;
      case '-':
        switch(argv[i][1]) {
          case 'v':
            if (++i >= argc) {
              showHelp();
              return 0;
            }
            valueColoring = strtol(argv[i],(char **)0,10);
            break;
          case 'p':
            if (++i >= argc) {
              showHelp();
              return 0;
            }
            pointSize = (float)strtol(argv[i],(char **)0,10);
            break;
          case 'd':
            if (++i >= argc) {
              showHelp();
              return 0;
            }
            fixedDimensionality = strtol(argv[i],(char **)0,10);
            break;
          case 'a':
            noAxes = true;
            break;
          case 'r':
            reverseVid = true;
            break;
          case 'f':
            ignore = 1;
            break;
          case 'h':
          default:
            showHelp();
            return 0;
        }
        break;
      default:
        try {
          in = new std::ifstream(argv[i]);
          if (!*in) {
            std::cerr << "Could not open " << argv[i] << std::endl;
            return -1;
          }
        } catch ( ... ) {
          std::cerr << "Could not open " << argv[i] << std::endl;
          return -1;
        }
        break;
    }
  }

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
  glutInitWindowSize(DEFAULT_WINDOW_X,DEFAULT_WINDOW_Y);
  glutCreateWindow("Flatland");

  glutDisplayFunc(fl_display);
  glutReshapeFunc(fl_reshape);
  glutKeyboardFunc(fl_keyboard);

  plotter = new GLPlotWindow();
  readPlotData(*in,valueColoring,noAxes,ignore,fixedDimensionality);

  glutMainLoop();
}
