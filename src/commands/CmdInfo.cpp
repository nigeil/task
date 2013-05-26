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
#include <Date.h>
#include <Duration.h>
#include <ViewText.h>
#include <main.h>
#include <text.h>
#include <i18n.h>
#include <CmdInfo.h>

extern Context context;
////////////////////////////////////////////////////////////////////////////////
CmdInfo::CmdInfo ()
{
  _keyword     = "information";
  _usage       = "task <filter> information";
  _description = _("Shows all data and metadata");
  _read_only   = true;

  // This is inaccurate, but it does prevent a GC.  While this doesn't make a
  // lot of sense, given that the info command shows the ID, it does mimic the
  // behavior of versions prior to 2.0, which the test suite relies upon.
  //
  // Once the test suite is completely modified, this can be corrected.
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdInfo::execute (std::string& output)
{
  int rc = 0;

  // Apply filter.
  std::vector <Task> filtered;
  filter (filtered);

  if (! filtered.size ())
  {
    context.footnote (_("No matches."));
    rc = 1;
  }

  // Get the undo data.
  std::vector <std::string> undo;
  if (context.config.getBoolean ("journal.info"))
    undo = context.tdb2.undo.get_lines ();

  // Determine the output date format, which uses a hierarchy of definitions.
  //   rc.dateformat.info
  //   rc.dateformat
  std::string dateformat = context.config.get ("dateformat.info");
  if (dateformat == "")
    dateformat = context.config.get ("dateformat");

  std::string dateformatanno = context.config.get ("dateformat.annotation");
  if (dateformatanno == "")
    dateformatanno = dateformat;

  // Render each task.
  std::stringstream out;
  std::vector <Task>::iterator task;
  for (task = filtered.begin (); task != filtered.end (); ++task)
  {
    ViewText view;
    view.width (context.getWidth ());
    view.add (Column::factory ("string", sgettext("column|Name")));
    view.add (Column::factory ("string", sgettext("column|Value")));

    // If an alternating row color is specified, notify the table.
    if (context.color ())
    {
      Color alternate (context.config.get ("color.alternate"));
      view.colorOdd (alternate);
      view.intraColorOdd (alternate);
    }

    Date now;

    // id
    int row = view.addRow ();
    view.set (row, 0, sgettext("column|ID"));
    view.set (row, 1, (task->id ? format (task->id) : "-"));

    std::string status = ucFirst (Task::statusToText (task->getStatus ()));

    // description
    Color c;
    autoColorize (*task, c);
    std::string description = task->get ("description");
    int indent = context.config.getInteger ("indent.annotation");

    std::map <std::string, std::string> annotations;
    task->getAnnotations (annotations);
    std::map <std::string, std::string>::iterator ann;
    for (ann = annotations.begin (); ann != annotations.end (); ++ann)
      description += "\n"
                   + std::string (indent, ' ')
                   + Date (ann->first.substr (11)).toString (dateformatanno)
                   + " "
                   + ann->second;

    row = view.addRow ();
    view.set (row, 0, sgettext("column|Description"));
    view.set (row, 1, description, c);

    // status
    row = view.addRow ();
    view.set (row, 0, sgettext("column|Status"));
    view.set (row, 1, status);

    // project
    if (task->has ("project"))
    {
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Project"));
      view.set (row, 1, task->get ("project"));
    }

    // priority
    if (task->has ("priority"))
    {
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Priority"));
      view.set (row, 1, task->get ("priority"));
    }

    // dependencies: blocked
    {
      std::vector <Task> blocked;
      dependencyGetBlocking (*task, blocked);
      if (blocked.size ())
      {
        std::stringstream message;
        std::vector <Task>::const_iterator it;
        for (it = blocked.begin (); it != blocked.end (); ++it)
          message << it->id << " " << it->get ("description") << "\n";

        row = view.addRow ();
        view.set (row, 0, STRING_CMD_INFO_BLOCKED);
        view.set (row, 1, message.str ());
      }
    }

    // dependencies: blocking
    {
      std::vector <Task> blocking;
      dependencyGetBlocked (*task, blocking);
      if (blocking.size ())
      {
        std::stringstream message;
        std::vector <Task>::const_iterator it;
        for (it = blocking.begin (); it != blocking.end (); ++it)
          message << it->id << " " << it->get ("description") << "\n";

        row = view.addRow ();
        view.set (row, 0, STRING_CMD_INFO_BLOCKING);
        view.set (row, 1, message.str ());
      }
    }

    // recur
    if (task->has ("recur"))
    {
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Recurrence"));
      view.set (row, 1, task->get ("recur"));
    }

    // until
    if (task->has ("until"))
    {
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Until"));
      view.set (row, 1, Date (task->get_date ("until")).toString (dateformat));
    }

    // mask
    if (task->getStatus () == Task::recurring)
    {
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Mask"));
      view.set (row, 1, task->get ("mask"));
    }

    if (task->has ("parent"))
    {
      // parent
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Parent task"));
      view.set (row, 1, task->get ("parent"));

      // imask
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Mask Index"));
      view.set (row, 1, task->get ("imask"));
    }

    // due (colored)
    if (task->has ("due"))
    {
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Due"));
      view.set (row, 1, Date (task->get_date ("due")).toString (dateformat));
    }

    // wait
    if (task->has ("wait"))
    {
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Waiting until"));
      view.set (row, 1, Date (task->get_date ("wait")).toString (dateformat));
    }

    // scheduled
    if (task->has ("scheduled"))
    {
      row = view.addRow ();
      view.set (row, 0, STRING_COLUMN_LABEL_SCHED);
      view.set (row, 1, Date (task->get_date ("scheduled")).toString (dateformat));
    }

    // start
    if (task->has ("start"))
    {
      row = view.addRow ();
      view.set (row, 0, sgettext("column|Start"));
      view.set (row, 1, Date (task->get_date ("start")).toString (dateformat));
    }

    // end
    if (task->has ("end"))
    {
      row = view.addRow ();
      view.set (row, 0, sgettext("column|End"));
      view.set (row, 1, Date (task->get_date ("end")).toString (dateformat));
    }

    // tags ...
    std::vector <std::string> tags;
    task->getTags (tags);
    if (tags.size ())
    {
      std::string allTags;
      join (allTags, " ", tags);

      row = view.addRow ();
      view.set (row, 0, STRING_COLUMN_LABEL_TAGS);
      view.set (row, 1, allTags);
    }

    // uuid
    row = view.addRow ();
    view.set (row, 0, STRING_COLUMN_LABEL_UUID);
    std::string uuid = task->get ("uuid");
    view.set (row, 1, uuid);

    // entry
    row = view.addRow ();
    view.set (row, 0, STRING_COLUMN_LABEL_ENTERED);
    Date dt (task->get_date ("entry"));
    std::string entry = dt.toString (dateformat);

    std::string age;
    std::string created = task->get ("entry");
    if (created.length ())
    {
      Date dt (strtol (created.c_str (), NULL, 10));
      age = Duration (now - dt).format ();
    }

    view.set (row, 1, entry + " (" + age + ")");

    // Task::urgency
    row = view.addRow ();
    view.set (row, 0, STRING_COLUMN_LABEL_URGENCY);
    view.set (row, 1, trimLeft (format (task->urgency (), 4, 4)));

    // modified
    if (task->has ("modified"))
    {
      row = view.addRow ();
      view.set (row, 0, STRING_CMD_INFO_MODIFIED);

      Date mod (task->get_date ("modified"));

      std::string age = Duration (now - mod).format ();
      view.set (row, 1, Date (task->get_date ("modified")).toString (dateformat) +
                              " (" + age + ")");
    }

    // Show any UDAs
    std::vector <std::string> all = task->all ();
    std::vector <std::string>::iterator att;
    std::string type;
    for (att = all.begin (); att != all.end (); ++att)
    {
      type = context.config.get ("uda." + *att + ".type");
      if (type != "")
      {
        Column* col = context.columns[*att];
        if (col)
        {
          std::string value = task->get (*att);
          if (value != "")
          {
            row = view.addRow ();
            view.set (row, 0, col->label ());

            if (type == "date")
              value = Date (value).toString (dateformat);
            else if (type == "duration")
              value = Duration (value).formatCompact ();

            view.set (row, 1, value);
          }
        }
      }
    }

    // Show any orphaned UDAs, which are identified by not being represented in
    // the context.columns map.
    for (att = all.begin (); att != all.end (); ++att)
      if (att->substr (0, 11) != "annotation_" &&
          context.columns.find (*att) == context.columns.end ())
      {
         row = view.addRow ();
         view.set (row, 0, "[" + *att);
         view.set (row, 1, task->get (*att) + "]");
      }

    // Create a second table, containing undo log change details.
    ViewText journal;

    // If an alternating row color is specified, notify the table.
    if (context.color ())
    {
      Color alternate (context.config.get ("color.alternate"));
      journal.colorOdd (alternate);
      journal.intraColorOdd (alternate);
    }

    journal.width (context.getWidth ());
    journal.add (Column::factory ("string", STRING_COLUMN_LABEL_DATE));
    journal.add (Column::factory ("string", STRING_CMD_INFO_MODIFICATION));

    if (context.config.getBoolean ("journal.info") &&
        undo.size () > 3)
    {
      // Scan the undo data for entries matching this task.
      std::string when;
      std::string previous;
      std::string current;
      unsigned int i = 0;
      long total_time = 0;
      long last_timestamp = 0;
      while (i < undo.size ())
      {
        when = undo[i++];
        previous = "";
        if (undo[i].substr (0, 3) == "old")
          previous = undo[i++];

        current = undo[i++];
        i++; // Separator

        if (current.find ("uuid:\"" + uuid) != std::string::npos)
        {
          if (previous != "")
          {
            int row = journal.addRow ();

            Date timestamp (strtol (when.substr (5).c_str (), NULL, 10));
            journal.set (row, 0, timestamp.toString (dateformat));

            Task before (previous.substr (4));
            Task after (current.substr (4));
            journal.set (row, 1, taskInfoDifferences (before, after, dateformat, last_timestamp, timestamp.toEpoch()));

            // calculate the total active time
            if (before.get ("start") == ""
              && after.get ("start") != "")
            {
              // task started
              total_time -= timestamp.toEpoch ();
            }
            else if (((before.get ("start") != "" &&
                       after.get ("start") == "") ||
                      (before.get ("status") != "completed" &&
                       after.get ("status") == "completed")) &&
                     total_time < 0)
            {
              // task stopped or done
              total_time += timestamp.toEpoch ();
            }
          }
        }
      }

      // add now() if task is still active
      if (total_time < 0)
        total_time += Date ().toEpoch ();

      // print total active time
      if (total_time > 0)
      {
        row = journal.addRow ();
        journal.set (row, 0, STRING_CMD_INFO_TOTAL_ACTIVE);
        journal.set (row, 1, Duration (total_time).formatPrecise (),
                     (context.color () ? Color ("bold") : Color ()));
      }
    }

    out << optionalBlankLine ()
        << view.render ()
        << "\n";

    if (journal.rows () > 0)
      out << journal.render ()
          << "\n";
  }

  output = out.str ();
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
