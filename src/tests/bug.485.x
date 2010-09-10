#! /usr/bin/perl
################################################################################
## taskwarrior - a command line task list manager.
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
use Test::More tests => 9;

# Create the rc file.
if (open my $fh, '>', 'bug.rc')
{
  print $fh "data.location=.\n";

  close $fh;
  ok (-r 'bug.rc', 'Created bug.rc');
}

# Bug #485 - 'task list recur:month' doesn't list monthly tasks
qx{../task rc:bug.rc add one due:tomorrow recur:monthly};
qx{../task rc:bug.rc add two due:tomorrow recur:month};
my $output = qx{../task rc:bug.rc list recur:monthly};
like ($output, qr/one/, 'monthly -> monthly');
like ($output, qr/two/, 'month   -> monthly');

$output = qx{../task rc:bug.rc list recur:month};
like ($output, qr/one/, 'monthly -> month');
like ($output, qr/two/, 'month   -> month');

# Cleanup.
unlink 'pending.data';
ok (!-r 'pending.data', 'Removed pending.data');

unlink 'completed.data';
ok (!-r 'completed.data', 'Removed completed.data');

unlink 'undo.data';
ok (!-r 'undo.data', 'Removed undo.data');

unlink 'bug.rc';
ok (!-r 'bug.rc', 'Removed bug.rc');

exit 0;