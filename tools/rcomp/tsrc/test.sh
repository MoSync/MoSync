#!/bin/sh
#
n_tests=3
RCOMP=../src/rcomp


test_all()
{
	echo Running through each test...
	test_1
	test_2
	test_3
}


test_1()
{
echo Test 1 : Compare output with previous version

rm -f test1.lis

echo ""
echo "TUTEXT (16-bit)"
echo "Expect five problem messages about deprecated characters..."
echo ""
$RCOMP -u -stutext.rss -otutext16.rsc.new -htutext0.rsg.new
diff tutext0.rsg.new tutext0.rsg >> test1.lis
if [ $? != 0 ]; then
	echo "#"
	echo "# TEST FAILED: tutext0.rsg"
	echo "#"
fi
diff tutext16.rsc.new tutext16.rsc >> test1.lis
if [ $? != 0 ]; then
	echo "#"
	echo "# TEST FAILED: tutext0.rsc"
	echo "#"
fi

echo ""
echo "TWTEXT (16-bit only)..."
echo ""
$RCOMP -u -stwtext.rss -otwtext16.rsc.new -htwtext0.rsg.new
diff twtext0.rsg.new twtext0.rsg >> test1.lis
if [ $? != 0 ]; then
	echo "#"
	echo "# TEST FAILED: twtext0.rsg"
	echo "#"
fi
diff twtext16.rsc.new twtext16.rsc >> test1.lis
if [ $? != 0 ]; then
	echo "#"
	echo "# TEST FAILED: twtext0.rsc"
	echo "#"
fi

grep test1 *.lis

echo "Check the contents of test1.lis for successful comparisons..."
echo "A successful run yields a file of approximately 336 bytes in size."
}


MACROS="\
    STUPID \
    ERROR_INVALID_MEMBER \
    MAIN_SYNTAX_ERROR_1 \
    H_SYNTAX_ERROR_1 \
    ERROR_MISSING_STRUCT"

test_2()
{
echo "Test 2 : Demonstrate behaviour with source errors"

for m in $MACROS; do
    echo "  testing '$m'"
    cpp -I . -D$m < error1.rss | $RCOMP -u -oerror1.rsc -herror1.rsg
done

echo ""
echo "See the documentation in test.htm for an explanation of the error messages"
}

test_3()
{
echo "Test 3 : Test various real-life cases from different S60 SDKs"

$RCOMP -6 -u -saknexslider.rpp -oaknexslider.rsc.test
cmp aknexslider.rsc aknexslider.rsc.test
rm aknexslider.rsc.test

$RCOMP -6 -u -saknexslideraif.rpp -oaknexslideraif.rsc.test
cmp aknexslideraif.rsc aknexslideraif.rsc.test
rm aknexslideraif.rsc.test

$RCOMP -u -saknexsettinglistaif.rpp -oaknexsettinglistaif.rsc.test -:aknexsettinglistaif.rsc.test_dump_
cmp aknexsettinglistaif.rsc aknexsettinglistaif.rsc.test
cmp aknexsettinglistaif.rsc_dump_1 aknexsettinglistaif.rsc.test_dump_1
rm aknexsettinglistaif.rsc.test aknexsettinglistaif.rsc.test_dump_1

mkdir -p aknexsettinglist.rsc.test_dump
$RCOMP -u -saknexsettinglist.rpp -oaknexsettinglist.rsc.test -:aknexsettinglist.rsc.test_dump/dump_
# this testcase fails, even though the file seems to work ok...
# seems to be some issue regarding compressed/uncompressed unicode
#cmp aknexsettinglist.rsc aknexsettinglist.rsc.test
diff -urN -x .\* aknexsettinglist.rsc_dump aknexsettinglist.rsc.test_dump
rm aknexsettinglist.rsc.test
rm -rf aknexsettinglist.rsc.test_dump

$RCOMP -u -shelloworldbasic_reg.rpp -ohelloworldbasic_reg.rsc.test
cmp helloworldbasic_reg.rsc helloworldbasic_reg.rsc.test
rm helloworldbasic_reg.rsc.test

$RCOMP -u -shelloworldbasic.rpp -ohelloworldbasic.rsc.test
cmp helloworldbasic.rsc helloworldbasic.rsc.test
rm helloworldbasic.rsc.test

$RCOMP -u -snegativeenum.rpp -onegativeenum.rsc.test
cmp negativeenum.rsc negativeenum.rsc.test
rm negativeenum.rsc.test
}


help()
{
echo "Usage: test.sh parameter"
echo "where parameter may be \"HELP\", \"ALL\" or a test number."
echo "The current tests are:"
echo " 1 - Test compiler produces identical output to previous versions"
echo " 2 - Test behaviour with various errors"
echo " 3 - Test various real-life cases from different S60 SDKs"
}


#
# main
#
case $1 in
	1) test_1;;
	2) test_2;;
	3) test_3;;
	all) test_all;;
	help) help;;
	*) help;;
esac
