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
#include <ColStatus.h>
#include <text.h>
#include <i18n.h>
#include <utf8.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
ColumnStatus::ColumnStatus ()
{
  _name  = "status";
  _type  = "string";
  _style = "long";
  _label = _("Status");

  _styles.push_back ("long");
  _styles.push_back ("short");

  _examples.push_back (_("Pending"));
  _examples.push_back (_("P"));
}

////////////////////////////////////////////////////////////////////////////////
ColumnStatus::~ColumnStatus ()
{
}

////////////////////////////////////////////////////////////////////////////////
bool ColumnStatus::validate (std::string& value)
{
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Overriden so that style <----> label are linked.
// Note that you can not determine which gets called first.
void ColumnStatus::setStyle (const std::string& value)
{
  _style = value;

  if (_style == "short" && _label == _("Status"))
    // TRANSLATORS: Short for "status".
    _label = _("St");
}

////////////////////////////////////////////////////////////////////////////////
// Set the minimum and maximum widths for the value.
void ColumnStatus::measure (Task& task, unsigned int& minimum, unsigned int& maximum)
{
  Task::status status = task.getStatus ();

  if (_style == "default" ||
      _style == "long")
  {
    if (status == Task::pending)
      // TRANSLATORS: Task pending.
      minimum = maximum = utf8_width (_("Pending"));
    else if (status == Task::deleted)
      // TRANSLATORS: Task deleted.
      minimum = maximum = utf8_width (_("Deleted"));
    else if (status == Task::waiting)
      // TRANSLATORS: Task waiting.
      minimum = maximum = utf8_width (_("Waiting"));
    else if (status == Task::completed)
      minimum = maximum = utf8_width (_("Completed"));
    else if (status == Task::recurring)
      // TRANSLATORS: Task recurring.
      minimum = maximum = utf8_width (_("Recurring"));
  }
  else if (_style == "short")
    minimum = maximum = 1;
  else
    throw format (_("Unrecognized column format '{1}.{2}'"), _name, _style);
}

////////////////////////////////////////////////////////////////////////////////
void ColumnStatus::render (
  std::vector <std::string>& lines,
  Task& task,
  int width,
  Color& color)
{
  Task::status status = task.getStatus ();
  std::string value;

  if (_style == "default" ||
      _style == "long")
  {
         if (status == Task::pending)   value = _("Pending");
    else if (status == Task::completed) value = _("Completed");
    else if (status == Task::deleted)   value = _("Deleted");
    else if (status == Task::waiting)   value = _("Waiting");
    else if (status == Task::recurring) value = _("Recurring");
  }

  else if (_style == "short")
  {
         if (status == Task::pending)   value = /* TRANSLATORS: Task pending.   */ _("P");
    else if (status == Task::completed) value = /* TRANSLATORS: Task completed. */ _("C");
    else if (status == Task::deleted)   value = /* TRANSLATORS: Task deleted.   */ _("D");
    else if (status == Task::waiting)   value = /* TRANSLATORS: Task waiting.   */ _("W");
    else if (status == Task::recurring) value = /* TRANSLATORS: Task recurring. */ _("R");
  }

  lines.push_back (color.colorize (leftJustify (value, width)));
}

////////////////////////////////////////////////////////////////////////////////
