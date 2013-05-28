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
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <unistd.h>
#include <Duration.h>
#include <Context.h>
#include <i18n.h>
#include <text.h>
#include <util.h>
#include <i18n.h>
#include <main.h>
#include <CmdEdit.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdEdit::CmdEdit ()
{
  _keyword     = "edit";
  _usage       = "task <filter> edit";
  _description = _("Launches an editor to modify a task directly");
  _read_only   = false;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
// Introducing the Silver Bullet.  This feature is the catch-all fixative for
// various other ills.  This is like opening up the hood and going in with a
// wrench.  To be used sparingly.
int CmdEdit::execute (std::string& output)
{
  int rc = 0;

  // Filter the tasks.
  handleRecurrence ();
  std::vector <Task> filtered;
  filter (filtered);

  // Find number of matching tasks.  Skip recurring parent tasks.
  std::vector <Task>::iterator task;
  for (task = filtered.begin (); task != filtered.end (); ++task)
    if (editFile (*task))
      context.tdb2.modify (*task);

  context.tdb2.commit ();
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
std::string CmdEdit::findValue (
  const std::string& text,
  const std::string& name)
{
  std::string::size_type found = text.find (name);
  if (found != std::string::npos)
  {
    std::string::size_type eol = text.find ("\n", found + 1);
    if (eol != std::string::npos)
    {
      std::string value = text.substr (
        found + name.length (),
        eol - (found + name.length ()));

      return trim (value, "\t ");
    }
  }

  return "";
}

////////////////////////////////////////////////////////////////////////////////
std::vector <std::string> CmdEdit::findValues (
  const std::string& text,
  const std::string& name)
{
  std::vector <std::string> results;
  std::string::size_type found = 0;

  while (found != std::string::npos)
  {
    found = text.find (name, found + 1);
    if (found != std::string::npos)
    {
      std::string::size_type eol = text.find ("\n", found + 1);
      if (eol != std::string::npos)
      {
        std::string value = text.substr (
          found + name.length (),
          eol - (found + name.length ()));

        found = eol - 1;
        results.push_back (trim (value, "\t "));
      }
    }
  }

  return results;
}

////////////////////////////////////////////////////////////////////////////////
std::string CmdEdit::formatDate (
  Task& task,
  const std::string& attribute,
  const std::string& dateformat)
{
  std::string value = task.get (attribute);
  if (value.length ())
  {
    Date dt (value);
    value = dt.toString (dateformat);
  }

  return value;
}

////////////////////////////////////////////////////////////////////////////////
std::string CmdEdit::formatDuration (
  Task& task,
  const std::string& attribute)
{
  std::string value = task.get (attribute);
  if (value.length ())
  {
    Duration dur (value);
    value = dur.formatSeconds ();
  }

  return value;
}

////////////////////////////////////////////////////////////////////////////////
std::string CmdEdit::formatTask (Task task, const std::string& dateformat)
{
  std::stringstream before;
  bool verbose = context.verbose ("edit") ||
                 context.config.getBoolean ("edit.verbose"); // Deprecated 2.0

  if (verbose)
              // TRANSLATORS: Lines should not exceed 80 characters.
    before << _("# The 'task <id> edit' command allows you to modify all aspects of a task\n"
                "# using a text editor.  Below is a representation of all the task details.\n"
                "# Modify what you wish, and when you save and quit your editor,\n"
                "# taskwarrior will read this file, determine what changed, and apply\n"
                "# those changes.  If you exit your editor without saving or making\n"
                "# modifications, taskwarrior will do nothing.")
           << "\n#\n"
              // TRANSLATORS: Lines should not exceed 80 characters.
           << _("# Lines that begin with # represent data you cannot change, like ID.\n"
                "# If you get too creative with your editing, taskwarrior will send you\n"
                "# back to the editor to try again.")
           << "\n#\n"
              // TRANSLATORS: Lines should not exceed 80 characters.
           << _("# Should you find yourself in an endless loop, re-editing the same file,\n"
                "# just quit the editor without making any changes.  Taskwarrior will\n"
                "# notice this and stop the editing.")
           << "\n#\n";

  // TRANSLATORS: Preserve the fixed-width of the first column.
  before << _("# Name               Editable details\n" \
              "# -----------------  ----------------------------------------------------\n")
         << "# ID:                " << task.id                                          << "\n"
         << "# UUID:              " << task.get ("uuid")                                << "\n"
         << "# Status:            " << ucFirst (Task::statusToText (task.getStatus ())) << "\n"
         << "# Mask:              " << task.get ("mask")                                << "\n"
         << "# iMask:             " << task.get ("imask")                               << "\n"
         << "  Project:           " << task.get ("project")                             << "\n"
         << "  Priority:          " << task.get ("priority")                            << "\n";

  std::vector <std::string> tags;
  task.getTags (tags);
  std::string allTags;
  join (allTags, " ", tags);

  if (verbose)
    before << "# " << _("Separate the tags with spaces, like this: tag1 tag2") << "\n";

  before << "  Tags:              " << allTags                                          << "\n"
         << "  Description:       " << task.get ("description")                         << "\n"
         << "  Created:           " << formatDate (task, "entry", dateformat)           << "\n"
         << "  Started:           " << formatDate (task, "start", dateformat)           << "\n"
         << "  Ended:             " << formatDate (task, "end", dateformat)             << "\n"
         << "  Scheduled:         " << formatDate (task, "scheduled", dateformat)       << "\n"
         << "  Due:               " << formatDate (task, "due", dateformat)             << "\n"
         << "  Until:             " << formatDate (task, "until", dateformat)           << "\n"
         << "  Recur:             " << task.get ("recur")                               << "\n"
         << "  Wait until:        " << formatDate (task, "wait", dateformat)            << "\n"
         << "  Parent:            " << task.get ("parent")                              << "\n";

  if (verbose)
    before << _("# Annotations look like this: <date> -- <text> and there can be any number of them.\n" \
                "# The ' -- ' separator between the date and text field should not be removed.\n" \
                "# A \"blank slot\" for adding an annotation follows for your convenience.\n");

  std::map <std::string, std::string> annotations;
  task.getAnnotations (annotations);
  std::map <std::string, std::string>::iterator anno;
  for (anno = annotations.begin (); anno != annotations.end (); ++anno)
  {
    Date dt (strtol (anno->first.substr (11).c_str (), NULL, 10));
    before << "  Annotation:        " << dt.toString (dateformat)
           << " -- "                  << anno->second << "\n";
  }

  Date now;
  before << "  Annotation:        " << now.toString (dateformat) << " -- \n";

  // Add dependencies here.
  std::vector <std::string> dependencies;
  task.getDependencies (dependencies);
  std::stringstream allDeps;
  for (unsigned int i = 0; i < dependencies.size (); ++i)
  {
    if (i)
      allDeps << ",";

    Task t;
    context.tdb2.get (dependencies[i], t);
    if (t.getStatus () == Task::pending ||
        t.getStatus () == Task::waiting)
      allDeps << t.id;
    else
      allDeps << dependencies[i];
  }

  if (verbose)
    before << "# " << _("Dependencies should be a comma-separated list of task IDs/UUIDs or ID ranges, with no spaces.") << "\n";

  before << "  Dependencies:      " << allDeps.str () << "\n";

  // UDAs
  std::vector <std::string> udas;
  std::map <std::string, Column*>::iterator col;
  for (col = context.columns.begin (); col != context.columns.end (); ++col)
    if (context.config.get ("uda." + col->first + ".type") != "")
      udas.push_back (col->first);

  if (udas.size ())
  {
    before << "# " << _("User Defined Attributes") << "\n";
    std::sort (udas.begin (), udas.end ());
    std::vector <std::string>::iterator uda;
    for (uda = udas.begin (); uda != udas.end (); ++uda)
    {
      int pad = 13 - uda->length ();
      std::string padding = "";
      if (pad > 0)
        padding = std::string (pad, ' ');

      std::string type = context.config.get ("uda." + *uda + ".type");
      if (type == "string" || type == "numeric")    
        before << "  UDA " << *uda << ": " << padding << task.get (*uda) << "\n";
      else if (type == "date")
        before << "  UDA " << *uda << ": " << padding << formatDate (task, *uda, dateformat) << "\n";
      else if (type == "duration")
        before << "  UDA " << *uda << ": " << padding << formatDuration (task, *uda) << "\n";
    }
  }

  // UDA orphans
  std::vector <std::string> orphans;
  task.getUDAOrphans (orphans);

  if (orphans.size ())
  {
    before << "# " << _("User Defined Attribute Orphans") << "\n";
    std::sort (orphans.begin (), orphans.end ());
    std::vector <std::string>::iterator orphan;
    for (orphan = orphans.begin (); orphan != orphans.end (); ++orphan)
    {
      int pad = 6 - orphan->length ();
      std::string padding = "";
      if (pad > 0)
        padding = std::string (pad, ' ');

      before << "  UDA Orphan " << *orphan << ": " << padding << task.get (*orphan) << "\n";
    }
  }

  before << "# " << _("End") << "\n";
  return before.str ();
}

////////////////////////////////////////////////////////////////////////////////
void CmdEdit::parseTask (Task& task, const std::string& after, const std::string& dateformat)
{
  // project
  std::string value = findValue (after, "\n  Project:");
  if (task.get ("project") != value)
  {
    if (value != "")
    {
      context.footnote (_("Project modified."));
      task.set ("project", value);
    }
    else
    {
      context.footnote (_("Project deleted."));
      task.remove ("project");
    }
  }

  // priority
  value = findValue (after, "\n  Priority:");
  if (task.get ("priority") != value)
  {
    if (value != "")
    {
      if (context.columns["priority"]->validate (value))
      {
        context.footnote (_("Priority modified."));
        task.set ("priority", value);
      }
    }
    else
    {
      context.footnote (_("Priority deleted."));
      task.remove ("priority");
    }
  }

  // tags
  value = findValue (after, "\n  Tags:");
  std::vector <std::string> tags;
  split (tags, value, ' ');
  task.remove ("tags");
  task.addTags (tags);

  // description.
  value = findValue (after, "\n  Description:");
  if (task.get ("description") != value)
  {
    if (value != "")
    {
      context.footnote (_("Description modified."));
      task.set ("description", value);
    }
    else
      throw std::string (_("Cannot remove description."));
  }

  // entry
  value = findValue (after, "\n  Created:");
  if (value != "")
  {
    std::string formatted = formatDate (task, "entry", dateformat);

    if (formatted != value)
    {
      context.footnote (_("Creation date modified."));
      task.set ("entry", Date(value, dateformat).toEpochString ());
    }
  }
  else
    throw std::string (_("Cannot remove creation date."));

  // start
  value = findValue (after, "\n  Started:");
  if (value != "")
  {
    if (task.get ("start") != "")
    {
      std::string formatted = formatDate (task, "start", dateformat);

      if (formatted != value)
      {
        context.footnote (_("Start date modified."));
        task.set ("start", Date(value, dateformat).toEpochString ());
      }
    }
    else
    {
      context.footnote (_("Start date modified."));
      task.set ("start", Date(value, dateformat).toEpochString ());
    }
  }
  else
  {
    if (task.get ("start") != "")
    {
      context.footnote (_("Start date removed."));
      task.remove ("start");
    }
  }

  // end
  value = findValue (after, "\n  Ended:");
  if (value != "")
  {
    if (task.get ("end") != "")
    {
      std::string formatted = formatDate (task, "end", dateformat);

      if (formatted != value)
      {
        context.footnote (_("End date modified."));
        task.set ("end", Date(value, dateformat).toEpochString ());
      }
    }
    else if (task.getStatus () != Task::deleted)
      throw std::string (_("Cannot set a done date on a pending task."));
  }
  else
  {
    if (task.get ("end") != "")
    {
      context.footnote (_("End date removed."));
      task.setStatus (Task::pending);
      task.remove ("end");
    }
  }

  // scheduled
  value = findValue (after, "\n  Scheduled:");
  if (value != "")
  {
    if (task.get ("scheduled") != "")
    {
      std::string formatted = formatDate (task, "scheduled", dateformat);

      if (formatted != value)
      {
        context.footnote (_("Scheduled date modified."));
        task.set ("scheduled", Date(value, dateformat).toEpochString ());
      }
    }
    else
    {
      context.footnote (_("Scheduled date modified."));
      task.set ("scheduled", Date(value, dateformat).toEpochString ());
    }
  }
  else
  {
    if (task.get ("scheduled") != "")
    {
      context.footnote (_("Scheduled date removed."));
      task.setStatus (Task::pending);
      task.remove ("scheduled");
    }
  }

  // due
  value = findValue (after, "\n  Due:");
  if (value != "")
  {
    if (task.get ("due") != "")
    {
      std::string formatted = formatDate (task, "due", dateformat);

      if (formatted != value)
      {
        context.footnote (_("Due date modified."));
        task.set ("due", Date(value, dateformat).toEpochString ());
      }
    }
    else
    {
      context.footnote (_("Due date modified."));
      task.set ("due", Date(value, dateformat).toEpochString ());
    }
  }
  else
  {
    if (task.get ("due") != "")
    {
      if (task.getStatus () == Task::recurring ||
          task.get ("parent") != "")
      {
        context.footnote (_("Cannot remove a due date from a recurring task."));
      }
      else
      {
        context.footnote (_("Due date removed."));
        task.remove ("due");
      }
    }
  }

  // until
  value = findValue (after, "\n  Until:");
  if (value != "")
  {
    if (task.get ("until") != "")
    {
      std::string formatted = formatDate (task, "until", dateformat);

      if (formatted != value)
      {
        context.footnote (_("Until date modified."));
        task.set ("until", Date(value, dateformat).toEpochString ());
      }
    }
    else
    {
      context.footnote (_("Until date modified."));
      task.set ("until", Date(value, dateformat).toEpochString ());
    }
  }
  else
  {
    if (task.get ("until") != "")
    {
      context.footnote (_("Until date removed."));
      task.remove ("until");
    }
  }

  // recur
  value = findValue (after, "\n  Recur:");
  if (value != task.get ("recur"))
  {
    if (value != "")
    {
      Duration d;
      if (d.valid (value))
      {
        context.footnote (_("Recurrence modified."));
        if (task.get ("due") != "")
        {
          task.set ("recur", value);
          task.setStatus (Task::recurring);
        }
        else
          throw std::string (_("A recurring task must have a due date."));
      }
      else
        throw std::string (_("Not a valid recurrence duration."));
    }
    else
    {
      context.footnote (_("Recurrence removed."));
      task.setStatus (Task::pending);
      task.remove ("recur");
      task.remove ("until");
      task.remove ("mask");
      task.remove ("imask");
    }
  }

  // wait
  value = findValue (after, "\n  Wait until:");
  if (value != "")
  {
    if (task.get ("wait") != "")
    {
      std::string formatted = formatDate (task, "wait", dateformat);

      if (formatted != value)
      {
        context.footnote (_("Wait date modified."));
        task.set ("wait", Date(value, dateformat).toEpochString ());
        task.setStatus (Task::waiting);
      }
    }
    else
    {
      context.footnote (_("Wait date modified."));
      task.set ("wait", Date(value, dateformat).toEpochString ());
      task.setStatus (Task::waiting);
    }
  }
  else
  {
    if (task.get ("wait") != "")
    {
      context.footnote (_("Wait date removed."));
      task.remove ("wait");
      task.setStatus (Task::pending);
    }
  }

  // parent
  value = findValue (after, "\n  Parent:");
  if (value != task.get ("parent"))
  {
    if (value != "")
    {
      context.footnote (_("Parent UUID modified."));
      task.set ("parent", value);
    }
    else
    {
      context.footnote (_("Parent UUID removed."));
      task.remove ("parent");
    }
  }

  // Annotations
  std::map <std::string, std::string> annotations;
  std::string::size_type found = 0;
  while ((found = after.find ("\n  Annotation:", found)) != std::string::npos)
  {
    found += 14;  // Length of "\n  Annotation:".

    std::string::size_type eol = after.find ("\n", found + 1);
    if (eol != std::string::npos)
    {
      std::string value = trim (after.substr (
        found,
        eol - found), "\t ");

      std::string::size_type gap = value.find (" -- ");
      if (gap != std::string::npos)
      {
        // TODO keeping the initial dates even if dateformat approximates them
        // is complex as finding the correspondence between each original line
        // and edited line may be impossible (bug #705). It would be simpler if
        // each annotation was put on a line with a distinguishable id (then
        // for each line: if the annotation is the same, then it is copied; if
        // the annotation is modified, then its original date may be kept; and
        // if there is no corresponding id, then a new unique date is created).
        Date when (value.substr (0, gap), dateformat);

        // This guarantees that if more than one annotation has the same date,
        // that the seconds will be different, thus unique, thus not squashed.
        // Bug #249
        when += (const int) annotations.size ();

        std::stringstream name;
        name << "annotation_" << when.toEpoch ();
        std::string text = trim (value.substr (gap + 4), "\t ");
        annotations.insert (std::make_pair (name.str (), text));
      }
    }
  }

  task.setAnnotations (annotations);

  // Dependencies
  value = findValue (after, "\n  Dependencies:");
  std::vector <std::string> dependencies;
  split (dependencies, value, ",");

  task.remove ("depends");
  std::vector <std::string>::iterator dep;
  for (dep = dependencies.begin (); dep != dependencies.end (); ++dep)
  {
    std::vector <int> ids;

    // Crude UUID check
    if (dep->length () == 36)
    {
      int id = context.tdb2.pending.id (*dep);
      ids.push_back (id);
    }
    else
      A3::extract_id (*dep, ids);

    std::vector <int>::iterator id;
    for (id = ids.begin (); id != ids.end(); id++)
      task.addDependency (*id);
  }

  // UDAs
  std::map <std::string, Column*>::iterator col;
  for (col = context.columns.begin (); col != context.columns.end (); ++col)
  {
    std::string type = context.config.get ("uda." + col->first + ".type");
    if (type != "")
    {
      std::string value = findValue (after, "\n  UDA " + col->first + ":");
      if ((task.get (col->first) != value) && (type != "date" ||
           (task.get (col->first) != Date(value, dateformat).toEpochString ())))
      {
        if (value != "")
        {
          context.footnote (format (_("UDA {1} modified."), col->first));

          if (type == "string")
          {
            task.set (col->first, value);
          }
          else if (type == "numeric")
          {
            Nibbler n (value);
            double d;
            if (n.getNumber (d) &&
                n.depleted ())
              task.set (col->first, value);
            else
              throw format (_("The value '{1}' is not a valid numeric value."), value);
          }
          else if (type == "date")
          {
            Date d (value, dateformat);
            task.set (col->first, d.toEpochString ());
          }
          else if (type == "duration")
          {
            Duration d (value);
            task.set (col->first, (time_t) d);
          }
        }
        else
        {
          context.footnote (format (_("UDA {1} deleted."), col->first));
          task.remove (col->first);
        }
      }
    }
  }
  
  // UDA orphans
  std::vector <std::string> orphanValues = findValues (after, "\n  UDA Orphan ");
  std::vector <std::string>::iterator orphan;
  for (orphan = orphanValues.begin (); orphan != orphanValues.end (); ++orphan)
  {
    std::string::size_type colon = orphan->find (':');
    if (colon != std::string::npos)
    {
      std::string name  = trim (orphan->substr (0, colon),  "\t ");
      std::string value = trim (orphan->substr (colon + 1), "\t ");

      if (value != "")
        task.set (name, value);
      else
        task.remove (name);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
bool CmdEdit::editFile (Task& task)
{
  // Check for file permissions.
  Directory location (context.config.get ("data.location"));
  if (! location.writable ())
    throw std::string (_("Your data.location directory is not writable."));

  // Create a temp file name in data.location.
  std::stringstream file;
  file << "task." << getpid () << "." << task.id << ".task";

  // Determine the output date format, which uses a hierarchy of definitions.
  //   rc.dateformat.edit
  //   rc.dateformat
  std::string dateformat = context.config.get ("dateformat.edit");
  if (dateformat == "")
    dateformat = context.config.get ("dateformat");

  // Change directory for the editor
  std::string current_dir = Directory::cwd ();
  int ignored = chdir (location._data.c_str ());
  ++ignored; // Keep compiler quiet.

  // Format the contents, T -> text, write to a file.
  std::string before = formatTask (task, dateformat);
  std::string before_orig = before;
  File::write (file.str (), before);

  // Determine correct editor: .taskrc:editor > $VISUAL > $EDITOR > vi
  std::string editor = context.config.get ("editor");
  char* peditor = getenv ("VISUAL");
  if (editor == "" && peditor) editor = std::string (peditor);
  peditor = getenv ("EDITOR");
  if (editor == "" && peditor) editor = std::string (peditor);
  if (editor == "") editor = "vi";

  // Complete the command line.
  editor += " ";
  editor += "\"" + file.str () + "\"";

ARE_THESE_REALLY_HARMFUL:
  bool changes = false; // No changes made.

  // Launch the editor.
  std::cout << format (_("Launching '{1}' now..."), editor) << "\n";
  if (-1 == system (editor.c_str ()))
    std::cout << _("No editing performed.") << "\n";
  else
    std::cout << _("Editing complete.") << "\n";

  // Slurp file.
  std::string after;
  File::read (file.str (), after);

  // Update task based on what can be parsed back out of the file, but only
  // if changes were made.
  if (before_orig != after)
  {
    std::cout << _("Edits were detected.") << "\n";
    std::string problem = "";
    bool oops = false;

    try
    {
      parseTask (task, after, dateformat);
    }

    catch (const std::string& e)
    {
      problem = e;
      oops = true;
    }

    if (oops)
    {
      std::cerr << _("Error: ") << problem << "\n";

      // Preserve the edits.
      before = after;
      File::write (file.str (), before);

      if (confirm (_("Taskwarrior couldn't handle your edits.  Would you like to try again?")))
        goto ARE_THESE_REALLY_HARMFUL;
    }
    else
      changes = true;
  }
  else
  {
    std::cout << _("No edits were detected.") << "\n";
    changes = false;
  }

  // Cleanup.
  File::remove (file.str ());
  ignored = chdir (current_dir.c_str ());
  return changes;
}

////////////////////////////////////////////////////////////////////////////////
