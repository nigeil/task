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
#include <ColPriority.h>
#include <text.h>
#include <i18n.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
ColumnPriority::ColumnPriority ()
{
  _name  = "priority";
  _type  = "string";
  _style = "short";
  // TRANSLATORS: Short for "priority".
  _label = sgettext("column|Pri");

  _styles.push_back ("short");
  _styles.push_back ("long");

  _examples.push_back ("H");
  _examples.push_back ("High");
}

////////////////////////////////////////////////////////////////////////////////
ColumnPriority::~ColumnPriority ()
{
}

////////////////////////////////////////////////////////////////////////////////
bool ColumnPriority::validate (std::string& value)
{
  value = upperCase (value);

  if (value == "H" ||
      value == "M" ||
      value == "L" ||
      value == "")
    return true;

  return false;
}

////////////////////////////////////////////////////////////////////////////////
// Overriden so that style <----> label are linked.
// Note that you can not determine which gets called first.
void ColumnPriority::setStyle (const std::string& value)
{
  _style = value;

  if (_style == "long" && _label == sgettext("column|Pri"))
    _label = sgettext("column|Priority");
}

////////////////////////////////////////////////////////////////////////////////
// Set the minimum and maximum widths for the value.
void ColumnPriority::measure (Task& task, unsigned int& minimum, unsigned int& maximum)
{
  std::string priority = task.get (_name);

  if (priority == "")
    minimum = maximum = 0;
  else
    minimum = maximum = 1;

  if (_style == "long")
  {
         if (priority == "H") minimum = maximum = 4;
    else if (priority == "M") minimum = maximum = 6;
    else if (priority == "L") minimum = maximum = 3;
  }
  else if (_style != "default" &&
           _style != "short")
    throw format (_("Unrecognized column format '{1}.{2}'"), "priority", _style);
}

////////////////////////////////////////////////////////////////////////////////
void ColumnPriority::render (
  std::vector <std::string>& lines,
  Task& task,
  int width,
  Color& color)
{
  std::string priority = task.get (_name);
  if (_style == "long")
  {
         if (priority == "H") priority = "High";
    else if (priority == "M") priority = "Medium";
    else if (priority == "L") priority = "Low";
  }

  lines.push_back (color.colorize (leftJustify (priority, width)));
}

////////////////////////////////////////////////////////////////////////////////
