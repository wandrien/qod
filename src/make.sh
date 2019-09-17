#!/bin/bash

set -e
#set -x

DEFAULT_WINE=""
if which wine > /dev/null 2> /dev/null ; then
	DEFAULT_WINE="wine"
fi

export WINE="${WINE-$DEFAULT_WINE}"

export BOOTSTRAP_COMPILER="${BOOTSTRAP_COMPILER-../precompiled/lcontext_c}"

mk()
{
	echo "    BUILD $4$5"
    $1 $2.ctx $3 \
		--optimize speed \
		--output $4.$5.asm \
		--output-tree $4.$5.tree \
		--output-include-list $4.$5.includes \
		--emit-source-line-notes \
		$6 $7 $8 && \
    fasm -m 200000 $4.$5.asm $4$5 >/dev/null && \
	chmod a+x $4$5
}

stage_a()
{
	printf "=> ${CODE_COLOR_YELLOW}Stage A${CODE_COLOR_NOCOLOR}\n"
	mk $BOOTSTRAP_COMPILER ctx4lnx    --linux out/ lcontext_a
}

stage_b()
{
	printf "=> ${CODE_COLOR_YELLOW}Stage B${CODE_COLOR_NOCOLOR}\n"
	mk out/lcontext_a ctx4lnx       --linux   out/ lcontext_b
	mk out/lcontext_a ctx4lnx       --linux   out/ lcontext_b_debug --optimize none
	mk out/lcontext_a ctx4lnx       --linux   out/ lcontext_b_size  --optimize size
	mk out/lcontext_a ctx4win       --win32-c out/ lcontext_b.exe
	mk out/lcontext_a ctx4win       --win32-c out/ lcontext_b_debug.exe --optimize none
	mk out/lcontext_a ctx4win       --win32-c out/ lcontext_b_size.exe --optimize size
}

stage_c()
{
	printf "=> ${CODE_COLOR_YELLOW}Stage C${CODE_COLOR_NOCOLOR}\n"
	mk out/lcontext_b ctx4lnx       --linux   out/ lcontext_c #--warn-unused-globals
	mk out/lcontext_b ctx4lnx       --linux   out/ lcontext_c_debug --optimize none
	mk out/lcontext_b ctx4lnx       --linux   out/ lcontext_c_size --optimize size
	mk out/lcontext_b ctx4win       --win32-c out/ lcontext_c.exe #--warn-unused-globals
	mk out/lcontext_b ctx4win       --win32-c out/ lcontext_c_debug.exe --optimize none
	mk out/lcontext_b ctx4win       --win32-c out/ lcontext_c_size.exe --optimize size

	# Generated assembler listings for B and C stages should be identical.
	diff out/.lcontext_b.asm           out/.lcontext_c.asm
	diff out/.lcontext_b_debug.asm     out/.lcontext_c_debug.asm
	diff out/.lcontext_b_size.asm      out/.lcontext_c_size.asm
	diff out/.lcontext_b.exe.asm       out/.lcontext_c.exe.asm
	diff out/.lcontext_b_debug.exe.asm out/.lcontext_c_debug.exe.asm
	diff out/.lcontext_b_size.exe.asm  out/.lcontext_c_size.exe.asm

	# The binaries should be identical too.
	diff out/lcontext_b           out/lcontext_c
	diff out/lcontext_b_debug     out/lcontext_c_debug
	diff out/lcontext_b_size      out/lcontext_c_size

	# Do not test PE files, since they differ in timestamp.
	# FIXME: find a way to erase the timestamp
	#diff out/lcontext_b.exe       out/lcontext_c.exe
	#diff out/lcontext_b_debug.exe out/lcontext_c_debug.exe
	#diff out/lcontext_b_size.exe  out/lcontext_c_size.exe
}

stage_d()
{
	if [ -z "$WINE" ] ;  then
		printf "=> ${CODE_COLOR_YELLOW}Stage D: skipped${CODE_COLOR_NOCOLOR}\n"
		return
	fi

	printf "=> ${CODE_COLOR_YELLOW}Stage D${CODE_COLOR_NOCOLOR}\n"
	mk "$WINE out/lcontext_b.exe" ctx4lnx       --linux   out/ lcontext_d
	mk "$WINE out/lcontext_b.exe" ctx4lnx       --linux   out/ lcontext_d_debug --optimize none
	mk "$WINE out/lcontext_b.exe" ctx4lnx       --linux   out/ lcontext_d_size --optimize size
	mk "$WINE out/lcontext_b.exe" ctx4win       --win32-c out/ lcontext_d.exe
	mk "$WINE out/lcontext_b.exe" ctx4win       --win32-c out/ lcontext_d_debug.exe --optimize none
	mk "$WINE out/lcontext_b.exe" ctx4win       --win32-c out/ lcontext_d_size.exe --optimize size

	# Generated assembler listings for B and D stages should be identical.
	diff out/.lcontext_b.asm           out/.lcontext_d.asm
	diff out/.lcontext_b_debug.asm     out/.lcontext_d_debug.asm
	diff out/.lcontext_b_size.asm      out/.lcontext_d_size.asm
	diff out/.lcontext_b.exe.asm       out/.lcontext_d.exe.asm
	diff out/.lcontext_b_debug.exe.asm out/.lcontext_d_debug.exe.asm
	diff out/.lcontext_b_size.exe.asm  out/.lcontext_d_size.exe.asm

	# The binaries should be identical too.
	diff out/lcontext_b           out/lcontext_d
	diff out/lcontext_b_debug     out/lcontext_d_debug
	diff out/lcontext_b_size      out/lcontext_d_size

	# Do not test PE files, since they differ in timestamp.
	# FIXME: find a way to erase the timestamp
	#diff out/lcontext_b.exe       out/lcontext_d.exe
	#diff out/lcontext_b_debug.exe out/lcontext_d_debug.exe
	#diff out/lcontext_b_size.exe  out/lcontext_d_size.exe
}

