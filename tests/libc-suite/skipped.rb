SKIPPED_DIRECTORIES = [
	'argp',
	'assert',
	'catgets',
	'crypt',
	'elf',
	'setjmp',
	'signal',
	'dirent',
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
	'tst-getcwd.c',
	'tst-fcntl.c',
	'bug-ftw1.c',
	'bug-ftw2.c',
	'bug-ftw3.c',
	'bug-ftw4.c',
	'bug-ftw5.c',
	'tst-statvfs.c',
	'tst-openat.c',	# todo
	'tst-unlinkat.c',	# todo
	'tst-fstatat.c',	# todo, dirent
	'tst-futimesat.c',	# todo
	'tst-renameat.c',	# todo
	'tst-fchownat.c',	# todo
	'tst-fchmodat.c',	# todo
	'tst-faccessat.c',	# todo
	'tst-symlinkat.c',	# todo
	'tst-linkat.c',	# todo
	'tst-readlinkat.c',	# todo
	'tst-mkdirat.c',	# todo
	'tst-mknodat.c',	# todo
	'tst-mkfifoat.c',	# todo
	'tst-ttyname_r.c',	# todo
	'tst-posix_fallocate.c',	# todo
	'tst-widetext.c',	# todo, iconv
	'tst-ext.c',
	'tst-ext2.c',
	'tst-fopenloc.c',
	'tst-atime.c',
	'bug-memstream1.c',	# todo, ftello64
	'bug-wmemstream1.c',	# todo, ftello64
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
	
	'tst-dirname.c',	# dirname
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
	
	'tst-truncate64.c',	# truncate64
	'tst-fork.c',	# fork
	'tst-dir.c',	# todo, dirent.h
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
	'tst-environ.c',	#todo, setenv & getenv
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
	'tst_wscanf.c',	# we don't have stdin.
	'tst_getwc.c',
	'tst-swscanf.c',	# Implementation-defined behaviour (%[a-c]). Also, an unsupported locale. See notes.
	'bug-mmap-fflush.c',	# system
	'tst-fopenloc2.c',	# GNU extension: fopen(ccs).
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
