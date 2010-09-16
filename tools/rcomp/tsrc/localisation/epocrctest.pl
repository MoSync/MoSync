#
# Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of the License "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: 
#

use strict;
use warnings;
use Cwd;		# for cwd
use FindBin;		# for FindBin::Bin
use File::Copy;		# for copy()

my $curdrive="x";	    	# will be initialised when first needed
my $PerlBinPath;	# fully qualified pathname of the directory containing this script

# establish the path to the Perl binaries
BEGIN {
	require 5.005_03;		# check user has a version of perl that will cope
	$PerlBinPath = $FindBin::Bin;	# X:/epoc32/tools
	$PerlBinPath =~ s/\//\\/g;	# X:\epoc32\tools
}
use lib $PerlBinPath;

print "$PerlBinPath\n";

open EPOCRC_INPUT, "epocrc_input.txt";
if(!(-d "test-data"))
{
	mkdir("test-data") || die "Cannot mkdir test-data: $!";
}
my $file_line;
my $state = 0;
my $line_number = 0;
my $rss_number="";
my $expected_stdout_file = "";
my $expected_stderr_file = "";
my $rls_file;
my $rss_file;
my @extra_args;

while($file_line = <EPOCRC_INPUT>)
{
	$line_number++;
	if($state == 0)
	{
		if($file_line=~/^\s*\+\+\+\+\s*$/)
		{ $state = 1;	next; 
		}
		elsif($file_line=~/^\s*$/)
		{ next; # do nothing as blank line
		}
		else 
		{ 	print "Error on line number $line_number.\n";
			exit(1); 
		}
	}
	if($state == 1)
	{
		if($file_line=~/^\s*\*{4}Number:\s*(\d+)\s*$/)
		{ $state = 2;	
			$rss_number = "$1";
			if(!(-d  "$PerlBinPath\\test-data\\$rss_number"))
			{
				mkdir "$PerlBinPath\\test-data\\$rss_number";
			}
			next; 
		}
		elsif($file_line=~/^\s*$/)
		{ next; # do nothing as blank line
		}
		else 
		{ 	print "Error on line number $line_number.\n";
			exit(1);
		}
	}
	if($state == 2)
	{
		if($file_line=~/^\s*\*{4}Extra args:(.*)$/)
		{	
			$state = 3;
			if($1 ne "")
			{
				push @extra_args, $1;
			}
			else
			{
				push @extra_args, "";
			}
			next;
		}
	}
	if($state == 3)
	{
		if($file_line=~/^\s*\*{4}Expected stdout:\s*$/)
		{ 
			$state = 4;
			close RLS_FILE;
			$expected_stdout_file = "$PerlBinPath\\test-data\\$rss_number\\expected.stdout";
			open STDOUT_FILE, ">$expected_stdout_file";
			next; 
		}
		elsif($file_line=~/^\s*$/)
		{ 
			next; # do nothing as blank line
		}
		else 
		{ 	
			print "Error on line number $line_number.\n";
			exit(1);
		}
	}
	if($state == 4)
	{
		if($file_line=~/^\s*\*{4}Expected stderr:\s*$/)
		{ $state = 5;	
			close STDOUT_FILE;
			$expected_stderr_file = "$PerlBinPath\\test-data\\$rss_number\\expected.stderr";
			open STDERR_FILE, ">$expected_stderr_file";
			next; 
		}
		else 
		{ print STDOUT_FILE $file_line;
		}
	}
	if($state == 5)
	{
		if($file_line=~/^\s*\*{4}rss data:\s*$/)
		{ $state = 6;	
			close STDERR_FILE;
			$rss_file = "$PerlBinPath\\test-data\\$rss_number\\test$rss_number\.rss";
			open RSS_FILE, ">$rss_file";
			next; 
		}
		else 
		{ print STDERR_FILE $file_line;
		}
	}
	if($state == 6)
	{
		if($file_line=~/^\s*\*{4}rls data:\s*$/)
		{ $state = 7;	
			close RSS_FILE;
			$rls_file = "$PerlBinPath\\test-data\\$rss_number\\test$rss_number\.rls";
			open RLS_FILE, ">$rls_file";
			next; 
		}
		else 
		{ print RSS_FILE $file_line;
		}
	}	
	if($state == 7)
	{
		if($file_line=~/^\s*----\s*$/)
		{ $state = 0;	
			close RLS_FILE;
			next; 
		}
		else 
		{ print RLS_FILE $file_line;
		}
	}
}
close RLS_FILE;



