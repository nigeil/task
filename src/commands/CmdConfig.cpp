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
#include <i18n.h>
#include <text.h>
#include <util.h>
#include <CmdConfig.h>

extern Context context;

////////////////////////////////////////////////////////////////////////////////
CmdConfig::CmdConfig ()
{
  _keyword     = "config";
  _usage       = "task          config [name [value | '']]";
  _description = _("Change settings in the task configuration");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdConfig::execute (std::string& output)
{
  int rc = 0;
  std::stringstream out;

  // Get the non-attribute, non-fancy command line arguments.
  std::vector <std::string> words = context.a3.extract_words ();

  // Support:
  //   task config name value    # set name to value
  //   task config name ""       # set name to blank
  //   task config name          # remove name
  if (words.size ())
  {
    std::string name = words[0];
    std::string value = "";

    if (words.size () > 1)
    {
      for (unsigned int i = 1; i < words.size (); ++i)
      {
        if (i > 1)
          value += " ";

        value += words[i];
      }
    }

    if (name != "")
    {
      bool change = false;

      // Read .taskrc (or equivalent)
      std::vector <std::string> contents;
      File::read (context.config._original_file, contents);

      // task config name value
      // task config name ""
      if (words.size () > 1)
      {
        bool found = false;
        std::vector <std::string>::iterator line;
        for (line = contents.begin (); line != contents.end (); ++line)
        {
          // If there is a comment on the line, it must follow the pattern.
          std::string::size_type comment = line->find ("#");
          std::string::size_type pos     = line->find (name + "=");

          if (pos != std::string::npos &&
              (comment == std::string::npos ||
               comment > pos))
          {
            found = true;
            if (confirm (format (_("Are you sure you want to change the value of '{1}' from '{2}' to '{3}'?"), name, context.config.get (name), value)))
            {
              if (comment != std::string::npos)
                *line = name + "=" + value + " " + line->substr (comment);
              else
                *line = name + "=" + value;

              change = true;
            }
          }
        }

        // Not found, so append instead.
        if (!found &&
            confirm (format (_("Are you sure you want to add '{1}' with a value of '{2}'?"), name, value)))
        {
          contents.push_back (name + "=" + value);
          change = true;
        }
      }

      // task config name
      else
      {
        bool found = false;
        std::vector <std::string>::iterator line;
        for (line = contents.begin (); line != contents.end (); ++line)
        {
          // If there is a comment on the line, it must follow the pattern.
          std::string::size_type comment = line->find ("#");
          std::string::size_type pos     = line->find (name + "=");

          if (pos != std::string::npos &&
              (comment == std::string::npos ||
               comment > pos))
          {
            found = true;

            // Remove name
            if (confirm (format (_("Are you sure you want to remove '{1}'?"), name)))
            {
              *line = "";
              change = true;
            }
          }
        }

        if (!found)
          throw format (_("No entry named '{1}' found."), name);
      }

      // Write .taskrc (or equivalent)
      if (change)
      {
        File::write (context.config._original_file, contents);
        out << format (_("Config file {1} modified."),
                       context.config._original_file._data)
            << "\n";
      }
      else
        out << _("No changes made.") << "\n";
    }
    else
      throw std::string (_("Specify the name of a config variable to modify."));

    output = out.str ();
  }
  else
    throw std::string (_("Specify the name of a config variable to modify."));

  return rc;
}

////////////////////////////////////////////////////////////////////////////////
CmdCompletionConfig::CmdCompletionConfig ()
{
  _keyword     = "_config";
  _usage       = "task          _config";
  _description = _("Lists all supported configuration variables, for completion purposes");
  _read_only   = true;
  _displays_id = false;
}

////////////////////////////////////////////////////////////////////////////////
int CmdCompletionConfig::execute (std::string& output)
{
  std::vector <std::string> configs;
  context.config.all (configs);
  std::sort (configs.begin (), configs.end ());

  std::vector <std::string>::iterator config;
  for (config = configs.begin (); config != configs.end (); ++config)
    output += *config + "\n";

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
