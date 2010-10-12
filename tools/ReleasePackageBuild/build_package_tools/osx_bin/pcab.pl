#!/usr/bin/perl -w
# Build an installable Pocket PC cabinet file.
# Copyright 2006 Shaun Jackman


#use strict;
use Getopt::Long;
use Pod::Usage;
use Text::ParseWords;
use MIME::Base64;


# Constants.
my $architecture = 0;
my @version_requirements = (4, 0, 6, 99, 0, -536870912); # 4.0.0 to 6.99.0xE0000000 (-536870912)
my $verbose = 0;

# Strings.
my $string_count = 0;
my %string_ids;

# Directories.
my $directory_count = 0;
my %directory_ids;
my @directories;

# Files.
my $file_count = 0;
my %file_ids;
my %file_dirs;
my @files;

# RegHives.
my $reghive_count = 0;
my %reghive_ids;
my @reghive_roots;
my @reghives;

# RegKeys.
my $regkey_count = 0;
my %regkey_ids;
my @regkey_hives;
my @regkey_keys;
my @regkey_names;
my @regkey_values;
my @regkey_xmlvalues;
my @regkey_types;
my @regkey_typeids;

# Links.
my $links_count = 0;
my %links;
my %link_targets;
my %link_dirs;


# Returns the ID of the specified string.
sub get_string_id($)
{
        my $string = shift;
        my $id = $string_ids{$string};
        if( defined $id) {
                return $id;
        } else {
                $id = ++$string_count;
                $string_ids{$string} = $id;
                return $id;
        }
}


# Returns the ID of the specified directory.
sub get_directory_id($)
{
        my $directory = shift;
        my $id = $directory_ids{$directory};
        if( defined $id) {
                return $id;
        } else {
                $id = ++$directory_count;
                $directory_ids{$directory} = $id;

                my @strings = split '/', $directory;
                my @ids;
                foreach my $string (@strings) {
                        next if length $string == 0;
                        push @ids, get_string_id( $string);
                }
                push @ids, 0;
                $directories[$id] = "\...@ids";

                return $id;
        }
}


# Returns the ID of the specified registry key
sub get_reghive_id($$)
{
        my $root = shift;
        my $key = shift;
        my $hive_name = "$root/$key";
        my $id = $reghive_ids{$hive_name};
        if( defined $id) {
                return $id;
        } else {
                $id = ++$reghive_count;
                $reghive_ids{$hive_name} = $id;

                my @strings = split '/', $key;
                my @ids;
                foreach my $string (@strings) {
                        next if length $string == 0;
                        push @ids, get_string_id( $string);
                }
                push @ids, 0;
                $reghives[$id] = "\...@ids";
                $reghive_roots[$id] = $root;

                return $id;
        }
}

sub parseint($)
{
        my $str = shift;
        if ($str =~ /^0x/i) {
                return hex($str);
        } elsif ($str =~ /^0b/i) {
                return oct($str);
        } else {
                return int($str);
        }
}


# Returns a list of keys sorted by value.
sub get_value_sorted_keys(%)
{
        my %hash = @_;
        return sort { $hash{$a} <=> $hash{$b} } keys %hash;
}


# Creates the files.
sub parse_input($)
{
        my $destdir = shift;
        while(<>) {
                my ($file, $directory, $linkname, $linkdir) = &shellwords($_);
                next if length $directory == 0;

                my $id = ++$file_count;
                $file_ids{$file} = $id;
                $file_dirs{$file} = "$destdir$directory";
                $files[$id] = get_directory_id "$destdir$directory";


                if (defined $linkdir) {
                        $linkname .= '.lnk';

                        $link_targets{$linkname} = "$destdir$directory/$file";
                        $link_dirs{$linkname} = $linkdir;

                        my $linknameid = get_string_id $linkname;
                        my $dirid = 0;
                        my $basedir = 0;
                        if ($linkdir =~ /^%CE(\d+)%$/) {
                                $basedir = $1;
                        } else {
                                $dirid = get_directory_id $linkdir;
                        }

                        my @strings = split '/', $linkname;
                        my @stringids;
                        foreach my $string (@strings) {
                                next if length $string == 0;
                                push @stringids, get_string_id $string;
                        }

                        my $fileid = $id;
                        my $id = ++$links_count;
                        my $type = 1;
                        $links{$id} = [ $dirid, $basedir, $fileid, 1, [ @stringids ] ];
                }
        }
}


