////////////////////////////////////////////////////////////////////////////////
// taskwarrior - a command line task list manager.
//
// Copyright 2010 - 2013, Johannes Schlatow.
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

#include <text.h>
#include <i18n.h>
#include <TransportShell.h>
#include <util.h>

////////////////////////////////////////////////////////////////////////////////
TransportShell::TransportShell(const Uri& uri) : Transport(uri)
{
  _executable = "cp";
}

////////////////////////////////////////////////////////////////////////////////
void TransportShell::send(const std::string& source)
{
  std::vector<std::string> sourcelist;
  std::vector<std::string>::const_iterator source_iter;

  if (_uri._path == "")
    throw std::string (_("When using the 'sh+cp' protocol to copy multiple files, a path must be specified."));

  if (is_filelist(source))
  {
    expand_braces (source, _uri._path, sourcelist);
    // Is there more than one file to transfer?
    // Then path has to end with a '/'
    if (sourcelist.size() > 1 && !_uri.is_directory())
      throw format (_("The uri '{1}' does not appear to be a directory."), _uri._path);
    for (source_iter = sourcelist.begin (); source_iter != sourcelist.end (); ++source_iter)
      _arguments.push_back (*source_iter);
  }
  else
  {
    _arguments.push_back (source);
  }
  _arguments.push_back (_uri._path);

  if (execute ())
    throw std::string (_("shell command failed, see output above."));
}

////////////////////////////////////////////////////////////////////////////////
void TransportShell::recv(std::string target)
{
  std::vector<std::string> paths;
  std::vector<std::string>::const_iterator paths_iter;

  if (_uri._path == "")
    throw std::string (_("When using the 'sh+cp' protocol to copy multiple files, a path must be specified."));

  if (is_filelist(_uri._path))
  {
    expand_braces (_uri._path, target, paths);

    // Is there more than one file to transfer?
    // Then target has to end with a '/'
    if (paths.size() > 1 && !is_directory(target))
      throw format (_("The uri '{1}' does not appear to be a directory."), target);

    for (paths_iter = paths.begin (); paths_iter != paths.end (); ++paths_iter)
      _arguments.push_back (*paths_iter);
  }
  else
  {
    _arguments.push_back (_uri._path);
  }

  _arguments.push_back (target);

  if (execute ())
    throw std::string (_("shell command failed, see output above."));
}

////////////////////////////////////////////////////////////////////////////////
