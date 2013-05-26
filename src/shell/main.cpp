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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstring>

#ifdef CYGWIN
#include <time.h>
#else
#include <sys/time.h>
#endif

#include <text.h>
#include <i18n.h>
#include <Color.h>
#include <Context.h>
#include <Readline.h>

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

  std::string usage = _(
    "Usage: tasksh [<commands-file>]    Execute task commands inside <commands-file> if given,\n" \
    "                                   or otherwise, start interactive task shell.\n" \
    "       tasksh --version            Print task version.\n" \
    "       tasksh --help               Print this help.\n");

  bool read_from_file = false;

  if (argc > 2)
  {
    std::cout << usage << "\n";
    return -1;
  }
  else if (argc == 2)
  {
    if (!strcmp (argv[1], "--version"))
    {
      std::cout << VERSION << "\n";
      return 0;
    }
    else if (!strcmp (argv[1], "--help"))
    {
      std::cout << usage << "\n";
      return 0;
    }
    else
    {
      // The user has given tasksh a task commands file to execute
      File input_file = File (argv[1]);
      if (!input_file.exists ())
      {
        std::cout << _("Input file does not exist.") << "\n";
        std::cout << usage << "\n";

        return -1;
      }

      read_from_file = true;
    }
  }

  // If a file is given, read from it
  std::ifstream fin;
  if (read_from_file)
  {
    fin.open (argv[1]);
  }

  // Commands may be redirected too
  std::istream &in = read_from_file ? fin : std::cin;

  if (Readline::interactiveMode (in))
  {
    // Begining initilaization
    if (context.initialize (0, NULL))
    {
      return -1;
    }

    // Display some kind of welcome message.
    Color bold (Color::nocolor, Color::nocolor, false, true, false);

    std::cout << (context.color () ? bold.colorize (PACKAGE_STRING)
                                   : PACKAGE_STRING)
              << " shell\n\n"
              << _("Enter any task command (such as 'list'), or hit 'Enter'.\n" \
                   "There is no need to include the 'task' command itself.\n" \
                   "Enter 'quit' (or 'bye', 'exit') to end the session.\n")
              << "\n";
  }

  // Make a copy because context.clear will delete them.
  std::string permanent_overrides;
  std::vector <Arg>::iterator i;
  for (i = context.a3.begin (); i != context.a3.end (); ++i)
  {
    if (i->_category == Arg::cat_rc ||
        i->_category == Arg::cat_override)
    {
      if (i != context.a3.begin ())
        permanent_overrides += " ";

      permanent_overrides += i->_raw;
    }
  }

  std::string input;

  std::vector <std::string> quit_commands;
  quit_commands.push_back ("quit");
  quit_commands.push_back ("exit");
  quit_commands.push_back ("bye");

  // The event loop.
  while (in)
  {
    std::string prompt (context.config.get ("shell.prompt") + " ");
    context.clear ();

    if (Readline::interactiveMode (in))
    {
      input = Readline::gets (prompt);

      // if a string has nothing but whitespaces, ignore it
      if (input.find_first_not_of (" \t") == std::string::npos)
        continue;
    }
    else
    {
      std::getline (in, input);

      // if a string has nothing but whitespaces, ignore it
      if (input.find_first_not_of (" \t") == std::string::npos)
        continue;

      std::cout << prompt << input << '\n';
    }

    try
    {
      Wordexp w ("task " + trim (input + permanent_overrides));

      for (int i = 0; i < w.argc (); ++i)
      {
        if (std::find (quit_commands.begin (), quit_commands.end (),
                       lowerCase (w.argv (i))) != quit_commands.end ())
        {
          context.clearMessages ();
          return 0;
        }
      }

      int status = context.initialize (w.argc (), (const char**)w.argv ());
      if (status == 0)
        context.run ();
    }

    catch (const std::string& error)
    {
      std::cerr << error << '\n';
      return -1;
    }

    catch (...)
    {
      std::cerr << _("Unknown error.") << '\n';
      return -2;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
