#
# Makefile
#
VERSION=1.1.0
PROJECT=bmconv

all clean install uninstall:
	$(MAKE) $@ -sC src

tarball: clean
	debclean
	cd .. && tar -czvf $(PROJECT)-$(VERSION).tar.gz $(PROJECT)-$(VERSION) --exclude .svn

regtest:
	$(MAKE) all -sC src
	$(MAKE) clean -sC test
	$(MAKE) regtest -sC test