if($state)
{ 
	print "Error: end of file reached while still processing.\n";
	exit(1);
}

my $test_directory = "$PerlBinPath\\test-data\\";
my $rsc_directory = "$PerlBinPath\\rsc-files\\";

print $test_directory . "\n";
my $epocrc = "";
opendir DH, $test_directory or die "Couldn't open the test directory: $!";
my $folder;
while ($folder = readdir(DH))
{
	if($folder ne "\." && $folder ne "\.\.")
	{
		print "\n---$folder---\n";
		my $this_test_dir = "$test_directory$folder\\";
		my $test_name = $this_test_dir. "test$folder.rss";
		my $rcomp = "rcomp";

		if(!(-e $test_name))
		{
			print "Skipping $test_name, cannot find file.\n";
		}
		else
		{
			my $file_line;	
			open EPOCRC_TEST, ">epocrc.test";
			print EPOCRC_TEST "RCOMP: $rcomp\n";
			print EPOCRC_TEST "TEST-NUMBER: $folder\n";
			print EPOCRC_TEST "TEST-DIR: $this_test_dir\n";
			close EPOCRC_TEST;
			$epocrc = "epocrc $test_name -o$this_test_dir" . "test$folder\.rsc -h$this_test_dir" . "test$folder\.rsg -t$this_test_dir -v -u -la -epocrc-test $extra_args[$folder-1]";
			print "$epocrc\n";
			system($epocrc);
		}
	}
}

	
# this stage will compare the stderr files of each test to check the output is as expected.
# empty lines are ignored but the order in which the errors occur are kept to as these should be the same
# in both files.
opendir DH, $test_directory or die "Couldn't open the test directory: $!";
print "\n\nComparing stderr files\n\n";
while ($folder = readdir(DH))
{
	if($folder ne "\." && $folder ne "\.\.")
	{
		print "\n---$folder---\n";
		my $this_test_dir = "$test_directory$folder\\";
		if(!(-e $this_test_dir . "expected.stderr"))
		{
			print "Could not find " . $this_test_dir . "expected.stderr\n";
			next;
		}
		if(!(-e $this_test_dir . "rcomp.stderr"))
		{
			print "Could not find " . $this_test_dir . "rcomp.stderr\n";
			next;
		}	

		# read in the lines from the expected output ignoring whitespace only lines
		my @expected_values;
		my $file_line;
		open EXPECTED, $this_test_dir . "expected.stderr";
		while($file_line = <EXPECTED>)
		{
			if($file_line!~/^\s*$/)
			{
				push @expected_values, $file_line;
			}
		}
		close EXPECTED;

		# read in the lines from the actual output ignoring whitespace only lines
		my @actual_values;
		open ACTUAL, $this_test_dir . "rcomp.stderr";
		while($file_line = <ACTUAL>)
		{
			if($file_line!~/^\s*$/)
			{
				push @actual_values, $file_line;
			}
		}
		close ACTUAL;

		if((scalar @expected_values) != (scalar @actual_values))
		{
			print "In " . $this_test_dir . ", actual and expected stderr have different sizes\n";
			next;
		}
		my $expected_file;
		my $expected_line_number;
		my $expected_type;
		my $expected_warning_number;
		my $expected_non_loc_warning;
		my $actual_file;
		my $actual_line_number;
		my $actual_type;
		my $actual_warning_number;
		my $actual_non_loc_warning;

		for( my $i = 0 ; $i < scalar @expected_values ; $i++)
		{
			$expected_file = "";
			$expected_line_number = "";
			$expected_type = "";
			$expected_warning_number = "";
			$expected_non_loc_warning = "";
			$actual_file = "";
			$actual_line_number = "";
			$actual_type = "";
			$actual_warning_number = "";
			$actual_non_loc_warning = "";

			if($expected_values[$i]=~/^\s*\S+(test-data\S+)\((\d+)\)\s*\:\s*(Warning:)\s*\((\d+)\).*$/)
			{
				$expected_file = $1;
				$expected_line_number = $2;
				$expected_type = $3;
				$expected_warning_number = $4;
			}
			else
			{
				$expected_non_loc_warning = $expected_values[$i];
			}
			if($actual_values[$i]=~/^\s*\S+(test-data\S+)\((\d+)\)\s*\:\s*(Warning:)\s*\((\d+)\).*$/)
			{
				$actual_file = $1;
				$actual_line_number = $2;
				$actual_type = $3;
				$actual_warning_number = $4;
			}
			else
			{
				$actual_non_loc_warning = $actual_values[$i];
			}

			if($actual_non_loc_warning ne "")
			{
				if(	$actual_non_loc_warning != $expected_non_loc_warning )
				{
					print "In " . $this_test_dir . ", $actual_non_loc_warning doesn't match $expected_non_loc_warning\n"; 
					
				}
			}
			else
			{
				if( $actual_file ne $expected_file)
				{
					print "In " . $this_test_dir . ", $expected_values[$i] doens't have a matching file name.\n"; 
					print "Actual: $actual_file, Expected: $expected_file\n";
				}
				elsif( $actual_line_number != $expected_line_number )
				{
					print "In " . $this_test_dir . ", $expected_values[$i] doens't have a matching line number.\n"; 
				}
				elsif( $actual_type ne $expected_type )
				{
					print "In " . $this_test_dir . ", $expected_values[$i] doens't have a matching warning type.\n"; 
				}
				elsif( $actual_warning_number ne $expected_warning_number )
				{
					print "In " . $this_test_dir . ", $expected_values[$i] doens't have a matching warning number.\n"; 
				}
			}
		}
	}
}

