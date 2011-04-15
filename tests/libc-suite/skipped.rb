SKIPPED_DIRECTORIES = [
	'argp',
	'assert',
	'catgets',
	'crypt',
	'elf',
	'setjmp',
	'signal',
	'dlfcn',
	'gmon',
	'iconv',	# todo
	'iconvdata',	# todo
	'inet',	# todo
	'intl',
	'localedata',
	'login',
	'nptl',
	'nss',
	'resolv',
	'resource',
	'rt',
	'shadow',
]

# These are tests that should pass, but don't, and could not be fixed.
SKIPPED_UNRESOLVED = [
	'bug-ungetc2.c',
	'test-double.c',	# 1269 test cases, 93 errors.
	'test-float.c',
	'testfnm.c',	# 49 tests, 3 errors.
	'runptests.c',	# 326 tests, 15 errors.
	'bug-regex9.c',	# regcomp failed: empty (sub)expression.
	'bug-regex11.c',
	'bug-regex15.c',	# regerror repetition-operator operand invalid.
	'bug-regex16.c',
	'bug-regex17.c',
	'bug-regex18.c',
	'bug-regex24.c',
	'bug-regex30.c',
	'tst-rxspencer.c',	# about 90% failures.
	'tst-pcre.c',
	'tst-boost.c',	# about 50% failures.
	'tst-fnmatch2.c',	# infinite loop.
	'bug-getopt3.c',
	'bug-getopt4.c',
	'bug-getopt5.c',
	'tstscanf.c',	# two minor failures.

	# Seems like a getc() causes the write position to jump to the end of the file,
	# but it should be the same as the read position. Too tricky to fix.
	'test_rdwr.c',
	
	'tst-sprintf.c',	# newlib's printf doesn't handle invalid formats the same way as glibc.
	'bug14.c',	# apparently, sscanf() doesn't convert from UTF-8 to wchar_t.
	'scanf13.c',	# 8 fails.
	'tst-strtod.c',	# 2 regular fails, and no locale support.
]