mkdir -p out

iconv -f cp866 -t utf8 < messages_cp866.ctxi > messages_utf8.ctxi

printf "=> ${CODE_COLOR_YELLOW}INFO:${CODE_COLOR_NOCOLOR}\n"
printf "\
    The precompiled compiler is used to build A.
    A is used to build B.
    B is used to build C.
    (OPTIONAL) The Windows version of B is also used to build D.
    Building B, C and D all should produce the same result both
    in the intermediate representation and the binary code.\n"

stage_a
stage_b
stage_c
stage_d


printf "=> ${CODE_COLOR_YELLOW}Building samples${CODE_COLOR_NOCOLOR}\n"

mk out/lcontext_c samples/z_t1  --win32-c out/ z_t1.exe
mk out/lcontext_c samples/z_t2  --win32-w out/ z_t2.exe
mk out/lcontext_c samples/z_t3  --win32-c out/ z_t3.exe
mk out/lcontext_c samples/z_t4  --win32-c out/ z_t4.exe
mk out/lcontext_c samples/z_t5  --win32-c out/ z_t5.exe
mk out/lcontext_c samples/z_t6  --win32-c out/ z_t6.exe

printf "=> ${CODE_COLOR_YELLOW}Running tests${CODE_COLOR_NOCOLOR}\n"

mkdir -p tests/out

do_test_with_compiler ()
{
	local failed=1
	local passed=0
	local c="$1" ; shift
	local test_name="$1" ; shift
	local condition="$1" ; shift

	local stdout_log=tests/out/"$test_name"."$c".stdout
	local stderr_log=tests/out/"$test_name"."$c".stderr

	local COMPILER_FLAGS="`grep -- 'COMPILER_FLAGS:' "tests/$f" | sed 's/^.*COMPILER_FLAGS://'`"

	if [ "x$condition" = "xcompilation_should_fail" ] ; then
		if (mk out/lcontext_"$c" tests/"$test_name"\
				--linux tests/out/ "$test_name" \
				$COMPILER_FLAGS \
				1> "$stdout_log" \
				2> "$stderr_log") ; then
			$CAT_TEST_LOGS "$stdout_log"
			$CAT_TEST_LOGS "$stderr_log"
			return $failed
		fi
		if (cat "$stdout_log" "$stderr_log" | grep -q "$1") ; then
			return $passed
		else
			$CAT_TEST_LOGS "$stdout_log"
			$CAT_TEST_LOGS "$stderr_log"
			return $failed
		fi
	elif [ "x$condition" = "xshould_print" ] ; then
		if ! (mk out/lcontext_"$c" tests/"$test_name"\
				--linux tests/out/ "$test_name" \
				$COMPILER_FLAGS \
				1> "$stdout_log" \
				2> "$stderr_log") ; then
			$CAT_TEST_LOGS "$stdout_log"
			$CAT_TEST_LOGS "$stderr_log"
			return $failed
		fi
		if [ "x`tests/out/"$test_name"`" = "x$1" ] ; then
			return $passed
		else
			$CAT_TEST_LOGS "$stdout_log"
			$CAT_TEST_LOGS "$stderr_log"
			return $failed
		fi
	else
		return $failed
	fi
}

_run_test ()
{
	do_test_with_compiler c "$@" \
	&& do_test_with_compiler c_debug "$@" \
	&& do_test_with_compiler c_size "$@"
}

do_test ()
{
	local test_name="$1"
	CAT_TEST_LOGS=true
	if _run_test "$@" ; then
		printf " => ${CODE_COLOR_GREEN}passed${CODE_COLOR_NOCOLOR} %s\n" "$test_name"
	else
		printf " => ${CODE_COLOR_RED}failed${CODE_COLOR_NOCOLOR} %s\n" "$test_name"
		CAT_TEST_LOGS=cat
		set -x
		_run_test "$@"
		set +x
		exit 1
	fi
}

do_tests ()
{
	for f in `(cd tests && grep -l  -- 'TEST:' *.* )` ; do
		local t="`basename -s .ctx  -- "$f"`"
		local c="`grep  -- 'TEST:' "tests/$f" | sed 's/^.*TEST://'`"
		eval do_test $t $c
	done
}

do_tests

printf "=> ${CODE_COLOR_YELLOW}Running valgrind --tool=callgrind${CODE_COLOR_NOCOLOR}\n"
valgrind --tool=callgrind \
	--callgrind-out-file=callgrind.out.lcontext_c \
	--dump-instr=yes \
	out/lcontext_c ctx4lnx.ctx --optimize speed --linux --output out/lcontext_c.callgrind.asm

printf "=> ${CODE_COLOR_YELLOW}Running valgrind --tool=memcheck${CODE_COLOR_NOCOLOR}\n"
valgrind --tool=memcheck \
	out/lcontext_c ctx4lnx.ctx --optimize speed --linux --output out/lcontext_c.memcheck.asm

