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
	'tst-fmemopen.c' => [],
}

SPECIFIC_FILES = {
	'test-stat2.c' => ["#{SETTINGS[:source_path]}/io/Makefile", "build/test-stat2.moo"],
	'tst-rxspencer.c' => ["#{SETTINGS[:source_path]}posix/rxspencer"],
	'tst-pcre.c' => ["#{SETTINGS[:source_path]}posix/PCRE.tests"],
	'tst-boost.c' => ["#{SETTINGS[:source_path]}posix/BOOST.tests"],
}

DEV_NULL = Proc.new do
	FileUtils.mkdir_p('filesystem/dev')
	FileUtils.touch('filesystem/dev/null')
end

SPECIFIC_CODE = {
	'tst-dir.c' => Proc.new do
		FileUtils.mkdir_p('filesystem/build/foo')
		FileUtils.cp('build/tst-dir.moo', 'filesystem/build/foo/tst-dir')
	end,
	'scanf4.c' => DEV_NULL,
	'bug22.c' => DEV_NULL,
}
