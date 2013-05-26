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
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <inttypes.h>
#include <Context.h>
#include <Duration.h>
#include <main.h>
#include <text.h>
#include <util.h>
#include <i18n.h>
#include <cmake.h>

extern Context context;

static void countTasks (const std::vector <Task>&, const std::string&, int&, int&);

////////////////////////////////////////////////////////////////////////////////
bool taskDiff (const Task& before, const Task& after)
{
  // Attributes are all there is, so figure the different attribute names
  // between before and after.
  std::vector <std::string> beforeAtts;
  Task::const_iterator att;
  for (att = before.begin (); att != before.end (); ++att)
    beforeAtts.push_back (att->first);

  std::vector <std::string> afterAtts;
  for (att = after.begin (); att != after.end (); ++att)
    afterAtts.push_back (att->first);

  std::vector <std::string> beforeOnly;
  std::vector <std::string> afterOnly;
  listDiff (beforeAtts, afterAtts, beforeOnly, afterOnly);

  if (beforeOnly.size () !=
      afterOnly.size ())
    return true;

  std::vector <std::string>::iterator name;
  for (name = beforeAtts.begin (); name != beforeAtts.end (); ++name)
    if (*name              != "uuid" &&
        before.get (*name) != after.get (*name))
      return true;

  return false;
}

////////////////////////////////////////////////////////////////////////////////
std::string taskDifferences (const Task& before, const Task& after)
{
  // Attributes are all there is, so figure the different attribute names
  // between before and after.
  std::vector <std::string> beforeAtts;
  Task::const_iterator att;
  for (att = before.begin (); att != before.end (); ++att)
    beforeAtts.push_back (att->first);

  std::vector <std::string> afterAtts;
  for (att = after.begin (); att != after.end (); ++att)
    afterAtts.push_back (att->first);

  std::vector <std::string> beforeOnly;
  std::vector <std::string> afterOnly;
  listDiff (beforeAtts, afterAtts, beforeOnly, afterOnly);

  // Now start generating a description of the differences.
  std::stringstream out;
  std::vector <std::string>::iterator name;
  for (name = beforeOnly.begin (); name != beforeOnly.end (); ++name)
    out << "  - "
        << format (_("{1} will be deleted."), ucFirst (*name))
        << "\n";

  for (name = afterOnly.begin (); name != afterOnly.end (); ++name)
  {
    if (*name == "depends")
    {
      std::vector <int> deps_after;
      after.getDependencies (deps_after);
      std::string to;
      join (to, ", ", deps_after);

      out << "  - "
          << format (_("Dependencies will be set to '{1}'."), to)
          << "\n";
    }
    else
      out << "  - "
          << format (_("{1} will be set to '{2}'."),
                     ucFirst (*name),
                     renderAttribute (*name, after.get (*name)))
          << "\n";
  }

  for (name = beforeAtts.begin (); name != beforeAtts.end (); ++name)
  {
    // Ignore UUID differences, and find values that changed, but are not also
    // in the beforeOnly and afterOnly lists, which have been handled above..
    if (*name              != "uuid" &&
        before.get (*name) != after.get (*name) &&
        std::find (beforeOnly.begin (), beforeOnly.end (), *name) == beforeOnly.end () &&
        std::find (afterOnly.begin (),  afterOnly.end (),  *name) == afterOnly.end ())
    {
      if (*name == "depends")
      {
        std::vector <int> deps_before;
        before.getDependencies (deps_before);
        std::string from;
        join (from, ", ", deps_before);

        std::vector <int> deps_after;
        after.getDependencies (deps_after);
        std::string to;
        join (to, ", ", deps_after);

        out << "  - "
            << format (_("Dependencies will be changed from '{1}' to '{2}'."), from, to)
            << "\n";
      }
      else
        out << "  - "
            << format (_("{1} will be changed from '{2}' to '{3}'."),
                       ucFirst (*name),
                       renderAttribute (*name, before.get (*name)),
                       renderAttribute (*name, after.get (*name)))
            << "\n";
    }
  }

  // Shouldn't just say nothing.
  if (out.str ().length () == 0)
    out << "  - "
        << _("No changes will be made.")
        << "\n";

  return out.str ();
}

