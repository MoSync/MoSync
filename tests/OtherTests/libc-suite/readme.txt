This is a system for running the glibc test suite on MoSync/newlib.

It has a few prerequisites:
* MoSync SDK.
* Ruby. Tested with version 1.9, but it may work with 1.8 as well.
* glibc source code. Any recent version should work.
* If you want to run the test suite on anything except MoRE: a web server, set up to serve files in a directory.
** For example: Lighttpd.

To set up the system:
Copy settings.rb.example to settings.rb. Edit it. These are the settings you'll want to change:
source_path: path to glibc.
loader_base_url: the address to your web server.
htdocs_dir: path to the directory from which the web server serves files.

To compile the test programs and run them on MoRE:
On a command line, run "ruby all_tests.rb".
There are around 225 tests, so this will take a while. About 30 minutes, on a modern desktop machine.
If you've set up SETTINGS[:htdocs_dir], this will also copy the tests to that directory.

To run a single test program:
"ruby all_tests.rb <name_of_test_program.c>"

To run the suite on a remote device (like a phone or tablet), the programs will be loaded via HTTP.
The loader program is located in the "loader" directory.
Before compiling it, copy config.h.example to config.h, and edit it to match SETTINGS[:loader_base_url].
Then, assuming your web server is accessible from your remote device, simply install and run the loader.
The loader will load and run each test in order, and stop if there is any error.
If there's an error, or the loader is interrupted: when restarted, it will continue where it left off.
To reset the loader: uninstall it, removing all of its saved data.

End of Line.
