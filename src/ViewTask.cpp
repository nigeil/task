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
#include <numeric>
#include <ViewTask.h>
#include <Context.h>
#include <Timer.h>
#include <text.h>
#include <utf8.h>
#include <i18n.h>
#include <main.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
ViewTask::ViewTask ()
: _width (0)
, _left_margin (0)
, _header (0)
, _odd (0)
, _even (0)
, _intra_padding (1)
, _intra_odd (0)
, _intra_even (0)
, _extra_padding (0)
, _extra_odd (0)
, _extra_even (0)
, _truncate_lines (0)
, _truncate_rows (0)
, _lines (0)
, _rows (0)
{
}

////////////////////////////////////////////////////////////////////////////////
ViewTask::~ViewTask ()
{
  std::vector <Column*>::iterator i;
  for (i = _columns.begin (); i != _columns.end (); ++i)
    delete *i;

  _columns.clear ();
}

////////////////////////////////////////////////////////////////////////////////
//   |<---------- terminal width ---------->|
//
//         +-------+  +-------+  +-------+
//         |header |  |header |  |header |
//   +--+--+-------+--+-------+--+-------+--+
//   |ma|ex|cell   |in|cell   |in|cell   |ex|
//   +--+--+-------+--+-------+--+-------+--+
//   |ma|ex|cell   |in|cell   |in|cell   |ex|
//   +--+--+-------+--+-------+--+-------+--+
// 
//   margin        - indentation for the whole table
//   extrapadding  - left and right padding for the whole table
//   intrapadding  - padding between columns
//
//
// Layout Algorithm:
//   - Height is irrelevant
//   - Determine the usable horizontal space for N columns:
//
//       usable = width - ma - (ex * 2) - (in * (N - 1))
//
//   - Look at every column, for every task, and determine the minimum and
//     maximum widths.  The minimum is the length of the largest indivisible
//     word, and the maximum is the full length of the value.
//   - If there is sufficient terminal width to display every task using the
//     maximum width, then do so.
//   - If there is insufficient terminal width to display every task using the
//     minimum width, then there is no layout solution.  Error.
//   - Otherwise there is a need for column wrapping.  Calculate the overage,
//     which is the difference between the sum of the minimum widths and the
//     usable width.
//   - Start by using all the minimum column widths, and distribute the overage
//     among all columns, one character at a time, while the column width is
//     less than the maximum width, and while there is overage remaining.
//
// Note: a possible enhancement is to proportionally distribute the overage
//       according to average data length.
//
// Note: an enhancement to the 'no solution' problem is to simply force-break
//       the larger fields.  If the widest field is W0, and the second widest
//       field is W1, then a solution may be achievable by reducing W0 --> W1.
//
std::string ViewTask::render (std::vector <Task>& data, std::vector <int>& sequence)
{
  context.timer_render.start ();

  bool const print_empty_columns = context.config.getBoolean ("print.empty.columns");
  std::vector <Column*> nonempty_columns;

  // Determine minimal, ideal column widths.
  std::vector <int> minimal;
  std::vector <int> ideal;

  std::vector <Column*>::iterator i;
  for (i = _columns.begin (); i != _columns.end (); ++i)
  {
    // Headers factor in to width calculations.
    unsigned int global_min = 0;
    unsigned int global_ideal = global_min;

    for (unsigned int s = 0; s < sequence.size (); ++s)
    {
      if ((int)s >= _truncate_lines && _truncate_lines != 0)
        break;

      if ((int)s >= _truncate_rows && _truncate_rows != 0)
        break;

      // Determine minimum and ideal width for this column.
      unsigned int min;
      unsigned int ideal;
      (*i)->measure (data[sequence[s]], min, ideal);

      if (min   > global_min)   global_min   = min;
      if (ideal > global_ideal) global_ideal = ideal;
    }

    if (print_empty_columns || global_min != 0)
    {
      unsigned int label_length = utf8_width ((*i)->label ());
      if (label_length > global_min)   global_min   = label_length;
      if (label_length > global_ideal) global_ideal = label_length;
      minimal.push_back (global_min);
      ideal.push_back (global_ideal);
    }

    if (! print_empty_columns && global_min != 0)
    {
      nonempty_columns.push_back (*i);
    }
  }

  if (! print_empty_columns)
    _columns = nonempty_columns;

  int all_extra = _left_margin
                + (2 * _extra_padding)
                + ((_columns.size () - 1) * _intra_padding);

  // Sum the widths.
  int sum_minimal = std::accumulate (minimal.begin (), minimal.end (), 0);
  int sum_ideal   = std::accumulate (ideal.begin (),   ideal.end (),   0);


  // Calculate final column widths.
  int overage = _width - sum_minimal - all_extra;
  context.debug (format ("ViewTask::render min={1} ideal={2} overage={3}", 
                         sum_minimal + all_extra,
                         sum_ideal + all_extra,
                         overage));

  std::vector <int> widths;

  // Ideal case.  Everything fits.
  if (_width == 0 || sum_ideal + all_extra <= _width)
  {
    widths = ideal;
  }

  // Not enough for minimum.
  else if (overage < 0)
  {
    context.error (format (_("The report has a minimum width of {1} and does not fit in the available width of {2}."), sum_minimal + all_extra, _width));
    widths = minimal;
  }

  // Perfect minimal width.
  else if (overage == 0)
  {
    widths = minimal;
  }

  // Extra space to share.
  else if (overage > 0)
  {
    widths = minimal;

    // Spread 'overage' among columns where width[i] < ideal[i]
    bool needed = true;
    while (overage && needed)
    {
      needed = false;
      for (unsigned int i = 0; i < _columns.size () && overage; ++i)
      {
        if (widths[i] < ideal[i])
        {
          ++widths[i];
          --overage;
          needed = true;
        }
      }
    }
  }

  // Compose column headers.
  unsigned int max_lines = 0;
  std::vector <std::vector <std::string> > headers;
  for (unsigned int c = 0; c < _columns.size (); ++c)
  {
    headers.push_back (std::vector <std::string> ());
    _columns[c]->renderHeader (headers[c], widths[c], _header);

    if (headers[c].size () > max_lines)
      max_lines = headers[c].size ();
  }

  // Output string.
  std::string out;
  _lines = 0;

  // Render column headers.
  std::string left_margin = std::string (_left_margin, ' ');
  std::string extra       = std::string (_extra_padding, ' ');
  std::string intra       = std::string (_intra_padding, ' ');

  std::string extra_odd   = context.color () ? _extra_odd.colorize  (extra) : extra;
  std::string extra_even  = context.color () ? _extra_even.colorize (extra) : extra;
  std::string intra_odd   = context.color () ? _intra_odd.colorize  (intra) : intra;
  std::string intra_even  = context.color () ? _intra_even.colorize (intra) : intra;

  for (unsigned int i = 0; i < max_lines; ++i)
  {
    out += left_margin + extra;

    for (unsigned int c = 0; c < _columns.size (); ++c)
    {
      if (c)
        out += intra;

      if (headers[c].size () < max_lines - i)
        out += _header.colorize (std::string (widths[c], ' '));
      else
        out += headers[c][i];
    }

    out += extra;

    // Trim right.
    out.erase (out.find_last_not_of (" ") + 1);
    out += "\n";

    // Stop if the line limit is exceeded.
    if (++_lines >= _truncate_lines && _truncate_lines != 0)
    {
      context.timer_render.stop ();
      return out;
    }
  }

  // Compose, render columns, in sequence.
  _rows = 0;
  std::vector <std::vector <std::string> > cells;
  std::vector <int>::iterator s;
  for (unsigned int s = 0; s < sequence.size (); ++s)
  {
    max_lines = 0;

    // Apply color rules to task.
    Color rule_color;
    autoColorize (data[sequence[s]], rule_color);

    // Alternate rows based on |s % 2|
    bool odd = (s % 2) ? true : false;
    Color row_color;
    if (context.color ())
    {
      row_color = odd ? _odd : _even;
      row_color.blend (rule_color);
    }

    for (unsigned int c = 0; c < _columns.size (); ++c)
    {
      cells.push_back (std::vector <std::string> ());
      _columns[c]->render (cells[c], data[sequence[s]], widths[c], row_color);

      if (cells[c].size () > max_lines)
        max_lines = cells[c].size ();
    }

    for (unsigned int i = 0; i < max_lines; ++i)
    {
      out += left_margin + (odd ? extra_odd : extra_even);

      for (unsigned int c = 0; c < _columns.size (); ++c)
      {
        if (c)
        {
          if (row_color.nontrivial ())
            out += row_color.colorize (intra);
          else
            out += (odd ? intra_odd : intra_even);
        }

        if (i < cells[c].size ())
          out += cells[c][i];
        else
          out += row_color.colorize (std::string (widths[c], ' '));
      }

      out += (odd ? extra_odd : extra_even);

      // Trim right.
      out.erase (out.find_last_not_of (" ") + 1);
      out += "\n";

      // Stop if the line limit is exceeded.
      if (++_lines >= _truncate_lines && _truncate_lines != 0)
      {
        context.timer_render.stop ();
        return out;
      }
    }

    cells.clear ();

    // Stop if the row limit is exceeded.
    if (++_rows >= _truncate_rows && _truncate_rows != 0)
    {
      context.timer_render.stop ();
      return out;
    }
  }

  context.timer_render.stop ();
  return out;
}

////////////////////////////////////////////////////////////////////////////////
