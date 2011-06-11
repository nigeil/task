////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2006 - 2011, Paul Beckingham, Federico Hernandez.
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the
//
//     Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor,
//     Boston, MA
//     02110-1301
//     USA
//
////////////////////////////////////////////////////////////////////////////////

#include <Context.h>
#include <Expression.h>
#include <main.h>
#include <CmdQuery.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdQuery::CmdQuery ()
{
  _keyword     = "_query";
  _usage       = "task _query [<filter>]";
  _description = "Executes external commands and scripts";
  _read_only   = true;
  _displays_id = true;
}

////////////////////////////////////////////////////////////////////////////////
int CmdQuery::execute (std::string& output)
{
  int rc = 0;

  // Get all the tasks.
  std::vector <Task> tasks;
  context.tdb.lock (context.config.getBoolean ("locking"));
  handleRecurrence ();
  Filter filter;
  context.tdb.load (tasks, filter);
  context.tdb.commit ();
  context.tdb.unlock ();

  // Filter.
  Arguments f = context.args.extract_read_only_filter ();
  Expression e (f);

  std::vector <Task> filtered;
  std::vector <Task>::iterator task;
  for (task = tasks.begin (); task != tasks.end (); ++task)
    if (e.eval (*task))
      filtered.push_back (*task);

  if (filtered.size () == 0)
  {
    context.footnote ("No matches.");
    return 1;
  }

  // Note: "limit:" feature not supported.
  // TODO Why not?

  // Compose output.
  for (task = filtered.begin (); task != filtered.end (); ++task)
  {
    if (task != filtered.begin ())
      output += ",\n";

    output += task->composeJSON (true);
  }

  output += "\n";
  return rc;
}

////////////////////////////////////////////////////////////////////////////////