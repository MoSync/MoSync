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
]

SKIPPED_FILES = [
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
	'test-stat.c',	# todo
	'test-stat2.c',	# todo
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
	'tst-fstatat.c',	# todo
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
]
