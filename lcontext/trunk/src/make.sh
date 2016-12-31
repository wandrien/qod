#!/bin/bash

set -e
set -x

mk()
{
    $1 $2.ctx $3 \
        --output $4.$5.asm \
        --output-tree $4.$5.tree \
        --output-include-list $4.$5.includes \
        $6 $7 $8 && \
    fasm  $4.$5.asm $4$5
}

iconv -f cp866 -t utf8 < messages_cp866.ctxi > messages_utf8.ctxi

mk ../bin/lcontext_c ctx4lnx    --linux out/ lcontext_a

mk out/lcontext_a ctx4lnx       --linux out/ lcontext_b
mk out/lcontext_b ctx4lnx       --linux out/ lcontext_c --warn-unused-globals

mk out/lcontext_b ctx4lnx       --linux out/ lcontext_c_debug --optimize none

mk out/lcontext_c ctx4win       --win32-c out/ lcontext_c.exe --warn-unused-globals
mk out/lcontext_c ctx4win       --win32-c out/ lcontext_c_debug.exe --optimize none

diff out/lcontext_b out/lcontext_c

mk out/lcontext_c samples/z_t1  --win32-c out/ z_t1.exe
mk out/lcontext_c samples/z_t2  --win32-w out/ z_t2.exe
mk out/lcontext_c samples/z_t3  --win32-c out/ z_t3.exe
mk out/lcontext_c samples/z_t4  --win32-c out/ z_t4.exe
mk out/lcontext_c samples/z_t5  --win32-c out/ z_t5.exe
mk out/lcontext_c samples/z_t6  --win32-c out/ z_t6.exe

mkdir -p tests/out

failed ()
{
	printf " => ${CODE_COLOR_RED}failed${CODE_COLOR_NOCOLOR} %s\n" "$*"
	exit 1;
}

passed ()
{
	printf " => ${CODE_COLOR_GREEN}passed${CODE_COLOR_NOCOLOR} %s\n" "$*"
}


do_test ()
{
	local test_name="$1" ; shift
	local condition="$1" ; shift
	if [ "x$condition" = "xcompilation_should_fail" ] ; then
		if (mk out/lcontext_c tests/"$test_name"\
				--linux tests/out/ "$test_name" \
				1> tests/out/"$test_name".stdout \
				2> tests/out/"$test_name".stderr) ; then
			failed "$test_name"
		fi
		if (cat tests/out/"$test_name".stdout tests/out/"$test_name".stderr | grep -q "$1") ; then
			passed "$test_name"
		else
			failed "$test_name"
		fi
	else
		failed "$test_name"
	fi
}

do_tests ()
{
	for f in `(cd tests && grep -l 'TEST:' *.* )` ; do
		local t="`basename -s .ctx "$f"`"
		local c="`grep 'TEST:' "tests/$f" | sed 's/^.*TEST://'`"
		eval do_test $t $c
	done
}

set +x

do_tests
