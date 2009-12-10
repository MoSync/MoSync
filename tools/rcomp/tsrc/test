#!/bin/sh
#
n_tests=2
RCOMP=../src/rcomp


test_all()
{
	echo Running through each test...
	test_1
	test_2
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


help()
{
echo "Usage: test.sh parameter"
echo "where parameter may be \"HELP\", \"ALL\" or a test number."
echo "The current tests are:"
echo " 1 - Test compiler produces identical output to previous versions"
echo " 2 - Test behaviour with various errors"
}


#
# main
#
case $1 in
	1) test_1;;
	2) test_2;;
	all) test_all;;
	help) help;;
	*) help;;
esac
