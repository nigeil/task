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
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <Context.h>
#include <text.h>
#include <util.h>
#include <i18n.h>
#include <main.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
void dependencyGetBlocked (const Task& task, std::vector <Task>& blocked)
{
  std::string uuid = task.get ("uuid");

  const std::vector <Task>& all = context.tdb2.pending.get_tasks ();
  std::vector <Task>::const_iterator it;
  for (it = all.begin (); it != all.end (); ++it)
    if ((it->getStatus () == Task::pending  ||
         it->getStatus () == Task::waiting) &&
        it->has ("depends")                 &&
        it->get ("depends").find (uuid) != std::string::npos)
      blocked.push_back (*it);
}

////////////////////////////////////////////////////////////////////////////////
void dependencyGetBlocking (const Task& task, std::vector <Task>& blocking)
{
  std::string depends = task.get ("depends");
  if (depends != "")
  {
    const std::vector <Task>& all = context.tdb2.pending.get_tasks ();
    std::vector <Task>::const_iterator it;
    for (it = all.begin (); it != all.end (); ++it)
      if ((it->getStatus () == Task::pending  ||
           it->getStatus () == Task::waiting) &&
          depends.find (it->get ("uuid")) != std::string::npos)
        blocking.push_back (*it);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Returns true if the supplied task adds a cycle to the dependency chain.
bool dependencyIsCircular (const Task& task)
{
  std::stack <Task> s;
  std::vector <std::string> deps_current;

  std::string task_uuid = task.get ("uuid");

  s.push (task);
  while (!s.empty ())
  {
    Task& current = s.top ();
    current.getDependencies (deps_current);

    // This is a basic depth first search that always terminates given the
    // assumption that any cycles in the dependency graph must have been
    // introduced by the task that is being checked.
    // Since any previous cycles would have been prevented by this very
    // function, this is a reasonable assumption.
    for (unsigned int i = 0; i < deps_current.size (); i++)
    {
      context.tdb2.get (deps_current[i], current);

      if (task_uuid == current.get ("uuid"))
        {
          // Cycle found, initial task reached for the second time!
          return true;
        }

      s.push (current);
    }
    s.pop ();
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
// Determine whether a dependency chain is being broken, assuming that 'task' is
// either completed or deleted.
//
//   blocked task blocking action
//   ------- ---- -------- -----------------------------
//           [1]  2        Chain broken
//                         Nag message generated
//                         Repair offered:  1 dep:-2
//
//           [1]  2        Chain broken
//                3        Nag message generated
//                         Repair offered:  1 dep:-2,-3
//
//   1       [2]           -
//
//   1,3     [2]           -
//
//   1       [2]  3        Chain broken
//                         Nag message generated
//                         Repair offered:  2 dep:-3
//                                          1 dep:-2,3
//
//   1,4     [2]  3,5      Chain broken
//                         Nag message generated
//                         Repair offered:  2 dep:-3,-5
//                                          1 dep:3,5
//                                          4 dep:3,5
//
void dependencyChainOnComplete (Task& task)
{
  std::vector <Task> blocking;
  dependencyGetBlocking (task, blocking);

  // If the task is anything but the tail end of a dependency chain.
  if (blocking.size ())
  {
    std::vector <Task> blocked;
    dependencyGetBlocked (task, blocked);

    // Nag about broken chain.
    if (context.config.getBoolean ("dependency.reminder"))
    {
      std::cout << format (_("Task {1} is blocked by:"), task.id)
                << "\n";

      std::vector <Task>::iterator b;
      for (b = blocking.begin (); b != blocking.end (); ++b)
        std::cout << "  " << b->id << " " << b->get ("description") << "\n";
    }

    // If there are both blocking and blocked tasks, the chain is broken.
    if (blocked.size ())
    {
      if (context.config.getBoolean ("dependency.reminder"))
      {
        std::cout << _("and is blocking:")
                  << "\n";

        std::vector <Task>::iterator b;
        for (b = blocked.begin (); b != blocked.end (); ++b)
          std::cout << "  " << b->id << " " << b->get ("description") << "\n";
      }

      if (!context.config.getBoolean ("dependency.confirmation") ||
          confirm (_("Would you like the dependency chain fixed?")))
      {
        // Repair the chain - everything in blocked should now depend on
        // everything in blocking, instead of task.id.
        std::vector <Task>::iterator left;
        std::vector <Task>::iterator right;
        for (left = blocked.begin (); left != blocked.end (); ++left)
        {
          left->removeDependency (task.id);

          for (right = blocking.begin (); right != blocking.end (); ++right)
            left->addDependency (right->id);
        }

        // Now update TDB2, now that the updates have all occurred.
        for (left = blocked.begin (); left != blocked.end (); ++left)
          context.tdb2.modify (*left);

        for (right = blocking.begin (); right != blocking.end (); ++right)
          context.tdb2.modify (*right);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void dependencyChainOnStart (Task& task)
{
  if (context.config.getBoolean ("dependency.reminder"))
  {
    std::vector <Task> blocking;
    dependencyGetBlocking (task, blocking);

    // If the task is anything but the tail end of a dependency chain, nag about
    // broken chain.
    if (blocking.size ())
    {
      std::cout << format (_("Task {1} is blocked by:"), task.id)
                << "\n";

      std::vector <Task>::iterator b;
      for (b = blocking.begin (); b != blocking.end (); ++b)
        std::cout << "  " << b->id << " " << b->get ("description") << "\n";
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Iff a dependency is being removed, is there something to do.
void dependencyChainOnModify (Task& before, Task& after)
{
  // TODO It is not clear that this should even happen.  TBD.
/*
  // Get the dependencies from before.
  std::string depends = before.get ("depends");
  std::vector <std::string> before_depends;
  split (before_depends, depends, ',');
  std::cout << "# dependencyChainOnModify before has " << before_depends.size () << "\n";

  // Get the dependencies from after.
  depends = after.get ("depends");
  std::vector <std::string> after_depends;
  split (after_depends, depends, ',');
  std::cout << "# dependencyChainOnModify after has " << after_depends.size () << "\n";

  // listDiff
  std::vector <std::string> before_only;
  std::vector <std::string> after_only;
  listDiff (before_depends, after_depends, before_only, after_only);

  // Any dependencies in before_only indicates that a dependency was removed.
  if (before_only.size ())
  {
    std::cout << "# dependencyChainOnModify detected a dependency removal\n";

    // before   dep:2,3
    // after    dep:2
    //
    // any tasks blocked by after might should be repaired to depend on 3.

    std::vector <Task> blocked;
    dependencyGetBlocked (after, blocked);

    std::vector <Task>::iterator b;
    for (b = blocked.begin (); b != blocked.end (); ++b)
    {
      std::cout << "# dependencyChainOnModify\n";
    }
  }
*/
}

////////////////////////////////////////////////////////////////////////////////
