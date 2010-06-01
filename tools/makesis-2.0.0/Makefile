VERSION=2.0.0

all clean install uninstall:
	make $@ -C src

tarball: clean
	debclean
	cd .. && tar -czvf makesis-$(VERSION).tar.gz makesis-$(VERSION) --exclude CVS	

regtest:
	make all -sC src
	make regtest -sC test
