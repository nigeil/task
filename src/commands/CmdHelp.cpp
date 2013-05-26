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
#include <CmdHelp.h>
#include <ViewText.h>
#include <Context.h>
#include <i18n.h>
#include <text.h>
#include <util.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdHelp::CmdHelp ()
{
  _keyword     = "help";
  _usage       = "task          help";
  _description = _("Displays this usage help text");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdHelp::execute (std::string& output)
{
  ViewText view;
  view.width (context.getWidth ());
  view.add (Column::factory ("string.left", ""));
  view.add (Column::factory ("string.left", ""));
  view.add (Column::factory ("string.left", ""));

  // Static first row.
  int row = view.addRow ();
  view.set (row, 0, _("Usage:"));
  view.set (row, 1, "task");
  view.set (row, 2, _("Runs rc.default.command, if specified."));

  // Obsolete method of getting a list of all commands.
  std::vector <std::string> all;
  std::map <std::string, Command*>::iterator i;
  for (i = context.commands.begin (); i != context.commands.end (); ++i)
    all.push_back (i->first);

  // Sort alphabetically by usage.
  std::sort (all.begin (), all.end ());

  // Add the regular commands.
  std::vector <std::string>::iterator name;
  for (name = all.begin (); name != all.end (); ++name)
  {
    if ((*name)[0] != '_')
    {
      row = view.addRow ();
      view.set (row, 1, context.commands[*name]->usage ());
      view.set (row, 2, context.commands[*name]->description ());
    }
  }

  // Add the helper commands.
  for (name = all.begin (); name != all.end (); ++name)
  {
    if ((*name)[0] == '_')
    {
      row = view.addRow ();
      view.set (row, 1, context.commands[*name]->usage ());
      view.set (row, 2, context.commands[*name]->description ());
    }
  }

  // Add the aliases commands.
  row = view.addRow ();
  view.set (row, 1, " ");

  std::map <std::string, std::string>::iterator alias;
  for (alias =  context.aliases.begin ();
       alias != context.aliases.end ();
       ++alias)
  {
    row = view.addRow ();
    view.set (row, 1, alias->first);
    view.set (row, 2, format (_("Aliased to '{1}'"), alias->second));
  }

  output = "\n"
         + view.render ()
         + "\n"
         + _("Documentation for Taskwarrior can be found using 'man task', 'man taskrc', 'man " \
             "task-tutorial', 'man task-color', 'man task-faq', 'man task-synch or at " \
             "http://taskwarrior.org\n" \
             "\n" \
             "The general form of commands is:\n" \
             "  task [<filter>] <command> [<mods>]\n" \
             "\n" \
             "The <filter> consists of zero or more restrictions on which tasks to select, " \
             "such as:\n" \
             "  task                                      <command> <mods>\n" \
             "  task 28                                   <command> <mods>\n" \
             "  task +weekend                             <command> <mods>\n" \
             "  task project:Home due.before:today        <command> <mods>\n" \
             "  task ebeeab00-ccf8-464b-8b58-f7f2d606edfb <command> <mods>\n" \
             "\n" \
             "By default, filter elements are combined with an implicit 'and' operator, but " \
             "'or' and 'xor' may also be used, provided parentheses are included:\n" \
             "  task '(/[Cc]at|[Dd]og/ or /[0-9]+/)'      <command> <mods>\n" \
             "\n" \
             "A filter may target specific tasks using ID or UUID numbers.  To specify " \
             "multiple tasks use one of these forms:\n" \
             "  task 1,2,3                                    delete\n" \
             "  task 1-3                                      info\n" \
             "  task 1,2-5,19                                 modify pri:H\n" \
             "  task 4-7 ebeeab00-ccf8-464b-8b58-f7f2d606edfb info\n" \
             "\n" \
             "The <mods> consist of zero or more changes to apply to the selected tasks, " \
             "such as:\n" \
             "  task <filter> <command> project:Home\n" \
             "  task <filter> <command> +weekend +garden due:tomorrow\n" \
             "  task <filter> <command> Description/annotation text\n" \
             "\n" \
             "Tags are arbitrary words, any quantity:\n" \
             "  +tag       The + means add the tag\n" \
             "  -tag       The - means remove the tag\n" \
             "\n" \
             "Built-in attributes are:\n" \
             "  description:    Task description text\n" \
             "  status:         Status of task - pending, completed, deleted, waiting\n" \
             "  project:        Project name\n" \
             "  priority:       Priority\n" \
             "  due:            Due date\n" \
             "  recur:          Recurrence frequency\n" \
             "  until:          Expiration date of a task\n" \
             "  limit:          Desired number of rows in report, or 'page'\n" \
             "  wait:           Date until task becomes pending\n" \
             "  entry:          Date task was created\n" \
             "  end:            Date task was completed/deleted\n" \
             "  start:          Date task was started\n" \
             "  scheduled:      Date task is scheduled to start\n" \
             "  depends:        Other tasks that this task depends upon\n" \
             "\n" \
             "Attribute modifiers make filters more precise.  Supported modifiers are:\n" \
             "  before     (synonyms under, below)\n" \
             "  after      (synonyms over, above)\n" \
             "  none\n" \
             "  any\n" \
             "  is         (synonym equals)\n" \
             "  isnt       (synonym not)\n" \
             "  has        (synonym contains)\n" \
             "  hasnt\n" \
             "  startswith (synonym left)\n" \
             "  endswith   (synonym right)\n" \
             "  word\n" \
             "  noword\n" \
             "\n" \
             "Alternately algebraic expressions support:\n" \
             "  and  or  xor            Logical operators\n" \
             "  <  <=  =  !=  >=  >     Relational operators\n" \
             "  (  )                    Precedence\n" \
             "\n" \
             "  task due.before:eom priority.not:L   list\n" \
             "  task '(due < eom or priority != L)'  list\n" \
             "\n" \
             "The default .taskrc file can be overridden with:\n" \
             "  task ... rc:<alternate file> ...\n" \
             "  task ... rc:~/.alt_taskrc ...\n" \
             "\n" \
             "The values in .taskrc (or alternate) can be overridden with:\n" \
             "  task ... rc.<name>=<value> ...\n" \
             "  task rc.color=off list\n" \
             "\n" \
             "Any command or attribute name may be abbreviated if still unique:\n" \
             "  task list project:Home\n" \
             "  task li       pro:Home\n" \
             "\n" \
             "Some task descriptions need to be escaped because of the shell:\n" \
             "  task add \"quoted ' quote\"\n" \
             "  task add escaped \\' quote\n" \
             "\n" \
             "The argument -- tells taskwarrior to treat all other args as description, even " \
             "if they would otherwise be attributes or tags:\n" \
             "  task add -- project:Home needs scheduling\n" \
             "\n" \
             "Many characters have special meaning to the shell, including:\n" \
             "  $ ! ' \" ( ) ; \\ ` * ? { } [ ] < > | & % # ~\n" \
             "\n");

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
