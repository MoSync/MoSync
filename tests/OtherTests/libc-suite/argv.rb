# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
	'tst-qsort2.c' => [],
}

SPECIFIC_FILES = {
	'test-stat2.c' => ["#{SETTINGS[:source_path]}/io/Makefile", "#{BUILD_DIR}/test-stat2.moo"],
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
		FileUtils.cp("#{BUILD_DIR}/tst-dir.moo", 'filesystem/build/foo/tst-dir')
	end,
	'scanf4.c' => DEV_NULL,
	'bug22.c' => DEV_NULL,
}
