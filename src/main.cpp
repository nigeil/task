////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2006-2013, Paul Beckingham, Federico Hernandez.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// http://www.opensource.org/licenses/mit-license.php
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>
#include <stdlib.h>

#ifdef CYGWIN
#include <time.h>
#else
#include <sys/time.h>
#endif

#include <i18n.h>
#include <Context.h>
#include <cmake.h>

Context context;

#ifdef HAVE_SRANDOM
#define srand(x) srandom(x)
#endif

////////////////////////////////////////////////////////////////////////////////
int main (int argc, const char** argv)
{
  // Set up randomness.
#ifdef CYGWIN
  srand (time (NULL));
#else
  struct timeval tv;
  gettimeofday (&tv, NULL);
  srand (tv.tv_usec);
#endif

#ifdef HAVE_GETTEXT
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, TASK_LOCALEDIR);
  textdomain (PACKAGE);
#endif

  int status = 0;

  if (argc == 2 && !strcmp (argv[1], "--version"))
  {
    std::cout << VERSION << "\n";
  }
  else
  {
    try
    {
      status = context.initialize (argc, argv);
      if (status == 0)
        status = context.run ();
    }

    catch (const std::string& error)
    {
      std::cerr << error << "\n";
      status = -1;
    }

    catch (...)
    {
      std::cerr << _("Unknown error.") << "\n";
      status = -2;
    }
  }

  return status;
}

////////////////////////////////////////////////////////////////////////////////