close DH;

# this stage will compare the stdout files of each test to check the output is as expected.
# empty lines are ignored but the order in which the output occur are kept to as these should be the same
# in both files.
opendir DH, $test_directory or die "Couldn't open the test directory: $!";

print "\n\nComparing stdout files\n\n";
while ($folder = readdir(DH))
{
	if($folder ne "\." && $folder ne "\.\.")
	{
		print "\n---$folder---\n";
		my $this_test_dir = "$test_directory$folder\\";
		if(!(-e $this_test_dir . "expected.stdout"))
		{
			print "Could not find " . $this_test_dir . "expected.stdout\n";
			next;
		}
		if(!(-e $this_test_dir . "rcomp.stdout"))
		{
			print "Could not find " . $this_test_dir . "rcomp.stdout\n";
			next;
		}	

		# read in the lines from the expected output ignoring whitespace only lines
		my @expected_values;
		my $file_line;
		open EXPECTED, $this_test_dir . "expected.stdout";
		while($file_line = <EXPECTED>)
		{
			if($file_line=~/^uidcrc\.exe (0x\S{8}) (0x\S{8}) (0x\S{8}) .*test-data(.*)$/)
			{
				push @expected_values, "$1,$2,$3,$4";
			}
			elsif($file_line!~/^\s*$/)
			{
				push @expected_values, $file_line;
			}
		}
		close EXPECTED;

		# read in the lines from the actual output ignoring whitespace only lines
		my @actual_values;
		open ACTUAL, $this_test_dir . "rcomp.stdout";
		while($file_line = <ACTUAL>)
		{
			if($file_line=~/^uidcrc\.exe (0x\S{8}) (0x\S{8}) (0x\S{8}) .*test-data(.*)$/)
			{
				push @actual_values, "$1,$2,$3,$4";
			}
			elsif($file_line!~/^\s*$/)
			{
				push @actual_values, $file_line;
			}
		}
		close ACTUAL;

		if((scalar @expected_values) != (scalar @actual_values))
		{
			print "In " . $this_test_dir . ", actual and expected stdout have different sizes\n";
			next;
		}

		for( my $i = 0 ; $i < scalar @expected_values ; $i++)
		{
			if($expected_values[$i] ne $actual_values[$i])
			{
				print "stdout files differ\n";
				next;
			}
		}
	}
}

close DH;

# this stage will compare the compiled files of each test to check the output is as expected.
opendir DH, $test_directory or die "Couldn't open the test directory: $!";

print "\n\nComparing compiled files\n\n";
while ($folder = readdir(DH))
{
	if($folder ne "\." && $folder ne "\.\.")
	{
		print "\n---$folder---\n";
		my $controlFile = $rsc_directory . "test$folder\.rsc";
		my $producedFile = $test_directory . $folder . "\\test$folder\.rsc";
		my $testOutput = `diff $controlFile $producedFile --binary -s`;
		my $differOutput = "Binary files $controlFile and $producedFile differ\n";
		my $identicalOutput = "Files $controlFile and $producedFile are identical\n";
		if($testOutput eq $identicalOutput)
		{
			# do nothing, blank output implies success
		}
		elsif($testOutput eq $differOutput)
		{
			print "$testOutput";
		}
		else
		{
			print "Unrecognised output: $testOutput";
		}
	}
}

close DH;

unlink "epocrc.test";
