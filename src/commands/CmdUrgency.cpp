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

#include <sstream>
#include <stdlib.h>
#include <Context.h>
#include <main.h>
#include <text.h>
#include <i18n.h>
#include <CmdUrgency.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdUrgency::CmdUrgency ()
{
  _keyword     = "_urgency";
  _usage       = "task <filter> _urgency";
  _description = _("Displays the urgency measure of a task");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdUrgency::execute (std::string& output)
{
  // Apply filter.
  std::vector <Task> filtered;
  filter (filtered);

  if (filtered.size () == 0)
  {
    context.footnote (_("No tasks specified."));
    return 1;
  }

  // Display urgency for the selected tasks.
  std::stringstream out;
  std::vector <Task>::iterator task;
  for (task = filtered.begin (); task != filtered.end (); ++task)
  {
    if (task->id)
    {
      // TRANSLATORS: {1} is the ID, {2} is the urgency.
      out << format (_("task {1} urgency {2}"),
                     task->id, task->urgency ())
          << "\n";
    }
    else
    {
      std::string uuid = task->get ("uuid");
      out << format (_("task {1} urgency {2}"),
                     uuid, task->urgency ())
          << "\n";
    }
  }

  output = out.str ();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
