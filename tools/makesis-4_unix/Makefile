VERSION=2.0.0

all clean install uninstall:
	$(MAKE) $@ -C src

tarball: clean
	cd .. && tar -czvf makesis-$(VERSION).tar.gz makesis-$(VERSION) --exclude CVS	

regtest:
	$(MAKE) all -sC src
	$(MAKE) regtest -sC test
