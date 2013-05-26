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
#include <Context.h>
#include <main.h>
#include <text.h>
#include <util.h>
#include <i18n.h>
#include <CmdModify.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdModify::CmdModify ()
{
  _keyword     = "modify";
  _usage       = "task <filter> modify <mods>";
  _description = _("Modifies the existing task with provided arguments.");
  _read_only   = false;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdModify::execute (std::string& output)
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
    modify_task_description_replace (*task, modifications);
    if (taskDiff (before, *task))
    {
      // Perform some logical consistency checks.
      if (task->has ("recur")  &&
          !task->has ("due")   &&
          !before.has ("due"))
        throw std::string (_("You cannot specify a recurring task without a due date."));

      if (before.has ("recur") &&
          before.has ("due")   &&
          (!task->has ("due")  ||
           task->get ("due") == ""))
        throw std::string (_("You cannot remove the due date from a recurring task."));

      if (before.has ("recur")  &&
          (!task->has ("recur") ||
           task->get ("recur") == ""))
        throw std::string (_("You cannot remove the recurrence from a recurring task."));

      // Delete the specified task.
      std::string question;
      if (task->id != 0)
        question = format (_("Modify task {1} '{2}'?"),
                           task->id,
                           task->get ("description"));
      else
        question = format (_("Modify task {1} '{2}'?"),
                           task->get ("uuid"),
                           task->get ("description"));

      if (permission (*task, taskDifferences (before, *task) + question, filtered.size ()))
      {
        updateRecurrenceMask (*task);
        dependencyChainOnModify (before, *task);
        ++count;
        feedback_affected (_("Modifying task {1} '{2}'."), *task);
        feedback_unblocked (*task);
        context.tdb2.modify (*task);
        if (context.verbose ("project"))
          projectChanges[task->get ("project")] = onProjectChange (before, *task);

        // Task potentially has siblings - modify them.
        if (task->has ("parent"))
        {
          std::vector <Task> siblings = context.tdb2.siblings (*task);
          if (siblings.size () &&
              confirm (_("This is a recurring task.  Do you want to modify all pending recurrences of this same task?")))
          {
            std::vector <Task>::iterator sibling;
            for (sibling = siblings.begin (); sibling != siblings.end (); ++sibling)
            {
              Task alternate (*sibling);
              modify_task_description_replace (*sibling, modifications);
              updateRecurrenceMask (*sibling);
              dependencyChainOnModify (alternate, *sibling);
              ++count;
              feedback_affected (_("Modifying recurring task {1} '{2}'."), *sibling);
              feedback_unblocked (*sibling);
              context.tdb2.modify (*sibling);
              if (context.verbose ("project"))
                projectChanges[sibling->get ("project")] = onProjectChange (alternate, *sibling);
            }
          }
        }

        // Task potentially has child tasks - modify them.
        else if (task->get ("status") == "recurring")
        {
          std::vector <Task> children = context.tdb2.children (*task);
          if (children.size () &&
              confirm (_("This is a recurring task.  Do you want to modify all pending recurrences of this same task?")))
          {
            std::vector <Task>::iterator child;
            for (child = children.begin (); child != children.end (); ++child)
            {
              Task alternate (*child);
              modify_task_description_replace (*child, modifications);
              updateRecurrenceMask (*child);
              context.tdb2.modify (*child);
              dependencyChainOnModify (alternate, *child);
              if (context.verbose ("project"))
                projectChanges[child->get ("project")] = onProjectChange (alternate, *child);
              ++count;
              feedback_affected (_("Modifying recurring task {1} '{2}'."), *child);
            }
          }
        }
      }
      else
      {
        std::cout << _("Task not modified.") << "\n";
        rc = 1;
        if (_permission_quit)
          break;
      }
    }
  }

  // Now list the project changes.
  std::map <std::string, std::string>::iterator i;
  for (i = projectChanges.begin (); i != projectChanges.end (); ++i)
    if (i->first != "")
      context.footnote (i->second);

  context.tdb2.commit ();
  feedback_affected (count == 1 ? _("Modified {1} task.") : _("Modified {1} tasks."), count);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