SKIPPED_FILES = SKIPPED_UNRESOLVED + [
	'tst-atomic.c',
	'tst-atomic-long.c',
	'backtrace-tst.c',
	'tst-longjmp_chk.c',
	'tst-chk1.c',
	'tst-chk2.c',
	'tst-chk3.c',
	'tst-lfschk1.c',
	'tst-lfschk2.c',
	'tst-lfschk3.c',
	'test-strcpy_chk.c',
	'test-stpcpy_chk.c',
	'tst-longjmp_chk.c',
	'tst-longjmp_chk2.c',
	'tst-gshadow.c',
	'test-utime.c',
	'test-lfs.c',
	'tst-fcntl.c',
	'bug-ftw1.c',
	'bug-ftw2.c',
	'bug-ftw3.c',
	'bug-ftw4.c',
	'bug-ftw5.c',
	'tst-statvfs.c',
	'tst-widetext.c',	# todo, iconv
	'tst-popen1.c',	# popen
	
	# locale
	'tst-C-locale.c',
	'tst-locname.c',
	'tst-duplocale.c',
	
	'tst-valloc.c',	# valloc
	'tst-obstack.c',	# obstack
	'tst-mallocstate.c',	# malloc_get_state
	'tst-mallocfork.c',
	'test-matherr.c',	# todo, matherr
	'test-fenv.c',	# complex.h
	
	# gmp.h
	'atest-exp.c',
	'atest-exp2.c',
	'atest-sincos.c',
	
	'test-fpucw.c',	# fpu_control.h
	
	# tgmath.h
	'test-tgmath.c',
	'test-tgmath2.c',
	'test-tgmath-int.c',
	'test-tgmath-ret.c',
	'bug-tgmath1.c',
	
	'tst-efgcvt.c',	# ecvt_r
	'tst-mntent.c',	# mntent.h
	'tst-mntent2.c',	# mntent.h
	'tst-pselect.c',	# select
	'tst-preadwrite64.c',	# 64
	'test-vfork.c',	# fork
	'tst-getlogin.c',	# getlogin_r
	'tst-mmap.c',	# mmap
	
	# todo, netinet
	'tst-getaddrinfo.c',
	
	'tst-fork.c',	# fork

	'tst-chmod.c',	# chmod
	'bug-regex1.c',	# re_registers
	'bug-regex4.c',	# re_search_2
	'bug-regex5.c', # locale/localeinfo.h
	'bug-regex7.c', # regex_t::regs
	'bug-regex8.c',	# re_search_2
	'bug-regex10.c', # regex_t::regs
	'bug-regex13.c',	# re_search
	'bug-regex14.c',	# re_comp
	'bug-regex19.c',	# re_search
	'bug-regex20.c',	# regex_internal.h
	'bug-regex22.c',	# re_search
	'bug-regex25.c', # regex_t::regs
	'bug-regex26.c',	# re_search
	'bug-regex28.c',	# re_search
	'tst-gnuglob.c',	# gnuglob
	'tst-regex.c',	# iconv, regex_t::fastmap
	'tst-regex2.c',	# regex_t::regs
	'transbug.c',	# re_registers
	'bug-ga1.c',	# sys/socket.h
	'tst-vfork1.c',
	'tst-vfork2.c',
	'tst-vfork3.c',
	'tst-waitid.c',
	'bug-glob1.c',	# symlink, glob
	'bug-glob2.c',
	'tst-sysconf.c',
	'tst-cpucount.c',
	'tst-cpuset.c',
	'wordexp-test.c',	# todo, wordexp -> dirent.h
	'tst-spawn.c',
	'tst-getpw.c',	# getpw
	'tst-shm.c',	# sys/mman.h
	'test-popen.c',	# popen
	'errnobug.c',	# pipe
	'tst-printfsz.c',	# register_printf_function
	'tst-obprintf.c',	# obstack.h
	'tst-unlockedio.c',
	'tst-xpg-basename.c',
	'tst-setcontext.c',
	'tst-makecontext.c',
	'tst-makecontext2.c',
	'tst-unsetenv1.c',	# clearenv
	'tst-strxfrm.c',	# newlocale
	'tst-strfry.c',	# strfry
	'bug-envz1.c',	# envz_entry
	'tst-endian.c',	# htobe16
	'clocktest.c',	# signal
	'tst-getdate.c',	# getdate
	'tst-ftime_l.c',	# locale_t
	'tst-strptime2.c',	# tm_gmtoff
	'bug-getdate1.c',	# getdate
	'tst-nanosleep.c',	# nanosleep
	'tst-nice.c',	# we are not nice
	'tst-perror.c',	# To be used only for testing glibc.
	'libio_tst-swscanf.c',	# Implementation-defined behaviour (%[a-c]). Also, an unsupported locale. See notes.
	'bug-mmap-fflush.c',	# system
	'tst-fopenloc2.c',	# GNU extension: fopen(ccs).
	# glibc doesn't implement open_wmemstream's sizep parameter according to the Open Group Base Specification,
	# so this test fails.
	'tst-wmemstream2.c',
	
	# fp exceptions not supported by newlib.
	'bug-nextafter.c',
	'bug-nexttoward.c',
	
	# some of the glibc math types are unsupported.
	'test-fenv.c',
	'test-fpucw.c',
	'test-idouble.c',
	'test-ifloat.c',
	'test-ildouble.c',
	'test-ldouble.c',
	#'test-matherr.c',
	
	'tst-error1.c',	# requires complex arguments and input files.
	
	'bug-glob3.c',	# GLOB_NOMATCH
	'bug-getopt1.c',	# non-posix modifier: +
	
	'bug5.c',	# system()
	
	'tst-seekdir.c', # seekdir()
	
	'scanf12.c',	# test seems broken.
	'tst-swprintf.c',	# locale ja_JP.EUC-JP
	'tst-popen.c',	# popen()
	'tst-popen2.c',	# popen()
	'scanf14.c',	# GNU extension: %as
	'scanf16.c',	# GNU extension: %as
	'bug21.c',	# GNU extension: %as
	'bug22.c',	# we don't support writing to /dev/null.
	
	'tst-random.c',	# initstate()
	'tst-random2.c',	# initstate()
	'tst-system.c',	# system()
	'tst-strtod4.c', # locale cs_CZ.UTF-8
	'tst-strtod5.c', # locale cs_CZ.UTF-8
	'tst-strtod6.c', # incorrect assumption about parsing of "NaN(foobar)"
	'tst-makecontext3.c',	# ucontext.h
	'tst-putenv.c',	# putenv from another process.
	
	'xbug.c',	# popen()
	
	'tst-timezone.c',	# non-standard TZ values.
	'test-tz.c',	# apparently, newlib doesn't support zones like "MST"
]

SKIPPED_PATTERNS = [
	/tst-exec.*.c/,
	/tst-rfc3484.*.c/,	# ifaddrs.h
	/tst-getaddrinfo.*.c/,	# sys/socket.h
	/tst-clock.*.c/,	# todo, clock_getres
	/tst-aio.*.c/,	# aio.h
	/test-canon.*.c/,
	/tst-rand48.*.c/,	# srand48_r
	/tst-svc.*.c/,	# strverscmp
	/tst-xdrmem.*.c/,
]
