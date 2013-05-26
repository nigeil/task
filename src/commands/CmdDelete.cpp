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
#include <CmdDelete.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdDelete::CmdDelete ()
{
  _keyword       = "delete";
  _usage         = "task <filter> delete <mods>";
  _description   = _("Deletes the specified task");
  _read_only     = false;
  _displays_id   = false;
  _needs_confirm = true;
}

////////////////////////////////////////////////////////////////////////////////
int CmdDelete::execute (std::string& output)
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
    Task before (*task);

    if (task->getStatus () == Task::pending   ||
        task->getStatus () == Task::completed ||
        task->getStatus () == Task::waiting)
    {
      // Delete the specified task.
      std::string question;
      if (task->id)
        // TRANSLATORS: {1} is the ID, {2} is the description.
        question = format (_("Permanently delete task {1} '{2}'?"),
                           task->id,
                           task->get ("description"));
      else
        question = format (_("Permanently delete task {1} '{2}'?"),
                           task->get ("uuid"),
                           task->get ("description"));

      modify_task_annotate (*task, modifications);
      task->setStatus (Task::deleted);
      if (! task->has ("end"))
        task->setEnd ();

      if (permission (*task, question, filtered.size ()))
      {
        updateRecurrenceMask (*task);
        ++count;
        context.tdb2.modify (*task);
        // TRANSLATORS: {1} is the ID, {2} is the description.
        feedback_affected (_("Deleting task {1} '{2}'."), *task);
        feedback_unblocked (*task);
        dependencyChainOnComplete (*task);
        if (context.verbose ("project"))
          projectChanges[task->get ("project")] = onProjectChange (*task);

        // Delete siblings.
        if (task->has ("parent"))
        {
          std::vector <Task> siblings = context.tdb2.siblings (*task);
          if (siblings.size () &&
              confirm (_("This is a recurring task.  Do you want to delete all pending recurrences of this same task?")))
          {
            std::vector <Task>::iterator sibling;
            for (sibling = siblings.begin (); sibling != siblings.end (); ++sibling)
            {
              modify_task_annotate (*sibling, modifications);
              sibling->setStatus (Task::deleted);
              if (! sibling->has ("end"))
                sibling->setEnd ();

              updateRecurrenceMask (*sibling);
              context.tdb2.modify (*sibling);
              feedback_affected (_("Deleting recurring task {1} '{2}'."), *sibling);
              feedback_unblocked (*sibling);
              ++count;
            }

            // Delete the parent
            Task parent;
            context.tdb2.get (task->get ("parent"), parent);
            parent.setStatus (Task::deleted);
            if (! parent.has ("end"))
              parent.setEnd ();

            context.tdb2.modify (parent);
          }
        }
      }
      else
      {
        std::cout << _("Task not deleted.") << "\n";
        rc = 1;
        if (_permission_quit)
          break;
      }
    }
    else
    {
      // TRANSLATORS: {1} is the ID, {2} is the description.
      std::cout << format (_("Task {1} '{2}' is not deletable."),
                           task->id,
                           task->get ("description"))
          << "\n";
      rc = 1;
    }
  }

  // Now list the project changes.
  std::map <std::string, std::string>::iterator i;
  for (i = projectChanges.begin (); i != projectChanges.end (); ++i)
    if (i->first != "")
      context.footnote (i->second);

  context.tdb2.commit ();
  feedback_affected (ngettext("Deleted {1} task.", "Deleted {1} tasks.", count), count);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
