#
# Makefile
#

BMCONV=../src/bmconv
TEST_MBM=test1.mbm
REF_MBM=test1_ref.mbm
HEADER_REF=test1_ref.h
HEADER_GEN=test1.h
DECOMP_MBMS=\
	decomp_gen_48i.bmp decomp_gen_48m.bmp \
	decomp_gen_32i.bmp decomp_gen_32m.bmp \
	decomp_gen_24i.bmp decomp_gen_24m.bmp \
	decomp_gen_16i.bmp decomp_gen_16m.bmp

regtest: test_compile test_commandfile test_decomp

test_compile:
	$(BMCONV) /h$(HEADER_GEN) $(TEST_MBM) \
	/c12test1_48i.bmp /c12test1_48m.bmp \
	/c12test1_32i.bmp /c12test1_32m.bmp \
	/c12test1_24i.bmp /c12test1_24m.bmp \
	/c12test1_16i.bmp /c12test1_16m.bmp
	diff $(TEST_MBM) $(REF_MBM)
	diff $(HEADER_REF) $(HEADER_GEN)
	rm -f $(TEST_MBM)
	rm -f $(HEADER_GEN)

test_commandfile:	test1.lst
	$(BMCONV) test1.lst
	diff $(TEST_MBM) $(REF_MBM)
	rm -f $(TEST_MBM)

test_decomp:	$(REF_MBM)
	$(BMCONV) /u $(REF_MBM) $(DECOMP_MBMS)
	diff decomp_ref_48i.bmp decomp_gen_48i.bmp
	diff decomp_ref_48m.bmp decomp_gen_48m.bmp
	diff decomp_ref_32i.bmp decomp_gen_32i.bmp
	diff decomp_ref_32m.bmp decomp_gen_32m.bmp
	diff decomp_ref_24i.bmp decomp_gen_24i.bmp
	diff decomp_ref_24m.bmp decomp_gen_24m.bmp
	diff decomp_ref_16i.bmp decomp_gen_16i.bmp
	diff decomp_ref_16m.bmp decomp_gen_16m.bmp
	rm -f decomp_gen_???.bmp

check:
	od -vtx4 $(REF_MBM) > $(REF_MBM).od
	od -vtx4 $(TEST_MBM) > $(TEST_MBM).od
	diff $(REF_MBM).od $(TEST_MBM).od

check_decomp:
	od -vtx1 decomp_ref_48i.bmp > decomp_ref_48i.bmp.od
	od -vtx1 decomp_gen_48i.bmp > decomp_gen_48i.bmp.od
	diff decomp_ref_48i.bmp.od decomp_gen_48i.bmp.od

clean:
	rm -f *~ *.od $(TEST_MBM) $(DECOMP_MBMS) $(HEADER_GEN)
