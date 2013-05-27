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

#include <cmake.h>
#include <sstream>
#include <stdlib.h>
#include <Context.h>
#include <ViewText.h>
#include <commit.h>
#include <CmdVersion.h>
#include <text.h>
#include <i18n.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdVersion::CmdVersion ()
{
  _keyword     = "version";
  _usage       = "task          version";
  _description = _("Shows the taskwarrior version number");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdVersion::execute (std::string& output)
{
  std::stringstream out;

  // Create a table for the disclaimer.
  int width = context.getWidth ();
  ViewText disclaimer;
  disclaimer.width (width);
  disclaimer.add (Column::factory ("string", ""));
  disclaimer.set (disclaimer.addRow (), 0, _("Taskwarrior may be copied only under the terms of the MIT license, which may be found in the taskwarrior source kit."));

  // Create a table for the URL.
  ViewText link;
  link.width (width);
  link.add (Column::factory ("string", ""));
  link.set (link.addRow (), 0, _("Documentation for taskwarrior can be found using 'man task', 'man taskrc', 'man task-tutorial', 'man task-color', 'man task-sync', 'man task-faq' or at http://taskwarrior.org"));

  Color bold ("bold");

  out << "\n"
      << format ( _("{1} {2} built for "),
                 (context.color () ? bold.colorize (PACKAGE) : PACKAGE),
                 (context.color () ? bold.colorize (VERSION) : VERSION))

#if defined (DARWIN)
      << "darwin"
#elif defined (SOLARIS)
      << "solaris"
#elif defined (CYGWIN)
      << "cygwin"
#elif defined (HAIKU)
      << "haiku"
#elif defined (OPENBSD)
      << "openbsd"
#elif defined (FREEBSD)
      << "freebsd"
#elif defined (NETBSD)
      << "netbsd"
#elif defined (LINUX)
      << "linux"
#elif defined (KFREEBSD)
      << "gnu-kfreebsd"
#elif defined (GNUHURD)
      << "gnu-hurd"
#else
      << _("unknown")
#endif

      << "\n"
      << "Copyright (C) 2006 - 2013 P. Beckingham, F. Hernandez."
      << "\n"
      << "\n"
      << disclaimer.render ()
      << "\n"
      << link.render ()
      << "\n";

  output = out.str ();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
CmdCompletionVersion::CmdCompletionVersion ()
{
  _keyword     = "_version";
  _usage       = "task          _version";
  _description = _("Shows only the taskwarrior version number");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdCompletionVersion::execute (std::string& output)
{
#ifdef HAVE_COMMIT
  output = COMMIT;
#else
  output = VERSION;
#endif
  output += "\n";
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