sub parse_regfile($)
{
        my $regfile = shift;
        open REGFILE, "<$regfile";
        while(<REGFILE>) {
                my @line = &shellwords($_);
                my $root = shift @line;
                my $key = shift @line;
                my $name = shift @line;
                my $type = shift @line;
                my $value = shift @line;
                next if length $root == 0;

                $root = uc($root);
                my $rootid;
                if ($root eq "HKCR") {
                        $rootid = 1;
                } elsif ($root eq "HKCU") {
                        $rootid = 2;
                } elsif ($root eq "HKLM") {
                        $rootid = 3;
                } elsif ($root eq "HKU") {
                        $rootid = 4;
                } else {
                        print STDERR "Unrecognised registry root $root\n";
                        next;
                }
                $type = lc($type);
                my $manifest_value = '';
                my $xml_value = '';
                my $typeid;
                if ($type eq "string") {
                        $manifest_value = $value . "\0";
                        $xml_value = $value;
                        $typeid = 0x00000000;
                        $type = "string";
                } elsif ($type eq "binary") {
                        $manifest_value = '';
                        while (length $value > 0) {
                                my $byte = hex(substr $value,0,2,'');
                                $manifest_value .= pack 'C',$byte;
                        }
                        $xml_value = encode_base64($manifest_value);
                        chomp $xml_value;
                        $typeid = 0x00000001;
                        $type = "binary";
                } elsif ($type eq "integer" || $type eq "int" || $type eq "dword") {
                        my $int = parseint($value);
                        $manifest_value = pack 'V', $int;
                        $xml_value = "$int";
                        $typeid = 0x00010001;
                        $type = "integer";
                } elsif ($type eq "multistring") {
                        my @strings;
                        push @strings, $value if defined $value;
                        push @strings, @line;
                        $manifest_value = '';
                        $xml_value = '';
                        foreach my $string (@strings) {
                                $manifest_value .= $string . "\0";
                                $xml_value .= $string . "\xEF\x80\x80"; # separate using EF8080, which is 0xF000 encoded as utf8
                        }
                        $manifest_value .= "\0";
                        $typeid = 0x00010000;
                        $type = "multiplestring";
                } else {
                        print STDERR "Unrecognised registry type $type\n";
                        next;
                }
                my $id = ++$regkey_count;
                $regkey_ids{$id} = $id;
                $regkey_hives[$id] = get_reghive_id $rootid, $key;
                $regkey_keys[$id] = "$root/$key";
                $regkey_names[$id] = $name;
                $regkey_values[$id] = $manifest_value;
                $regkey_xmlvalues[$id] = $xml_value;
                $regkey_typeids[$id] = $typeid;
                $regkey_types[$id] = $type;
        }
        close REGFILE;
}


