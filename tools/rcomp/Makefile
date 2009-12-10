#
# Makefile
#
all clean install uninstall:
	$(MAKE) $@ -sC src

regtest:
	$(MAKE) all -sC src
	$(MAKE) regtest -sC tsrc
