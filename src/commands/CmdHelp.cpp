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

#define L10N                                           // Localization complete.

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
  _description = STRING_CMD_HELP_USAGE;
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
  view.set (row, 0, STRING_CMD_HELP_USAGE_LABEL);
  view.set (row, 1, "task");
  view.set (row, 2, STRING_CMD_HELP_USAGE_DESC);

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
    view.set (row, 2, format (STRING_CMD_HELP_ALIASED, alias->second));
  }

  output = "\n"
         + view.render ()
         + "\n"
         + STRING_CMD_HELP_TEXT;

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