# Returns the entire manifest.
sub get_manifest($$)
{
        my ($provider, $application) = @_;

# Header.
        my $offset = 100;

# Application.
        my $application_offset = $offset;
        $application .= "\0";
        while ((length $application) % 4 != 0) {
                $application .= "\0";
        }
        $offset += length $application;

# Provider.
        my $provider_offset = $offset;
        $provider .= "\0";
        while ((length $provider) % 4 != 0) {
                $provider .= "\0";
        }
        $offset += length $provider;

# Unsupported platforms.
        my $unsupported_offset = $offset;
        my $unsupported = '';
        $offset += length $unsupported;

# Strings.
        my $strings_offset = $offset;
        my $strings = '';
        foreach my $string (get_value_sorted_keys %string_ids) {
                my $string_id = $string_ids{$string};
                print "$string($string_id)\n" if $verbose;
                $string .= "\0";
                while ((length $string) % 4 != 0) {
                        $string .= "\0";
                }
                $strings .= pack 'vv', $string_id, length $string;
                $strings .= $string;
        }
        $offset += length $strings;

# Directories.
        my $directories_offset = $offset;
        my $directories = '';
        foreach my $directory (get_value_sorted_keys %directory_ids) {
                my $directory_id = $directory_ids{$directory};
                my @ids = @{$directories[$directory_id]};
                print "$directory($directory_id): @ids\n" if $verbose;
                $directories .= pack 'vv', $directory_id, 2 * scalar @ids;
                foreach my $id (@ids) {
                        $directories .= pack 'v', $id;
                }
        }
        $offset += length $directories;

# Files.
        my $files_offset = $offset;
        my $files = '';
        foreach my $path (get_value_sorted_keys %file_ids) {
                my $file_id = $file_ids{$path};
                my $directory_id = $files[$file_id];
                my $file = $path;
                $file =~ s/^.*\///;
                $file .= "\0";
                while ((length $file) % 4 != 0) {
                        $file .= "\0";
                }
                print "$file($file_id): $directory_id\n" if $verbose;
                $files .= pack 'vvvVv', $file_id, $directory_id, $file_id,
                        0, length $file;
                $files .= $file;
        }
        $offset += length $files;

# RegHives.
        my $reghives_offset = $offset;
        my $reghives = '';
        foreach my $reghive (get_value_sorted_keys %reghive_ids) {
                my $reghive_id = $reghive_ids{$reghive};
                my $reghive_root = $reghive_roots[$reghive_id];
                my @ids = @{$reghives[$reghive_id]};
                print "$reghive($reghive_id): @ids\n" if $verbose;
                $reghives .= pack 'vvvv', $reghive_id, $reghive_root, 0, 2 * scalar @ids;
                foreach my $id (@ids) {
                        $reghives .= pack 'v', $id;
                }
        }
        $offset += length $reghives;

# RegKeys.
        my $regkeys_offset = $offset;
        my $regkeys = '';
        foreach my $regkey_id (get_value_sorted_keys %regkey_ids) {
                my $hive = $regkey_hives[$regkey_id];
                my $name = $regkey_names[$regkey_id];
                my $value = $regkey_values[$regkey_id];
                my $len = length($value);
                my $typeid = $regkey_typeids[$regkey_id];
                print "$name($regkey_id): \n" if $verbose;
                $regkeys .= pack 'vvvVv', $regkey_id, $hive, 0, $typeid, length($name) + 1 + length($value);
                $regkeys .= $name . "\0" . $value;
        }
        $offset += length $regkeys;

# Links.
        my $links_offset = $offset;
        my $links = '';
        foreach my $link_id (keys %links) {
                my @link = @{ $links{$link_id} };
                my $dirid = $link[0];
                my $basedir = $link[1];
                my $fileid = $link[2];
                my $type = $link[3];
                my @stringids = @{ $link[4] };
                my $length = 2 * scalar @stringids;
                $links .= pack 'vvvvvv', $link_id, $dirid, $basedir, $fileid, $type, $length;
                foreach my $id (@stringids) {
                        $links .= pack 'v', $id;
                }
        }
        $offset += length $links;

# Header.
        my $length = $offset;
        my @fields = (
                0, $length, 0, 1, $architecture,
                @version_requirements,
                $string_count, $directory_count, $file_count,
                $reghive_count, $regkey_count, $links_count,
                $strings_offset, $directories_offset, $files_offset,
                $reghives_offset, $regkeys_offset, $links_offset,
                $application_offset, length $application,
                $provider_offset, length $provider,
                $unsupported_offset, length $unsupported,
                0, 0);
        my $header = 'MSCE';
        $header .= pack 'V11 v6 V6 v8', @fields;

        return $header . $application . $provider . $unsupported .
                $strings . $directories . $files .
                $reghives . $regkeys . $links;
}

# Returns a munged version of the specified filename.
# Removes the leading path. Removes the extension. Removes spaces.
# Truncates to eight characters. Pads to eight characters with leading
# zeros. Adds a numeric extension.
sub munge_filename($$)
{
        my $munged = shift;
        my $extension = shift;
        $munged =~ s/^.*\///;
        $munged =~ s/\..*$//;
        $munged =~ s/ //;
        $munged = substr $munged, 0, 8;
        $munged = sprintf '%08s.%03d', $munged, $extension;
        return $munged;
}