////////////////////////////////////////////////////////////////////////////////
std::string taskInfoDifferences (const Task& before, const Task& after, const std::string& dateformat, long& last_timestamp, const long current_timestamp)
{
  // Attributes are all there is, so figure the different attribute names
  // between before and after.
  std::vector <std::string> beforeAtts;
  Task::const_iterator att;
  for (att = before.begin (); att != before.end (); ++att)
    beforeAtts.push_back (att->first);

  std::vector <std::string> afterAtts;
  for (att = after.begin (); att != after.end (); ++att)
    afterAtts.push_back (att->first);

  std::vector <std::string> beforeOnly;
  std::vector <std::string> afterOnly;
  listDiff (beforeAtts, afterAtts, beforeOnly, afterOnly);

  // Now start generating a description of the differences.
  std::stringstream out;
  std::vector <std::string>::iterator name;
  for (name = beforeOnly.begin (); name != beforeOnly.end (); ++name)
  {
    if (*name == "depends")
    {
        std::vector <int> deps_before;
        before.getDependencies (deps_before);
        std::string from;
        join (from, ", ", deps_before);

        out << format (_("Dependencies '{1}' deleted."), from)
            << "\n";
    }
    else if (name->substr (0, 11) == "annotation_")
    {
      out << format (_("Annotation '{1}' deleted."), before.get (*name))
          << "\n";
    }
    else if (*name == "start")
    {
      out << format (_("{1} deleted (duration: {2})."), ucFirst (*name),
                     Duration(current_timestamp - last_timestamp).formatPrecise())
          << "\n";
    }
    else
    {
      out << format (_("{1} deleted."), ucFirst (*name))
          << "\n";
    }
  }

  for (name = afterOnly.begin (); name != afterOnly.end (); ++name)
  {
    if (*name == "depends")
    {
      std::vector <int> deps_after;
      after.getDependencies (deps_after);
      std::string to;
      join (to, ", ", deps_after);

      out << format (_("Dependencies set to '{1}'."), to)
          << "\n";
    }
    else if (name->substr (0, 11) == "annotation_")
    {
      out << format (_("Annotation of '{1}' added."), after.get (*name))
          << "\n";
    }
    else
    {
      if (*name == "start")
          last_timestamp = current_timestamp;

      out << format (_("{1} set to '{2}'."),
                     ucFirst (*name),
                     renderAttribute (*name, after.get (*name), dateformat))
          << "\n";
    }
  }

  for (name = beforeAtts.begin (); name != beforeAtts.end (); ++name)
    if (*name              != "uuid" &&
        before.get (*name) != after.get (*name) &&
        before.get (*name) != "" && after.get (*name) != "")
    {
      if (*name == "depends")
      {
        std::vector <int> deps_before;
        before.getDependencies (deps_before);
        std::string from;
        join (from, ", ", deps_before);

        std::vector <int> deps_after;
        after.getDependencies (deps_after);
        std::string to;
        join (to, ", ", deps_after);

        out << format (_("Dependencies changed from '{1}' to '{2}'."), from, to)
            << "\n";
      }
      else if (name->substr (0, 11) == "annotation_")
      {
        out << format (_("Annotation changed to '{1}'."), after.get (*name))
            << "\n";
      }
      else
        out << format (_("{1} changed from '{2}' to '{3}'."),
                       ucFirst (*name),
                       renderAttribute (*name, before.get (*name), dateformat),
                       renderAttribute (*name, after.get (*name), dateformat))
            << "\n";
    }

  // Shouldn't just say nothing.
  if (out.str ().length () == 0)
    out << _("No changes made.")
        << "\n";

  return out.str ();
}

////////////////////////////////////////////////////////////////////////////////
std::string renderAttribute (const std::string& name, const std::string& value, const std::string& format /* = "" */)
{
  Column* col = context.columns[name];
  if (col                    &&
      col->type () == "date" &&
      value != "")
  {
    Date d ((time_t)strtol (value.c_str (), NULL, 10));
    if (format == "")
    {
      return d.toString (context.config.get ("dateformat"));
    }
    else
    {
      return d.toString (format);
    }
  }

  return value;
}

////////////////////////////////////////////////////////////////////////////////
// Implements:
//    <string>
void feedback_affected (const std::string& effect)
{
  if (context.verbose ("affected") ||
      context.config.getBoolean ("echo.command")) // Deprecated 2.0
  {
    std::cout << effect << "\n";
  }
}

////////////////////////////////////////////////////////////////////////////////
// Implements:
//    Deleted 3 tasks
//
// The 'effect' string should contain:
//    {1}    Quantity
void feedback_affected (const std::string& effect, int quantity)
{
  if (context.verbose ("affected") ||
      context.config.getBoolean ("echo.command")) // Deprecated 2.0
  {
    std::cout << format (effect, quantity)
              << "\n";
  }
}

////////////////////////////////////////////////////////////////////////////////
// Implements:
//    Deleting task 123 'This is a test'
//
// The 'effect' string should contain:
//    {1}    ID
//    {2}    Description
void feedback_affected (const std::string& effect, const Task& task)
{
  if (context.verbose ("affected") ||
      context.config.getBoolean ("echo.command")) // Deprecated 2.0
  {
    if (task.id)
      std::cout << format (effect, task.id, task.get ("description"))
                << "\n";
    else
      std::cout << format (effect, task.get ("uuid"), task.get ("description"))
                << "\n";
  }
}

