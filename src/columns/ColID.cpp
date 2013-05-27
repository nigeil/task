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
#include <math.h>
#include <Context.h>
#include <ColID.h>
#include <text.h>
#include <i18n.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
ColumnID::ColumnID ()
{
  _name       = "id";
  _type       = "number";
  _style      = "number";
  _label      = sgettext("column|ID");
  _modifiable = false;

  _styles.push_back ("number");

  _examples.push_back ("123");
}

////////////////////////////////////////////////////////////////////////////////
ColumnID::~ColumnID ()
{
}

////////////////////////////////////////////////////////////////////////////////
bool ColumnID::validate (std::string& value)
{
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Set the minimum and maximum widths for the value.
void ColumnID::measure (Task& task, unsigned int& minimum, unsigned int& maximum)
{
  int length;

       if (task.id < 10)     length = 1;                              // Fast
  else if (task.id < 100)    length = 2;                              // Fast
  else if (task.id < 1000)   length = 3;                              // Fast
  else if (task.id < 10000)  length = 4;                              // Fast
  else                       length = (int) log10 ((double) task.id); // Slow

  minimum = maximum = length;

  if (_style != "default" &&
      _style != "number")
    throw format (_("Unrecognized column format '{1}.{2}'"), _name, _style);
}

////////////////////////////////////////////////////////////////////////////////
void ColumnID::render (
  std::vector <std::string>& lines,
  Task& task,
  int width,
  Color& color)
{
  if (task.id)
    lines.push_back (color.colorize (rightJustify (task.id, width)));
  else
    lines.push_back (color.colorize (rightJustify ("-", width)));
}

////////////////////////////////////////////////////////////////////////////////
