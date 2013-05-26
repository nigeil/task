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

#include <Context.h>
#include <ColProject.h>
#include <text.h>
#include <utf8.h>
#include <util.h>
#include <i18n.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
ColumnProject::ColumnProject ()
{
  _name  = "project";
  _type  = "string";
  _style = "full";
  _label = sgettext("column|Project");

  _styles.push_back ("full");
  _styles.push_back ("parent");
  _styles.push_back ("indented");

  // TRANSLATORS: Sample project.
  _examples.push_back (_("home.garden"));
  // TRANSLATORS: Sample parent project.
  _examples.push_back (_("home"));
  // TRANSLATORS: Sample indented project.
  _examples.push_back (_("  home.garden"));

  _hyphenate = context.config.getBoolean ("hyphenate");
}

////////////////////////////////////////////////////////////////////////////////
ColumnProject::~ColumnProject ()
{
}

////////////////////////////////////////////////////////////////////////////////
bool ColumnProject::validate (std::string& value)
{
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Set the minimum and maximum widths for the value.
void ColumnProject::measure (Task& task, unsigned int& minimum, unsigned int& maximum)
{
  std::string project = task.get (_name);

  if (_style == "parent")
  {
    std::string::size_type period = project.find ('.');
    if (period != std::string::npos)
      project = project.substr (0, period);
  }
  else if (_style == "indented")
  {
    project = indentProject (project, "  ", '.');
  }
  else if (_style != "default"  &&
           _style != "full"     &&
           _style != "indented")
    throw format (_("Unrecognized column format '{1}.{2}'"), _name, _style);

  minimum = longestWord (project);
  maximum = utf8_width (project);
}

////////////////////////////////////////////////////////////////////////////////
void ColumnProject::render (
  std::vector <std::string>& lines,
  Task& task,
  int width,
  Color& color)
{
  std::string project = task.get (_name);
  if (_style == "parent")
  {
    std::string::size_type period = project.find ('.');
    if (period != std::string::npos)
      project = project.substr (0, period);
  }
  else if (_style == "indented")
  {
    project = indentProject (project, "  ", '.');
  }

  std::vector <std::string> raw;
  wrapText (raw, project, width, _hyphenate);

  std::vector <std::string>::iterator i;
  for (i = raw.begin (); i != raw.end (); ++i)
    lines.push_back (color.colorize (leftJustify (*i, width)));
}

////////////////////////////////////////////////////////////////////////////////