////////////////////////////////////////////////////////////////////////////////
// Implements feedback when adding special tags to a task.
void feedback_special_tags (const Task& task, const std::string& tag)
{
  if (context.verbose ("special"))
  {
    std::string msg;
    std::string explanation;
         if (tag == "nocolor") msg = _("The 'nocolor' special tag will disable color rules for this task.");
    else if (tag == "nonag")   msg = _("The 'nonag' special tag will prevent nagging when this task is modified.");
    else if (tag == "nocal")   msg = _("The 'nocal' special tag will keep this task off the 'calendar' report.");
    else if (tag == "next")    msg = _("The 'next' special tag will boost the urgency of this task so it appears on the 'next' report.");

    if (msg.length ())
    {
      if (task.id)
        std::cout << format (msg, task.id)
                  << "\n";
      else
        std::cout << format (msg, task.get ("uuid"))
                  << "\n";
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Called on completion, deletion and update.  If this task is blocking another
// task, then if this was the *only* blocking task, that other task is now
// unblocked.  Mention it.
//
// Implements:
//    Unblocked <id> '<description>'
void feedback_unblocked (const Task& task)
{
  if (context.verbose ("affected") ||
      context.config.getBoolean ("echo.command")) // Deprecated 2.0
  {
    // Get a list of tasks that depended on this task.
    std::vector <Task> blocked;
    dependencyGetBlocked (task, blocked);

    // Scan all the tasks that were blocked by this task
    std::vector <Task>::iterator i;
    for (i = blocked.begin (); i != blocked.end (); ++i)
    {
      std::vector <Task> blocking;
      dependencyGetBlocking (*i, blocking);
      if (blocking.size () == 0)
      {
        if (i->id)
          std::cout << format (_("Unblocked {1} '{2}'."),
                               i->id,
                               i->get ("description"))
                    << "\n";
        else
        {
          std::string uuid = i->get ("uuid");
          std::cout << format (_("Unblocked {1} '{2}'."),
                               i->get ("uuid"),
                               i->get ("description"))
                    << "\n";
        }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
void feedback_backlog ()
{
  if (context.config.get ("taskd.server") != "" &&
      context.verbose ("sync"))
  {
    std::vector <std::string> lines = context.tdb2.backlog.get_lines ();
    std::vector <std::string>::iterator line;
    for (line = lines.begin (); line != lines.end (); ++line)
    {
      if ((*line)[0] == '[')
      {
        context.footnote ("There are local changes.  Sync required.");
        break;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
std::string onProjectChange (Task& task, bool scope /* = true */)
{
  std::stringstream msg;
  std::string project = task.get ("project");

  if (project != "")
  {
    if (scope)
      msg << format (_("The project '{1}' has changed."), project)
          << "  ";

    // Count pending and done tasks, for this project.
    int count_pending = 0;
    int count_done = 0;
    std::vector <Task> all = context.tdb2.all_tasks ();
    countTasks (all, project, count_pending, count_done);

    // count_done  count_pending  percentage
    // ----------  -------------  ----------
    //          0              0          0%
    //         >0              0        100%
    //          0             >0          0%
    //         >0             >0  calculated
    int percentage = 0;
    if (count_done == 0)
      percentage = 0;
    else if (count_pending == 0)
      percentage = 100;
    else
      percentage = (count_done * 100 / (count_done + count_pending));

    msg << format (_("Project '{1}' is {2}% complete"), project, percentage)
        << " "
        << format (_("({1} of {2} tasks remaining)."), count_pending, count_pending + count_done);
  }

  return msg.str ();
}

///////////////////////////////////////////////////////////////////////////////
std::string onProjectChange (Task& task1, Task& task2)
{
  if (task1.get ("project") == task2.get ("project"))
    return onProjectChange (task1, false);

  std::string messages1 = onProjectChange (task1);
  std::string messages2 = onProjectChange (task2);

  if (messages1.length () && messages2.length ())
    return messages1 + '\n' + messages2;

  return messages1 + messages2;
}

///////////////////////////////////////////////////////////////////////////////
std::string onExpiration (Task& task)
{
  std::stringstream msg;

  if (context.verbose ("affected"))
    msg << format (_("Task {1} '{2}' expired and was deleted."), task.id, task.get ("description"));

  return msg.str ();
}

///////////////////////////////////////////////////////////////////////////////
static void countTasks (
  const std::vector <Task>& all,
  const std::string& project,
  int& count_pending,
  int& count_done)
{
  std::vector <Task>::const_iterator it;
  for (it = all.begin (); it != all.end (); ++it)
  {
    if (it->get ("project") == project)
    {
      switch (it->getStatus ())
      {
      case Task::pending:
      case Task::waiting:
        ++count_pending;
        break;

      case Task::completed:
        ++count_done;
        break;

      case Task::deleted:
      case Task::recurring:
      default:
        break;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
