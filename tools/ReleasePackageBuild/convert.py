import string
import sys
import os.path
import os
from subprocess import Popen
import subprocess

##
# Changes the dylib paths of the given files.
#
# files A list of existing files.
# name A name to change.
# new_name The new_name to replace it with.
#
def change_name(files, name, new_name):
	for file in files:
		paths = run_otool( file )
		change_lib_paths(file, paths, name, new_name)
	

##
# Gets the dylib information from the file and returns it.
#
def run_otool(file):
	proc = os.popen("otool -L %s" % (file));
	
	clean_lines = list()
	for line in proc.readlines():
		stripped_line = line.strip()
		path_part = line.split(" ")[0]
		clean_lines.append(path_part.strip("\t"))
		
	return clean_lines[1:]
	

##
# Changes the lib path for the given paths, by replaces
# name in the paths with new_name.
#
# file The binary or dylib to change
# paths A set of dylib paths included in the 
# name String to replace in the path
# new_name New string
#
def change_lib_paths(file, paths, name, new_name):
	for path in paths:
		if name in path:
			new_path = path.replace(name, new_name)
			print "install_name_tool -change %s %s %s" % (path, new_path, file)
			proc = os.popen("install_name_tool -change %s %s %s" % (path, new_path, file))
			
			# Wait for process to end
			proc.close()

##
# Ensures that the given files exist, prints
# an error message if they don't. The existing
# files is returned in a list.
#
# files - A list of files
def check_files(files):
	good_files = list()
	for file in files:
		if os.path.isfile(file):
			good_files.append( file )
		else:
			print "%s is not a file, ignored" % (file)
	
	return good_files


if __name__ == "__main__":
	if len(sys.argv) <= 3:
		print "convert: cur new files"
		print "Replaces prefixes in dylib path, e.g. replaces cur in the dylib paths with new, it can be a partial expression, for example cur can be /sw/lib and new can be /opt/local/lib."
		exit(1)
		
	cur = sys.argv[1]
	new = sys.argv[2]
	
	files = check_files(sys.argv[3:])
	
	change_name(files, cur, new);