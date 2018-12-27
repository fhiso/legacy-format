#!/usr/bin/perl

use strict;
use warnings;

sub CalEsc { return '@#D[A-Z][A-Z ]*@' }
sub S      { return '[ \t\r\n]+' }
sub Sq     { return '[ \t\r\n]*' }
sub Day    { return '[0-9]+' }
sub Month  { return '[A-Z][A-Z0-9][A-Z0-9]+' }
sub Year   { return '-?[0-9]+(/-?[0-9]+)?' }
sub Epoch  { return '([A-Z]([A-Z]|[A-Z0-9._]*[._][A-Z0-9._]*)'
                    . '|\$[^ #x9#xA#xD]+)' }

sub Date   { return '(' . CalEsc . S . ')?'
                    . '((' . Day . S . ')?' . Month . S . ')?'
                    . Year . '(' . Sq . Epoch ')?' }

sub Modif  { return '((ABT|CAL|EST|BEF|AFT|TO)' . S . ')?' . Date }
sub Phrase { return '[^\r\n()]*' }
sub Interp { return '(INT' . S . Date . S . '?)' . '\(' . Phrase . '\)' }
sub Range  { return 'BET' . S . Date . S . 'AND' . S . Date }
sub Period { return 'FROM' . S . Date . '(' . S . 'TO' . S . Date . ')?' }

sub Value  { return  Modif . '|' . Interp . '|' . Range . '|' . Period }

my $md = 0;
if ($ARGV[0] eq '--markdown') {
  $md = 1; shift;
}

no strict qw/refs/;
$_ = &{$ARGV[0]};

if ($md) {
  while (s/^(.{70,90}[\)\]][*+?]?)//) {
    print "$&\n";
  }
}

print "$_\n";