# Returns the _setup.xml file
sub get_setupxml($$)
{
        my ($provider, $application) = @_;

        my $output;

        $output .= "<wap-provisioningdoc>\n";
        $output .= "\t<characteristic type=\"Install\">\n";
        $output .= "\t\t<parm name=\"InstallPhase\" value=\"install\" />\n";
        $output .= "\t\t<parm name=\"OSVersionMin\" value=\"$version_requirements[0].$version_requirements[1]\" />\n";
        $output .= "\t\t<parm name=\"OSVersionMax\" value=\"$version_requirements[2].$version_requirements[3]\" />\n";
        $output .= "\t\t<parm name=\"BuildNumberMin\" value=\"$version_requirements[4]\" />\n";
        $output .= "\t\t<parm name=\"BuildNumberMax\" value=\"$version_requirements[5]\" />\n";
        $output .= "\t\t<parm name=\"AppName\" value=\"$provider $application\" />\n";
        my @filenames = get_value_sorted_keys %file_ids;
        my $firstdir = $file_dirs{$filenames[0]};
        $firstdir =~ s/\//\\/g;
        $output .= "\t\t<parm name=\"InstallDir\" value=\"$firstdir\" translation=\"install\" />\n";
        my $dirs = scalar keys %directory_ids;
        my $files = scalar keys %file_ids;
        $output .= "\t\t<parm name=\"NumDirs\" value=\"$dirs\" />\n";
        $output .= "\t\t<parm name=\"NumFiles\" value=\"$files\" />\n";
        $output .= "\t\t<parm name=\"NumRegKeys\" value=\"$regkey_count\" />\n";
        $output .= "\t\t<parm name=\"NumRegVals\" value=\"$regkey_count\" />\n";
        my $shortcuts = scalar keys %links;
        $output .= "\t\t<parm name=\"NumShortcuts\" value=\"$shortcuts\" />\n";
        $output .= "\t</characteristic>\n";
        $output .= "\t<characteristic type=\"FileOperation\">\n";
        $firstdir =~ s/\\/\//g;

        foreach my $path (get_value_sorted_keys %file_ids) {
                my $file_id = $file_ids{$path};
                my $dir = $file_dirs{$path};
                my $file = $path;
                $file =~ s/^.*\///g;
                $file =~ s/\//\\/g;
                $dir =~ s/($firstdir)/\%InstallDir\%/;
                $dir =~ s/\//\\/g;
                my $munged_file = munge_filename $file, $file_id;
                $output .= "\t\t<characteristic type=\"$dir\" translation=\"install\">\n";
                $output .= "\t\t\t<characteristic type=\"MakeDir\" />\n";
                $output .= "\t\t\t<characteristic type=\"$file\" translation=\"install\">\n";
                $output .= "\t\t\t\t<characteristic type=\"Extract\">\n";
                $output .= "\t\t\t\t\t<parm name=\"Source\" value=\"$munged_file\" />\n";
                $output .= "\t\t\t\t</characteristic>\n";
                $output .= "\t\t\t</characteristic>\n";
                $output .= "\t\t</characteristic>\n";
        }
        foreach my $link (get_value_sorted_keys %link_dirs) {
                my $link_dir = $link_dirs{$link};
                my $link_target = $link_targets{$link};
                $link =~ s/\//\\/g;
                $link_dir =~ s/\//\\/g;
                $link_target =~ s/($firstdir)/\%InstallDir\%/;
                $link_target =~ s/\//\\/g;
                $output .= "\t\t<characteristic type=\"$link_dir\" translation=\"install\">\n";
                $output .= "\t\t\t<characteristic type=\"MakeDir\" />\n";
                $output .= "\t\t\t<characteristic type=\"$link\" translation=\"install\">\n";
                $output .= "\t\t\t\t<characteristic type=\"Shortcut\">\n";
                $output .= "\t\t\t\t\t<parm name=\"Source\" value=\"$link_target\" translation=\"install\" />\n";
                $output .= "\t\t\t\t</characteristic>\n";
                $output .= "\t\t\t</characteristic>\n";
                $output .= "\t\t</characteristic>\n";
        }
        $output .= "\t</characteristic>\n";
        $output .= "\t<characteristic type=\"Registry\">\n";
        foreach my $regkey_id (get_value_sorted_keys %regkey_ids) {
                my $key = $regkey_keys[$regkey_id];
                my $name = $regkey_names[$regkey_id];
                my $type = $regkey_types[$regkey_id];
                my $value = $regkey_xmlvalues[$regkey_id];
                $key =~ s/\//\\/g;
                $output .= "\t\t<characteristic type=\"$key\">\n";
                $output .= "\t\t\t<parm name=\"$name\" value=\"$value\" datatype=\"$type\" />\n";
                $output .= "\t\t</characteristic>\n";
        }
        $output .= "\t</characteristic>\n";
        $output .= "</wap-provisioningdoc>\n";

        return $output;
}


# Prints the version message and exits.
sub version()
{
        print
                "pocketpc-cab 1.0.1\n" .
                "Written by Shaun Jackman <sjackm...\@gmail.com>.\n" .
                "\n" .
                "Copyright 2006 Shaun Jackman\n" .
                "This is free software; see the source for copying\n" .
                "conditions. There is NO warranty; not even for\n" .
                "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n";
        exit 0;
}


