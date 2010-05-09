#! /usr/bin/perl
################################################################################
## task - a command line task list manager.
##
## Copyright 2006 - 2010, Paul Beckingham.
## All rights reserved.
##
## This program is free software; you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free Software
## Foundation; either version 2 of the License, or (at your option) any later
## version.
##
## This program is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
## FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
## details.
##
## You should have received a copy of the GNU General Public License along with
## this program; if not, write to the
##
##     Free Software Foundation, Inc.,
##     51 Franklin Street, Fifth Floor,
##     Boston, MA
##     02110-1301
##     USA
##
################################################################################

use strict;
use warnings;
use Test::More tests => 7;

# Create the rc file.
if (open my $fh, '>', 'hook.rc')
{
  print $fh "data.location=.\n",
            "report.long.columns=id,project,priority,entry,start_time,due,recur,countdown,age,tags,description\n",
            "hooks=on\n",
            "hook.format-start_time=" . $ENV{'PWD'} . "/hook:start_time\n";
  close $fh;
  ok (-r 'hook.rc', 'Created hook.rc');
}

# Create the hook functions.
if (open my $fh, '>', 'hook')
{
  print $fh "function start_time (name, value)\n",
            "  value = '<' .. value .. '>'\n",
            "  return value, 0, nil\n",
            "end\n";
  close $fh;
  ok (-r 'hook', 'Created hook');
}

my $output = qx{../task rc:hook.rc version};
if ($output =~ /PUC-Rio/)
{
  qx{../task rc:hook.rc add foo};
  qx{../task rc:hook.rc start 1};
  $output = qx{../task rc:hook.rc long};

  like ($output, qr/<\d+\/\d+\/\d+\s\d+:\d+:\d+>/, 'format-start_time hook start_time -> <start_time>');
}
else
{
  pass ('format-start_time hook start_time -> <start_time> - skip: no Lua support');
}

# Cleanup.
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

unlink 'undo.data';
ok (!-r 'undo.data', 'Removed undo.data');

unlink 'hook';
ok (!-r 'hook', 'Removed hook');

unlink 'hook.rc';
ok (!-r 'hook.rc', 'Removed hook.rc');

exit 0;
