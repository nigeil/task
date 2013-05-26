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

#include <stdlib.h>
#include <Context.h>
#include <Date.h>
#include <ColDescription.h>
#include <text.h>
#include <utf8.h>
#include <util.h>
#include <i18n.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
ColumnDescription::ColumnDescription ()
{
  _name  = "description";
  _type  = "string";
  _style = "combined";
  _label = sgettext("column|Description");

  _styles.push_back ("combined");
  _styles.push_back ("desc");
  _styles.push_back ("oneline");
  _styles.push_back ("truncated");
  _styles.push_back ("count");

  _dateformat = context.config.get ("dateformat.annotation");
  if (_dateformat == "")
    _dateformat = context.config.get ("dateformat");

  std::string t  = Date ().toString (_dateformat);
  // TRANSLATORS: Sample description.
  std::string d  = _("Move your clothes down on to the lower peg");
  // TRANSLATORS: Sample annotation #1.
  std::string a1 = _("Immediately before your lunch");
  // TRANSLATORS: Sample annotation #2.
  std::string a2 = _("If you are playing in the match this afternoon");
  // TRANSLATORS: Sample annotation #3.
  std::string a3 = _("Before you write your letter home");
  // TRANSLATORS: Sample annotation #4.
  std::string a4 = _("If you're not getting your hair cut");

  _examples.push_back (d
                       + "\n  " + t + " " + a1
                       + "\n  " + t + " " + a2
                       + "\n  " + t + " " + a3
                       + "\n  " + t + " " + a4);
  _examples.push_back (d);
  _examples.push_back (d
                       + " " + t + " " + a1
                       + " " + t + " " + a2
                       + " " + t + " " + a3
                       + " " + t + " " + a4);
  _examples.push_back (d.substr (0, 20) + "...");
  _examples.push_back (d + " [4]");

  _hyphenate = context.config.getBoolean ("hyphenate");

  _indent = context.config.getInteger ("indent.annotation");
}

////////////////////////////////////////////////////////////////////////////////
ColumnDescription::~ColumnDescription ()
{
}

////////////////////////////////////////////////////////////////////////////////
bool ColumnDescription::validate (std::string& value)
{
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Set the minimum and maximum widths for the value.
void ColumnDescription::measure (Task& task, unsigned int& minimum, unsigned int& maximum)
{
  std::string description = task.get (_name);

  // The text
  // <indent> <date> <anno>
  // ...
  if (_style == "default" ||
      _style == "combined")
  {
    minimum = longestWord (description);
    maximum = utf8_width (description);

    if (task.annotation_count)
    {
      unsigned int min_anno = _indent + Date::length (_dateformat);
      if (min_anno > minimum)
        minimum = min_anno;

      std::map <std::string, std::string> annos;
      task.getAnnotations (annos);
      std::map <std::string, std::string>::iterator i;
      for (i = annos.begin (); i != annos.end (); i++)
      {
        unsigned int len = min_anno + 1 + utf8_width (i->second);
        if (len > maximum)
          maximum = len;
      }
    }
  }

  // Just the text
  else if (_style == "desc")
  {
    maximum = utf8_width (description);
    minimum = longestWord (description);
  }

  // The text <date> <anno> ...
  else if (_style == "oneline")
  {
    minimum = longestWord (description);
    maximum = utf8_width (description);

    if (task.annotation_count)
    {
      unsigned int min_anno = Date::length (_dateformat);
      std::map <std::string, std::string> annos;
      task.getAnnotations (annos);
      std::map <std::string, std::string>::iterator i;
      for (i = annos.begin (); i != annos.end (); i++)
        maximum += min_anno + 1 + utf8_width (i->second);
    }
  }

  // The te...
  else if (_style == "truncated")
  {
    minimum = 4;
    maximum = utf8_width (description);
  }

  // The text [2]
  else if (_style == "count")
  {
    // <description> + ' ' + '[' + <count> + ']'
    maximum = utf8_width (description) + 1 + 1 + format (task.annotation_count).length () + 1;
    minimum = longestWord (description);
  }

  else
    throw format (_("Unrecognized column format '{1}.{2}'"), _name, _style);
}

////////////////////////////////////////////////////////////////////////////////
void ColumnDescription::render (
  std::vector <std::string>& lines,
  Task& task,
  int width,
  Color& color)
{
  std::string description = task.get (_name);

  // This is a description
  // <date> <anno>
  // ...
  if (_style == "default" ||
      _style == "combined")
  {
    std::map <std::string, std::string> annos;
    task.getAnnotations (annos);
    if (annos.size ())
    {
      std::map <std::string, std::string>::iterator i;
      for (i = annos.begin (); i != annos.end (); i++)
      {
        Date dt (strtol (i->first.substr (11).c_str (), NULL, 10));
        description += "\n" + std::string (_indent, ' ') + dt.toString (_dateformat) + " " + i->second;
      }
    }

    std::vector <std::string> raw;
    wrapText (raw, description, width, _hyphenate);

    std::vector <std::string>::iterator i;
    for (i = raw.begin (); i != raw.end (); ++i)
      lines.push_back (color.colorize (leftJustify (*i, width)));
  }

  // This is a description
  else if (_style == "desc")
  {
    std::vector <std::string> raw;
    wrapText (raw, description, width, _hyphenate);

    std::vector <std::string>::iterator i;
    for (i = raw.begin (); i != raw.end (); ++i)
      lines.push_back (color.colorize (leftJustify (*i, width)));
  }

  // This is a description <date> <anno> ...
  else if (_style == "oneline")
  {
    std::map <std::string, std::string> annos;
    task.getAnnotations (annos);
    if (annos.size ())
    {
      std::map <std::string, std::string>::iterator i;
      for (i = annos.begin (); i != annos.end (); i++)
      {
        Date dt (atoi (i->first.substr (11).c_str ()));
        description += " " + dt.toString (_dateformat) + " " + i->second;
      }
    }

    std::vector <std::string> raw;
    wrapText (raw, description, width, _hyphenate);

    std::vector <std::string>::iterator i;
    for (i = raw.begin (); i != raw.end (); ++i)
      lines.push_back (color.colorize (leftJustify (*i, width)));
  }

  // This is a des...
  else if (_style == "truncated")
  {
    int len = utf8_width (description);
    if (len > width)
      lines.push_back (color.colorize (description.substr (0, width - 3) + "..."));
    else
      lines.push_back (color.colorize (leftJustify (description, width)));
  }

  // This is a description [2]
  else if (_style == "count")
  {
    std::map <std::string, std::string> annos;
    task.getAnnotations (annos);

    if (annos.size ())
      description += " [" + format ((int) annos.size ()) + "]";

    std::vector <std::string> raw;
    wrapText (raw, description, width, _hyphenate);

    std::vector <std::string>::iterator i;
    for (i = raw.begin (); i != raw.end (); ++i)
      lines.push_back (color.colorize (leftJustify (*i, width)));
  }
}

////////////////////////////////////////////////////////////////////////////////
