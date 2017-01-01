#!/bin/bash

set -e
#set -x

mk()
{
	echo "    BUILD $4$5"
    $1 $2.ctx $3 \
        --output $4.$5.asm \
        --output-tree $4.$5.tree \
        --output-include-list $4.$5.includes \
        $6 $7 $8 && \
    fasm  $4.$5.asm $4$5 >/dev/null && \
	chmod a+x $4$5
}

mkdir -p out

iconv -f cp866 -t utf8 < messages_cp866.ctxi > messages_utf8.ctxi

printf "=> ${CODE_COLOR_YELLOW}Stage A${CODE_COLOR_NOCOLOR}\n"

mk ../precompiled/lcontext_c ctx4lnx    --linux out/ lcontext_a

printf "=> ${CODE_COLOR_YELLOW}Stage B${CODE_COLOR_NOCOLOR}\n"
mk out/lcontext_a ctx4lnx       --linux   out/ lcontext_b
mk out/lcontext_a ctx4lnx       --linux   out/ lcontext_b_debug --optimize none
mk out/lcontext_a ctx4win       --win32-c out/ lcontext_b.exe
mk out/lcontext_a ctx4win       --win32-c out/ lcontext_b_debug.exe --optimize none

printf "=> ${CODE_COLOR_YELLOW}Stage C${CODE_COLOR_NOCOLOR}\n"
mk out/lcontext_b ctx4lnx       --linux   out/ lcontext_c #--warn-unused-globals
mk out/lcontext_b ctx4lnx       --linux   out/ lcontext_c_debug --optimize none
mk out/lcontext_b ctx4win       --win32-c out/ lcontext_c.exe #--warn-unused-globals
mk out/lcontext_b ctx4win       --win32-c out/ lcontext_c_debug.exe --optimize none

# Generated assembler listings for A and B stages should be identical.
diff out/.lcontext_b.asm           out/.lcontext_c.asm
diff out/.lcontext_b_debug.asm     out/.lcontext_c_debug.asm
diff out/.lcontext_b.exe.asm       out/.lcontext_c.exe.asm
diff out/.lcontext_b_debug.exe.asm out/.lcontext_c_debug.exe.asm

# The binaries should be identical too.
diff out/lcontext_b           out/lcontext_c
diff out/lcontext_b_debug     out/lcontext_c_debug
# Do not test PE files, since they differ in timestamp.
#diff out/lcontext_b.exe       out/lcontext_c.exe
#diff out/lcontext_b_debug.exe out/lcontext_c_debug.exe

printf "=> ${CODE_COLOR_YELLOW}Building samples${CODE_COLOR_NOCOLOR}\n"

mk out/lcontext_c samples/z_t1  --win32-c out/ z_t1.exe
mk out/lcontext_c samples/z_t2  --win32-w out/ z_t2.exe
mk out/lcontext_c samples/z_t3  --win32-c out/ z_t3.exe
mk out/lcontext_c samples/z_t4  --win32-c out/ z_t4.exe
mk out/lcontext_c samples/z_t5  --win32-c out/ z_t5.exe
mk out/lcontext_c samples/z_t6  --win32-c out/ z_t6.exe

printf "=> ${CODE_COLOR_YELLOW}Running tests${CODE_COLOR_NOCOLOR}\n"

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

do_test_with_compiler ()
{
	local failed=1
	local passed=0
	local c="$1" ; shift
	local test_name="$1" ; shift
	local condition="$1" ; shift
	if [ "x$condition" = "xcompilation_should_fail" ] ; then
		if (mk out/lcontext_"$c" tests/"$test_name"\
				--linux tests/out/ "$test_name" \
				1> tests/out/"$test_name"."$c".stdout \
				2> tests/out/"$test_name"."$c".stderr) ; then
			return $failed
		fi
		if (cat tests/out/"$test_name"."$c".stdout tests/out/"$test_name"."$c".stderr | grep -q "$1") ; then
			return $passed
		else
			return $failed
		fi
	else
		return $failed
	fi
}

do_test ()
{
	local test_name="$1"
	if do_test_with_compiler b "$@" && do_test_with_compiler c "$@" ; then
		passed "$test_name"
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
