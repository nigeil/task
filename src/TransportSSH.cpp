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

#include <cmake.h>
#include <text.h>
#include <i18n.h>
#include <TransportSSH.h>
#include <util.h>

////////////////////////////////////////////////////////////////////////////////
TransportSSH::TransportSSH(const Uri& uri) : Transport(uri)
{
  _executable = "scp";
}

////////////////////////////////////////////////////////////////////////////////
void TransportSSH::send(const std::string& source)
{
  std::vector<std::string> sourcelist;
  std::vector<std::string>::const_iterator source_iter;

  if (_uri._host == "")
    throw std::string (_("When using the 'ssh' protocol, the uri must contain a hostname."));

  // cmd line is: scp [-p port] [user@]host:path
  if (_uri._port != "")
  {
    _arguments.push_back ("-P");
    _arguments.push_back (_uri._port);
  }

  if (is_filelist (source))
  {
    expand_braces (source, _uri._data, sourcelist);
    // Is there more than one source?
    // Then path has to end with a '/'
    if (sourcelist.size () > 1 && !_uri.is_directory ())
      throw format (_("The uri '{1}' does not appear to be a directory."), _uri);

    for (source_iter = sourcelist.begin (); source_iter != sourcelist.end (); ++source_iter) {
      _arguments.push_back (*source_iter);
    }
  }
  else
  {
    _arguments.push_back (source);
  }

  if (_uri._user != "")
  {
    _arguments.push_back (_uri._user + "@" + _uri._host + ":" + escape (_uri._path, ' '));
  }
  else
  {
    _arguments.push_back (_uri._host + ":" + escape (_uri._path, ' '));
  }

  if (execute ())
    throw std::string (_("ssh failed, see output above."));
}

////////////////////////////////////////////////////////////////////////////////
void TransportSSH::recv(std::string target)
{
  if (_uri._host == "")
    throw std::string (_("When using the 'ssh' protocol, the uri must contain a hostname."));

  // Is there more than one file to transfer?
  // Then target has to end with a '/'
  if (is_filelist(_uri._path) && !is_directory(target))
    throw format (_("The uri '{1}' does not appear to be a directory."), target);

  // cmd line is: scp [-p port] [user@]host:path
  if (_uri._port != "")
  {
    _arguments.push_back ("-P");
    _arguments.push_back (_uri._port);
  }

  // We do not do {} expansion of the URI, as ssh servers do that for us.

  if (_uri._user != "")
  {
    _arguments.push_back (_uri._user + "@" + _uri._host + ":" + escape (_uri._path, ' '));
  }
  else
  {
    _arguments.push_back (_uri._host + ":" + escape (_uri._path, ' '));
  }

  _arguments.push_back (target);

  if (execute ())
    throw std::string (_("ssh failed, see output above."));
}

////////////////////////////////////////////////////////////////////////////////
