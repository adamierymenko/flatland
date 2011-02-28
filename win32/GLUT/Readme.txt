GLUT 3.7.6 for MinGW, release 2

This is a binary package of Nate Robins’ port of GLUT
(http://www.xmission.com/~nate/glut.html), which I have put together to allow
GLUT applications to be compiled with MinGW (http://www.mingw.org/). If you
experience problems with this package, you can send an email via
http://www.transmissionzero.co.uk/contact/ . Please don’t contact the original
author unless you are *certain* you have found a bug in GLUT for win32!

The “glut.h” header file has an extra macro defined to allow it to work with
MinGW, and the library file is provided in a format suitable for use with MinGW.
Other than that, the package is the same as the original. Please read the file
“README-win32.txt” for version and legal information.

Installation

The “glut.h” header should be placed in the “include\GL\” folder, and the
“libglut32.a” file should be placed in the “lib\” folder. Please read the
tutorial at http://www.transmissionzero.co.uk/computing/using-glut-with-mingw/
for further information and code examples.


Martin Payne
2007–02–25

http://www.transmissionzero.co.uk/
