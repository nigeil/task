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
#include <sstream>
#include <stdlib.h>
#include <Context.h>
#include <ViewText.h>
#include <Duration.h>
#include <text.h>
#include <util.h>
#include <i18n.h>
#include <main.h>
#include <CmdSummary.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdSummary::CmdSummary ()
{
  _keyword     = "summary";
  _usage       = "task <filter> summary";
  _description = _("Shows a report of task status by project");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
// Project  Remaining  Avg Age  Complete  0%                  100%
// A               12      13d       55%  XXXXXXXXXXXXX-----------
// B              109   3d 12h       10%  XXX---------------------
int CmdSummary::execute (std::string& output)
{
  int rc = 0;

  // Scan the pending tasks.
  handleRecurrence ();

  // Apply filter.
  std::vector <Task> filtered;
  filter (filtered);
  context.tdb2.commit ();

  // Generate unique list of project names from all pending tasks.
  std::map <std::string, bool> allProjects;
  std::vector <Task>::iterator task;
  for (task = filtered.begin (); task != filtered.end (); ++task)
    if (task->getStatus () == Task::pending)
      allProjects[task->get ("project")] = false;

  // Initialize counts, sum.
  std::map <std::string, int> countPending;
  std::map <std::string, int> countCompleted;
  std::map <std::string, double> sumEntry;
  std::map <std::string, int> counter;
  time_t now = time (NULL);

  // Initialize counters.
  std::map <std::string, bool>::iterator project;
  for (project = allProjects.begin (); project != allProjects.end (); ++project)
  {
    countPending   [project->first] = 0;
    countCompleted [project->first] = 0;
    sumEntry       [project->first] = 0.0;
    counter        [project->first] = 0;
  }

  // Count the various tasks.
  for (task = filtered.begin (); task != filtered.end (); ++task)
  {
    std::string project = task->get ("project");
    ++counter[project];

    if (task->getStatus () == Task::pending ||
        task->getStatus () == Task::waiting)
    {
      ++countPending[project];

      time_t entry = strtol (task->get ("entry").c_str (), NULL, 10);
      if (entry)
        sumEntry[project] = sumEntry[project] + (double) (now - entry);
    }

    else if (task->getStatus () == Task::completed)
    {
      ++countCompleted[project];

      time_t entry = strtol (task->get ("entry").c_str (), NULL, 10);
      time_t end   = strtol (task->get ("end").c_str (), NULL, 10);
      if (entry && end)
        sumEntry[project] = sumEntry[project] + (double) (end - entry);
    }
  }


  // Create a table for output.
  ViewText view;
  view.width (context.getWidth ());
  view.add (Column::factory ("string",            _("Project")));
  view.add (Column::factory ("string.right",      _("Remaining")));
  view.add (Column::factory ("string.right",      _("Avg age")));
  view.add (Column::factory ("string.right",      _("Complete")));
  view.add (Column::factory ("string.left_fixed", "0%                        100%"));

  Color bar_color (context.config.get ("color.summary.bar"));
  Color bg_color  (context.config.get ("color.summary.background"));
  Color label (context.config.get ("color.label"));

  view.colorHeader (label);

  int barWidth = 30;
  std::vector <std::string> processed;
  std::map <std::string, bool>::iterator i;
  for (i = allProjects.begin (); i != allProjects.end (); ++i)
  {
    if (countPending[i->first] > 0)
    {
      const std::vector <std::string> parents = extractParents (i->first);
      std::vector <std::string>::const_iterator parent;
      for (parent = parents.begin (); parent != parents.end (); parent++)
      {
        if (std::find (processed.begin (), processed.end (), *parent)
           == processed.end ())
        {
          int row = view.addRow ();
          view.set (row, 0, indentProject (*parent));
          processed.push_back (*parent);
        }
      }

      int row = view.addRow ();
      view.set (row, 0, (i->first == ""
                          ? _("(none)")
                          : indentProject (i->first, "  ", '.')));

      view.set (row, 1, countPending[i->first]);
      if (counter[i->first])
        view.set (row, 2, Duration ((int) (sumEntry[i->first] / (double)counter[i->first])).format ());

      int c = countCompleted[i->first];
      int p = countPending[i->first];
      int completedBar = (c * barWidth) / (c + p);

      std::string bar;
      std::string subbar;
      if (context.color ())
      {
        bar += bar_color.colorize (std::string (           completedBar, ' '));
        bar += bg_color.colorize  (std::string (barWidth - completedBar, ' '));
      }
      else
      {
        bar += std::string (           completedBar, '=')
            +  std::string (barWidth - completedBar, ' ');
      }
      view.set (row, 4, bar);

      char percent[12];
      sprintf (percent, "%d%%", 100 * c / (c + p));
      view.set (row, 3, percent);
      processed.push_back (i->first);
    }
  }

  std::stringstream out;
  if (view.rows ())
  {
    out << optionalBlankLine ()
        << view.render ()
        << optionalBlankLine ();

    if (view.rows ())
      out << format (_("{1} projects"), view.rows ());
    else
      out << _("{1} project");

    out << "\n";
  }
  else {
    out << _("No projects.") << "\n";
    rc = 1;
  }

  output = out.str ();
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
