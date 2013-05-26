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
#include <Context.h>
#include <ViewText.h>
#include <Color.h>
#include <text.h>
#include <i18n.h>
#include <main.h>
#include <CmdColumns.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdColumns::CmdColumns ()
{
  _keyword     = "columns";
  _usage       = "task          columns [substring]";
  _description = _("All supported columns and formatting styles");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdColumns::execute (std::string& output)
{
  // Obtain the arguments from the description.  That way, things like '--'
  // have already been handled.
  std::vector <std::string> words = context.a3.extract_words ();
  if (words.size () > 1)
    throw std::string (_("You can only specify one search string."));

  // Include all columns in the table.
  std::vector <std::string> names;
  std::map <std::string, Column*>::const_iterator col;
  for (col = context.columns.begin (); col != context.columns.end (); ++col)
    names.push_back (col->first);

  std::sort (names.begin (), names.end ());

  // Render a list of column names, formats and examples.
  ViewText formats;
  formats.width (context.getWidth ());
  formats.add (Column::factory ("string", _("Columns")));
  formats.add (Column::factory ("string", _("Supported Formats")));
  formats.add (Column::factory ("string", _("Example")));

  Color alternate (context.config.get ("color.alternate"));
  formats.colorOdd (alternate);
  formats.intraColorOdd (alternate);

  std::vector <std::string>::iterator name;
  for (name = names.begin (); name != names.end (); ++name)
  {
    if (words.size () == 0 ||
        find (*name, words[0], false) != std::string::npos)
    {
      const std::vector <std::string> styles   = context.columns[*name]->styles ();
      const std::vector <std::string> examples = context.columns[*name]->examples ();

      for (unsigned int i = 0; i < styles.size (); ++i)
      {
        int row = formats.addRow ();
        formats.set (row, 0, i == 0 ? *name : "");
        formats.set (row, 1, styles[i] + (i == 0 ? "*" : ""));
        formats.set (row, 2, i < examples.size () ? examples[i] : "");
      }
    }
  }

  output = optionalBlankLine ()
         + formats.render ()
         + "\n"
         + _("* Means default format, and therefore optional.  For example, 'due' and 'due.formatted' are equivalent.")
         + "\n";

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
CmdCompletionColumns::CmdCompletionColumns ()
{
  _keyword     = "_columns";
  _usage       = "task          _columns";
  _description = _("Displays only a list of supported columns");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdCompletionColumns::execute (std::string& output)
{
  // Include all columns.
  std::vector <std::string> names;
  std::map <std::string, Column*>::const_iterator col;
  for (col = context.columns.begin (); col != context.columns.end (); ++col)
    names.push_back (col->first);

  std::sort (names.begin (), names.end ());

  // Render only the column names.
  std::vector <std::string>::iterator name;
  for (name = names.begin (); name != names.end (); ++name)
    output += *name + "\n";

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
