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
#include <Context.h>
#include <text.h>
#include <util.h>
#include <i18n.h>
#include <main.h>
#include <CmdDuplicate.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdDuplicate::CmdDuplicate ()
{
  _keyword     = "duplicate";
  _usage       = "task <filter> duplicate <mods>";
  _description = _("Duplicates the specified tasks");
  _read_only   = false;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdDuplicate::execute (std::string& output)
{
  int rc = 0;
  int count = 0;

  // Apply filter.
  std::vector <Task> filtered;
  filter (filtered);
  if (filtered.size () == 0)
  {
    context.footnote (_("No tasks specified."));
    return 1;
  }

  // Apply the command line modifications to the new task.
  A3 modifications = context.a3.extract_modifications ();

  // Accumulated project change notifications.
  std::map <std::string, std::string> projectChanges;

  std::vector <Task>::iterator task;
  for (task = filtered.begin (); task != filtered.end (); ++task)
  {
    // Duplicate the specified task.
    Task dup (*task);
    dup.set ("uuid", uuid ());     // Needs a new UUID.
    dup.remove ("start");          // Does not inherit start date.
    dup.remove ("end");            // Does not inherit end date.
    dup.remove ("entry");          // Does not inherit entry date.

    // When duplicating a child task, downgrade it to a plain task.
    if (dup.has ("parent"))
    {
      dup.remove ("parent");
      dup.remove ("recur");
      dup.remove ("until");
      dup.remove ("imask");

      std::cout << format (_("Note: task {1} was a recurring task.  The duplicated task is not."), task->id)
          << "\n";
    }

    // When duplicating a parent task, create a new parent task.
    else if (dup.getStatus () == Task::recurring)
    {
      dup.remove ("mask");

      std::cout << format (_("Note: task {1} was a parent recurring task.  The duplicated task is too."), task->id)
          << "\n";
    }

    dup.setStatus (Task::pending); // Does not inherit status.
                                   // Must occur after Task::recurring check.

    modify_task_annotate (dup, modifications);

    if (permission (dup,
                    format (_("Duplicate task {1} '{2}'?"),
                            task->id,
                            task->get ("description")),
                    filtered.size ()))
    {
      context.tdb2.add (dup);
      ++count;
      feedback_affected (_("Duplicated task {1} '{2}'."), *task);

      if (context.verbose ("new-id"))
        std::cout << format (_("Created task {1}."), context.tdb2.next_id ()) + "\n";

      if (context.verbose ("project"))
        projectChanges[task->get ("project")] = onProjectChange (*task);
    }
    else
    {
      std::cout << _("Task not duplicated.") << "\n";
      rc = 1;
      if (_permission_quit)
        break;
    }
  }

  // Now list the project changes.
  std::map <std::string, std::string>::iterator i;
  for (i = projectChanges.begin (); i != projectChanges.end (); ++i)
    if (i->first != "")
      context.footnote (i->second);

  context.tdb2.commit ();
  feedback_affected (count == 1 ? _("Duplicated {1} task.") : _("Duplicated {1} tasks."), count);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