# Main.
sub main()
{
        # Parse the command line.
        my $provider = 'Provider';
        my $application = 'Application';
        my $sourcedir = '';
        my $destdir = '';
        my $regfile;

        GetOptions(
                        "provider=s" => \$provider,
                        "application=s" => \$application,
                        "source=s" => \$sourcedir,
                        "dest=s" => \$destdir,
                        "registry=s" => \$regfile,
                        "verbose|v!" => \$verbose,
                        "help|?" => sub { pod2usage(
                                -exitstatus => 0, -verbose => 1) },
                        "man" => sub { pod2usage(
                                -exitstatus => 0, -verbose => 2) },
                        "version" => \&version);

        $sourcedir .= '/' if length $sourcedir > 0;
        $destdir .= '/' if length $destdir > 0;

        if( scalar @ARGV < 1) {
                print
                        "pocketpc-cab: missing file arguments\n" .
                        "Try `pocketpc-cab --help' for more information.\n";
                exit 1;
        }
        my $cab_filename = pop @ARGV;

        # Parse the input file.
        parse_input( $destdir);
        parse_regfile($regfile) if defined $regfile;

        # Create the manifest.
        my $manifest = "manifest.000";
        open MANIFEST, ">$manifest";
        binmode MANIFEST;
        print MANIFEST get_manifest( $provider, $application);
        close MANIFEST;

        # Create the _setup.xml
        my $setupxml = "_setup.xml";
        open SETUPXML, ">$setupxml";
        print SETUPXML get_setupxml( $provider, $application);
        close SETUPXML;

        # Copy the data files.
        my $munged_files = " $setupxml";
        my $i = 0;
	`mkdir munged`;
        foreach my $file (get_value_sorted_keys %file_ids) {
                my $munged_file = munge_filename $file, ++$i;
                print "$file: $munged_file\n" if $verbose;
                `cp "$sourcedir$file" "munged/$munged_file"`;
                exit $? >> 8 if $? > 0;
                $munged_files = ' ' . "munged/$munged_file" . $munged_files;
        }
        $munged_files = 'manifest.000' . $munged_files;

        # Create the cab.
        print "$cab_filename: $munged_files\n" if $verbose;
        my $lcab_output = `$ENV{'MOSYNCDIR'}/bin/lcab -r -n manifest.000 munged $setupxml "$cab_filename"`;
        exit $? >> 8 if $? > 0;
        print $lcab_output if $verbose;
        `rm -r munged manifest.000 $setupxml`;
}


# Entry-point.
main;


__END__

=head1 NAME

pocketpc-cab - build an installable Pocket PC cabinet file

=head1 SYNOPSIS

B<pocketpc-cab> [I<OPTION>]... I<INPUTFILE> I<CABINET>

=head1 DESCRIPTION
                                                                                
                                                        
Read the INPUTFILE, which is a list of filenames and destination
directories, and create CABINET, a cabinet file, that will install
those files into the specified directories.

=head1 OPTIONS

=over

=item B<-p, --provider>=I<PROVIDER>

set the provider name

=item B<-a, --application>=I<APPLICATION>

set the application name

=item B<-s, --source>=I<SOURCE>

set the source directory

=item B<-d, --dest>=I<DEST>

set the destination directory

=item B<-r, --registry>=I<REGISTRYFILE>

use a registry specification file

=item B<-v, --verbose>

display verbose output

=item B<--help>

display a brief help message

=item B<--man>

display the full documentation

=back

=head1 EXAMPLES

 $ cat > foobar.files <<EOF
 foobar.exe /bin "Shortcut Name" %CE11%
 foobar.dll /windows
 EOF
 $ cat > foobar.regkeys <<EOF
 HKCU Software/Fooware/Foobar Name string "Some string"
 HKCU Software/Fooware/Foobar Data binary 00112233445566778899aabbccddeeff
 HKCU Software/Fooware/Foobar Value integer 42
 HKCU Software/Fooware/Foobar HexValue integer 0xff
 HKCU Software/Fooware/Foobar Names multistring "String 1" "String 2"
 EOF
 $ pocketpc-cab -p Fooware -a FooBar -r foobar.regkeys foobar.files foobar.cab

=head1 AUTHOR

Written by Shaun Jackman.

=head1 REPORTING BUGS

Report bugs to Shaun Jackman <sjack...@gmail.com>.

=head1 COPYRIGHT

Copyright 2006 Shaun Jackman

This is free software; see the source for copying conditions. There is
NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

=head1 SEE ALSO

=over

=item B<lcab>(1)

create cabinet archives

=item B</usr/share/doc/pocketpc-cab/wince_cab_format.html>

Windows CE installation cabinet file format

=back

=cut

#ChangeLog
#2009-12-21 Andre Heinecke <aheinecke@intevation.d>
#        Fixed Memory Card install-location
#
#2006-04-06  Shaun Jackman  <sjack...@gmail.com>
#
#        * Release version 1.0.1.
#        * Fix the CAB for WinCE5 by putting the manifest.000 file first in
#        the cabinet file.
#        Thanks to Rouven Schürch <rouven.schue...@tegonal.com>.
#
#2004-09-17  Shaun Jackman  <sjack...@debian.org>
#
#        * Initial release, version 1.0.0.

