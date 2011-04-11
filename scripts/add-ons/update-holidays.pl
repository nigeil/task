#! /usr/bin/perl
################################################################################
## taskwarrior - a command line task list manager.
##
## Copyright 2006 - 2011, Paul Beckingham, Federico Hernandez.
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
use Getopt::Long;

# Give a nice error if the (non-standard) JSON module is not installed.
eval "use JSON";
if ($@)
{
  print "Error: You need to install the JSON Perl module.\n";
  exit 1;
}

eval "use LWP::Simple";
if ($@)
{
  print "Error: You need to install the LWP::Simple Perl module.\n";
  exit 1;
}

# Command line options, argument validation.
my $help;
my $locale;
my @regions;
my $file;
Getopt::Long::GetOptions (
  'help'     => \$help,
  'locale=s' => \$locale,
  'region=s' => \@regions,
  'file=s'   => \$file);

if ($help || !$file)
{
  print <<EOF;

usage: update-holidays.pl [--help]
                          [--locale <locale>]
                          [--region <region>]
                          --file <holiday-file>

  --help         Displays this message.
  --locale       Locale to be used, if it cannot be derived from the file name.
  --region       Regional data.  May be specified more than once.
  --file         Location of the holiday file to update.
                 Note: this file *will* be overwritten.

Typical usage is to simply specify a locale, like this:

  ./update-holidays.pl --locale en-US --file holidays.en-US.rc

This will give you all holidays for the locale.  If you want data for a locale
that has regional data, you may want to specify regions, to make sure you get
the data local to your region.  For example, the following command will download
Australian holiday data that is either national, or specific to the New South
Wales territory.

  update-holidays.pl --locale en-AU \
                     --region NSW \
                     --file holidays.en-AU.rc

Multiple regions may be specified, such as the two Swiss cantons of Zürich and
Schwyz:

  update-holidays.pl --locale de-CH \
                     --region Zürich \
                     --region Schwyz \
                     --file holidays.de-CH.rc

See http://holidata.net for details of supported locales and regions.

It is recommended that you regularly update your holiday files.  Not only does
this keep your holiday data current, but allows for corrected data to be used.

EOF

  exit 1;
}

# File name is required.
die "You must specify a holiday file to update.\n"  if ! $file;

# Convert @regions to %region_hash to simplify lookup.
my %region_hash = map {$_ => undef} @regions;

# Perhaps the locale can be found in the file name, if not already specified?
if (!$locale &&
    $file =~ /([a-z]{2}-[A-Z]{2})/)
{
  $locale = $1;
}

die "The locale could not be determined from the file name.  " .
    "Please secify a locale.\n"  if !$locale;

# Determine the current year.
my $current = (localtime (time))[5] + 1900;
my $next    = $current + 1;

# Construct the holidata.net URL.
my $url_current = "http://holidata.net/${locale}/${current}.json";
my $url_next    = "http://holidata.net/${locale}/${next}.json";

# Fetch data for the current year.
my $data_current = get ($url_current);
print "\n",
      "Data for ${locale}, for ${current} could not be downloaded.  This could\n",
      "mean that you do not have an internet connection, or that\n",
      "Holidata.net does not support this locale and/or year.\n",
      "\n"
  unless defined $data_current;

# Fetch data for the next year.
my $data_next = get ($url_next);
print "\n",
      "Data for ${locale}, for ${next} could not be downloaded.  This could\n",
      "mean that you do not have an internet connection, or that\n",
      "Holidata.net does not support this locale and/or year.\n",
      "\n"
  unless defined $data_next;

# Without data, cannot proceed.
my $data;
$data .= $data_current if defined $data_current;
$data .= $data_next    if defined $data_next;
exit (1) if !defined $data || $data eq '';

# Filter the holidays according to @regions.
my $id = 1;
my $content;
for my $holiday (split /\n/ms, $data)
{
  my $parsed = from_json ($holiday);

  if (@regions == 0 ||
      (@regions > 0 && ($parsed->{'region'} eq '' ||
                        exists $region_hash{$parsed->{'region'}})))
  {
    $content .= "holiday.${locale}${id}.name=" . $parsed->{'description'} .  "\n" .
                "holiday.${locale}${id}.date=" . $parsed->{'date'} .         "\n";
  }

  ++$id;
}

# Overwrite the file.
if (open my $fh, '>', $file)
#if (open my $fh, '>:encoding(UTF-8)', $file)
{
  print $fh
        "# International Holiday Data provided by Holidata.net\n",
        "#\n",
        "# ${url_current}\n",
        "#\n",
        $content;

  close $fh;
}

exit 0;

################################################################################
