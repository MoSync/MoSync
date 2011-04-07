DEFAULT_ARGV = ['--direct', '--test-dir', '/']

SPECIFIC_ARGV = {
	'test-stat2.c' => ['Makefile', '.', 'test-stat2.moo'],
	'tst_wprintf2.c' => ['Some Text'],
	'tstgetopt.c' => ['-a', '-b', '-cfoobar', '--required', 'foobar', '--optional=bazbug',
		'--none', 'random', '--colou', '--color', '--colour'],
	'tst-rxspencer.c' => ['--utf8', 'rxspencer/tests'],
	'tst-pcre.c' => ['PCRE.tests'],
	'tst-boost.c' => ['BOOST.tests'],
	'list.c' => ['/', '.'],
	'tst-dir.c' => ['/', '/build/foo', '/build', 'tst-dir'],
}

SPECIFIC_FILES = {
	'test-stat2.c' => ["#{SETTINGS[:source_path]}/io/Makefile", "build/test-stat2.moo"],
	'tst-rxspencer.c' => ["#{SETTINGS[:source_path]}posix/rxspencer"],
	'tst-pcre.c' => ["#{SETTINGS[:source_path]}posix/PCRE.tests"],
	'tst-boost.c' => ["#{SETTINGS[:source_path]}posix/BOOST.tests"],
}

SPECIFIC_CODE = {
	'tst-dir.c' => Proc.new do
		FileUtils.mkdir_p('filesystem/build/foo')
		FileUtils.cp('build/tst-dir.moo', 'filesystem/build/foo/tst-dir')
	end
}
