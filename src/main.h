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

#ifndef INCLUDED_MAIN
#define INCLUDED_MAIN

#define FEATURE_COLOR  1                               // Enable color.
//#define FEATURE_STDIN  1                             // Enable reading stdin.

#include <string>
#include <vector>
#include <map>
#include <sys/types.h>
#include <Context.h>
#include <Date.h>
#include <Color.h>

// recur.cpp
void handleRecurrence ();
Date getNextRecurrence (Date&, std::string&);
bool generateDueDates (Task&, std::vector <Date>&);
void updateRecurrenceMask (Task&);
int getDueState (const std::string&);
int getDueState (const Date&);
bool nag (Task&);

// rules.cpp
void initializeColorRules ();
void autoColorize (Task&, Color&);
std::string colorizeHeader (const std::string&);
std::string colorizeFootnote (const std::string&);
std::string colorizeError (const std::string&);
std::string colorizeDebug (const std::string&);

// dependency.cpp
void dependencyGetBlocked (const Task&, std::vector <Task>&);
void dependencyGetBlocking (const Task&, std::vector <Task>&);
bool dependencyIsCircular (const Task&);
void dependencyChainOnComplete (Task&);
void dependencyChainOnStart (Task&);
void dependencyChainOnModify (Task&, Task&);

// feedback.cpp
bool taskDiff (const Task&, const Task&);
std::string taskDifferences (const Task&, const Task&);
std::string taskInfoDifferences (const Task&, const Task&, const std::string&, long&, const long);
std::string renderAttribute (const std::string&, const std::string&, const std::string& format = "");
void feedback_affected (const std::string&);
void feedback_affected (const std::string&, int);
void feedback_affected (const std::string&, const Task&);
void feedback_special_tags (const Task&, const std::string&);
void feedback_unblocked (const Task&);
void feedback_backlog ();
std::string onProjectChange (Task&, bool scope = true);
std::string onProjectChange (Task&, Task&);
std::string onExpiration (Task&);

// sort.cpp
void sort_tasks (std::vector <Task>&, std::vector <int>&, const std::string&);

// legacy.cpp
void legacyAttributeCheck (const std::string&);
void legacyColumnMap (std::string&);
void legacySortColumnMap (std::string&);
std::string legacyCheckForDeprecatedColor ();
std::string legacyCheckForDeprecatedVariables ();
std::string legacyCheckForDeprecatedColumns ();

// list template
///////////////////////////////////////////////////////////////////////////////
template <class T> bool listDiff (const T& left, const T& right)
{
  if (left.size () != right.size ())
    return true;

  for (unsigned int i = 0; i < left.size (); ++i)
    if (left[i] != right[i])
      return true;

  return false;
}

///////////////////////////////////////////////////////////////////////////////
template <class T> void listDiff (
  const T& left, const T& right, T& leftOnly, T& rightOnly)
{
  leftOnly.clear ();
  rightOnly.clear ();

  for (unsigned int l = 0; l < left.size (); ++l)
  {
    bool found = false;
    for (unsigned int r = 0; r < right.size (); ++r)
    {
      if (left[l] == right[r])
      {
        found = true;
        break;
      }
    }

    if (!found)
      leftOnly.push_back (left[l]);
  }

  for (unsigned int r = 0; r < right.size (); ++r)
  {
    bool found = false;
    for (unsigned int l = 0; l < left.size (); ++l)
    {
      if (left[l] == right[r])
      {
        found = true;
        break;
      }
    }

    if (!found)
      rightOnly.push_back (right[r]);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <class T> void listIntersect (const T& left, const T& right, T& join)
{
  join.clear ();

  for (unsigned int l = 0; l < left.size (); ++l)
  {
    for (unsigned int r = 0; r < right.size (); ++r)
    {
      if (left[l] == right[r])
      {
        join.push_back (left[l]);
        break;
      }
    }
  }
}

#endif
////////////////////////////////////////////////////////////////////////////////
