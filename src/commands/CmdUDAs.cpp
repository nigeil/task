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
#include <sstream>
#include <algorithm>
#include <Context.h>
#include <ViewText.h>
#include <main.h>
#include <text.h>
#include <util.h>
#include <i18n.h>
#include <CmdUDAs.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdUDAs::CmdUDAs ()
{
  _keyword     = "udas";
  _usage       = "task          udas";
  _description = _("Shows all the defined UDA details");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdUDAs::execute (std::string& output)
{
  int rc = 0;
  std::stringstream out;

  std::vector <std::string> names;
  context.config.all (names);

  std::vector <std::string> udas;
  std::vector <std::string>::iterator name;
  for (name = names.begin (); name != names.end (); ++name)
  {
    if (name->substr (0, 4) == "uda." &&
        name->find (".type") != std::string::npos)
    {
      std::string::size_type period = name->find ('.', 4);
      if (period != std::string::npos)
        udas.push_back (name->substr (4, period - 4));
    }
  }

  // Load/filter all data.
  std::vector <Task> filtered;
  filter (filtered);

  if (udas.size ())
  {
    std::sort (udas.begin (), udas.end ());

    // Render a list of UDA name, type, label, allowed values,
    // possible default value, and finally the usage count.
    ViewText view;
    view.width (context.getWidth ());
    view.add (Column::factory ("string", _("Name")));
    view.add (Column::factory ("string", _("Type")));
    view.add (Column::factory ("string", _("Label")));
    view.add (Column::factory ("string", _("Allowed Values")));
    view.add (Column::factory ("string", _("Default")));
    view.add (Column::factory ("string", _("Usage Count")));

    std::vector <std::string>::iterator uda;
    for (uda = udas.begin (); uda != udas.end (); ++uda)
    {
      std::string type   = context.config.get ("uda." + *uda + ".type");
      std::string label  = context.config.get ("uda." + *uda + ".label");
      std::string values = context.config.get ("uda." + *uda + ".values");
      std::string defval = context.config.get ("uda." + *uda + ".default");
      if (label == "")
        label = *uda;

      // Count UDA usage by UDA.
      int count = 0;
      std::vector <Task>::iterator i;
      for (i = filtered.begin (); i != filtered.end (); ++i)
        if (i->has (*uda))
          ++count;

      int row = view.addRow ();
      view.set (row, 0, *uda);
      view.set (row, 1, type);
      view.set (row, 2, label);
      view.set (row, 3, values);
      view.set (row, 4, defval);
      view.set (row, 5, count);
    }

    out << optionalBlankLine ()
        << view.render ()
        << optionalBlankLine ()
        << (udas.size () == 1
              ? format (_("{1} UDA defined"),  udas.size ())
              : format (_("{1} UDAs defined"), udas.size ()))
        << "\n";
  }
  else
  {
    out << _("No UDAs defined.") << "\n";
    rc = 1;
  }

  // Orphans are task attributes that are not represented in context.columns.
  std::map <std::string, int> orphans;
  std::vector <Task>::iterator i;
  for (i = filtered.begin (); i != filtered.end (); ++i)
  {
    std::map <std::string, std::string>::iterator att;
    for (att = i->begin (); att != i->end (); ++att)
      if (att->first.substr (0, 11) != "annotation_" &&
          context.columns.find (att->first) == context.columns.end ())
        orphans[att->first]++;
  }

  if (orphans.size ())
  {
    // Display the orphans and their counts.
    ViewText orphanView;
    orphanView.width (context.getWidth ());
    orphanView.add (Column::factory ("string", _("Orphan UDA")));
    orphanView.add (Column::factory ("string", _("Usage Count")));

    std::map <std::string, int>::iterator o;
    for (o = orphans.begin (); o != orphans.end (); ++o)
    {
      int row = orphanView.addRow ();
      orphanView.set (row, 0, o->first);
      orphanView.set (row, 1, o->second);
    }

    out << optionalBlankLine ()
        << orphanView.render ()
        << optionalBlankLine ()
        << (udas.size () == 1
              ? format (_("{1} Orphan UDA"),  orphans.size ())
              : format (_("{1} Orphan UDAs"), orphans.size ()))
        << "\n";
  }

  output = out.str ();
  return rc;
}

///////////////////////////////////////////////////////////////////////////////
CmdCompletionUDAs::CmdCompletionUDAs ()
{
  _keyword     = "_udas";
  _usage       = "task          _udas";
  _description = _("Shows the defined UDAs for completion purposes");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdCompletionUDAs::execute (std::string& output)
{
  std::vector <std::string> names;
  context.config.all (names);

  std::vector <std::string> udas;
  std::vector <std::string>::iterator name;
  for (name = names.begin (); name != names.end (); ++name)
  {
    if (name->substr (0, 4) == "uda." &&
        name->find (".type") != std::string::npos)
    {
      std::string::size_type period = name->find ('.', 4);
      if (period != std::string::npos)
        udas.push_back (name->substr (4, period - 4));
    }
  }

  if (udas.size ())
  {
    std::sort (udas.begin (), udas.end ());
    join (output, "\n", udas);
    output += "\n";
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
