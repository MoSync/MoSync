The file epocrc_input.txt contains source data for various configurations of input for resource files.

There are no options to pass to the tests, simply type:

	perl epocrctest.pl
	
at the command line.

The output from the tests will be placed in a 'test-data' sub-directory. For test nn the following 
files will be produced:

testnn.rss - input file to epocrc.pl
testnn.rls - file included in testnn.rss by cpp. Contains localisation information

testnn.rpp - localised rpp file

testnn.rsc - output resource file
testnn.rsg - output header file

expected.stderr - the expected output to stderr (produced from epocrc_input.txt)
expected.stdout - the expected output to stdout (produced from epocrc_input.txt)

rcomp.stderr - output redirected to file from stderr
rcomp.stdout - output redirected to file from stdout


If the test is successful then the files expected.stderr and rcomp.stderr should contain the same
data, as should expected.stdout and rcomp.stdout.

Also, the generated .rsc file should match the version stored in rsc-files\

