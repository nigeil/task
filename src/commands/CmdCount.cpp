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

#include <Context.h>
#include <main.h>
#include <text.h>
#include <i18n.h>
#include <CmdCount.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdCount::CmdCount ()
{
  _keyword     = "count";
  _usage       = "task <filter> count";
  _description = _("Counts matching tasks");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdCount::execute (std::string& output)
{
  // Apply filter.
  handleRecurrence ();
  std::vector <Task> filtered;
  filter (filtered);
  context.tdb2.commit ();

  // Find number of matching tasks.  Skip recurring parent tasks.
  int count = 0;
  std::vector <Task>::iterator it;
  for (it = filtered.begin (); it != filtered.end (); ++it)
    if (it->getStatus () != Task::recurring)
      ++count;

  output = format (count) + "\n";
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
