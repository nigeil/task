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
#include <iostream>
#include <sstream>
#include <Context.h>
#include <Transport.h>
#include <JSON.h>
#include <text.h>
#include <util.h>
#include <i18n.h>
#include <main.h>
#include <CmdImport.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdImport::CmdImport ()
{
  _keyword     = "import";
  _usage       = "task          import <file> [<file> ...]";
  _description = _("Imports JSON files");
  _read_only   = false;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdImport::execute (std::string& output)
{
  int rc = 0;
  int count = 0;

	// Use the description as a file name.
  std::vector <std::string> words = context.a3.extract_words ();
  if (! words.size ())
    throw std::string (_("You must specify a file to import."));

  std::vector <std::string>::iterator word;
  for (word = words.begin (); word != words.end (); ++word)
  {
    std::string file = *word;
    std::cout << format (_("Importing '{1}'"), file) << "\n";

    std::string tmpfile = "";
    Uri uri (file);
    uri.parse ();

    Transport* transport;
    if ((transport = Transport::getTransport (uri)) != NULL)
    {
      std::string location (context.config.get ("data.location"));
      tmpfile = location + "/import.data";
      transport->recv (tmpfile);
      delete transport;

      file = tmpfile;
    }

    // Load the file.
    std::vector <std::string> lines;
    File::read (file, lines);

    std::vector <std::string>::iterator line;
    for (line = lines.begin (); line != lines.end (); ++line)
    {
      std::string object = trimLeft (
                             trimRight (
                               trimRight (
                                 trim (*line), ","),
                               "]"),
                             "[");
      // Skip blanks.  May be caused by the trim calls above.
      if (! object.length ())
        continue;

      // Parse the whole thing.
      Task task (object);

      context.tdb2.add (task);
      ++count;
      std::cout << "  "
                << task.get ("uuid")
                << " "
                << task.get ("description")
                << "\n";
    }
  }

  context.tdb2.commit ();

  context.footnote (format (_("Imported {1} tasks."), count));
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
