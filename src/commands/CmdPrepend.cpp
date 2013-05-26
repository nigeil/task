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
#include <util.h>
#include <text.h>
#include <i18n.h>
#include <main.h>
#include <CmdPrepend.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdPrepend::CmdPrepend ()
{
  _keyword     = "prepend";
  _usage       = "task <filter> prepend <mods>";
  _description = _("Prepends text to an existing task description");
  _read_only   = false;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdPrepend::execute (std::string& output)
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
  if (!modifications.size ())
    throw std::string (_("Additional text must be provided."));

  // Accumulated project change notifications.
  std::map <std::string, std::string> projectChanges;

  std::vector <Task>::iterator task;
  for (task = filtered.begin (); task != filtered.end (); ++task)
  {
    Task before (*task);

    // Prepend to the specified task.
    std::string question = format (_("Prepend to task {1} '{2}'?"),
                                   task->id,
                                   task->get ("description"));

    modify_task_description_prepend (*task, modifications);

    if (permission (*task, taskDifferences (before, *task) + question, filtered.size ()))
    {
      context.tdb2.modify (*task);
      ++count;
      feedback_affected (_("Prepending to task {1} '{2}'."), *task);
      if (context.verbose ("project"))
        projectChanges[task->get ("project")] = onProjectChange (*task, false);

      // Prepend to siblings.
      if (task->has ("parent"))
      {
        std::vector <Task> siblings = context.tdb2.siblings (*task);
        if (siblings.size () &&
            confirm (_("This is a recurring task.  Do you want to prepend to all pending recurrences of this same task?")))
        {
          std::vector <Task>::iterator sibling;
          for (sibling = siblings.begin (); sibling != siblings.end (); ++sibling)
          {
            modify_task_description_prepend (*sibling, modifications);
            context.tdb2.modify (*sibling);
            ++count;
            feedback_affected (_("Prepending to recurring task {1} '{2}'."), *sibling);
          }

          // Prepend to the parent
          Task parent;
          context.tdb2.get (task->get ("parent"), parent);
          modify_task_description_prepend (parent, modifications);
          context.tdb2.modify (parent);
        }
      }
    }
    else
    {
      std::cout << _("Task not prepended.") << "\n";
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
  feedback_affected (count == 1 ? _("Prepended {1} task.") : _("Prepended {1} tasks."), count);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
