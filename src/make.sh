#!/bin/bash

set -e
#set -x

DEFAULT_WINE=""
if which wine > /dev/null 2> /dev/null ; then
	DEFAULT_WINE="wine"
fi

DEFAULT_VALGRIND=""
if which valgrind > /dev/null 2> /dev/null ; then
	DEFAULT_VALGRIND="valgrind"
fi

DEFAULT_BOOTSTRAP_COMPILER="../precompiled/`cat ../precompiled/latest`/lcontext_c"

export WINE="${WINE-$DEFAULT_WINE}"
export VALGRIND="${VALGRIND-$DEFAULT_VALGRIND}"

export BOOTSTRAP_COMPILER="${BOOTSTRAP_COMPILER-$DEFAULT_BOOTSTRAP_COMPILER}"

crop_known_differencies()
{
	sed "s/; #line .*//" "$1" > "$1.cleared"
}

diff_pe()
{
	# Do not test PE files with diff, since they differ in timestamp.
	# We check the number of differing bytes, and if we have too many of them,
	# something's wrong.

	echo "    DIFF  $1 $2"

	diff_bytes="`(cmp -l "$1" "$2" || true) | wc -l`"
	if [ "$diff_bytes" -lt 5 ] ; then
		return 0
	fi
	set -x
	cmp -l "$1" "$2"
}

diff_plain()
{
	echo "    DIFF  $1 $2"
	diff -u "$1" "$2"
}


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
	diff_plain out/.lcontext_b.asm           out/.lcontext_c.asm
	diff_plain out/.lcontext_b_debug.asm     out/.lcontext_c_debug.asm
	diff_plain out/.lcontext_b_size.asm      out/.lcontext_c_size.asm
	diff_plain out/.lcontext_b.exe.asm       out/.lcontext_c.exe.asm
	diff_plain out/.lcontext_b_debug.exe.asm out/.lcontext_c_debug.exe.asm
	diff_plain out/.lcontext_b_size.exe.asm  out/.lcontext_c_size.exe.asm

	# The binaries should be identical too.
	diff_plain out/lcontext_b           out/lcontext_c
	diff_plain out/lcontext_b_debug     out/lcontext_c_debug
	diff_plain out/lcontext_b_size      out/lcontext_c_size
	diff_pe out/lcontext_b.exe       out/lcontext_c.exe
	diff_pe out/lcontext_b_debug.exe out/lcontext_c_debug.exe
	diff_pe out/lcontext_b_size.exe  out/lcontext_c_size.exe
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

	# We have some minor differences between asm listings generated
	# under different platforms, so we postprocess the listings now
	# to delete those differences.
	crop_known_differencies out/.lcontext_b.asm
	crop_known_differencies out/.lcontext_b_debug.asm
	crop_known_differencies out/.lcontext_b_size.asm
	crop_known_differencies out/.lcontext_b.exe.asm
	crop_known_differencies out/.lcontext_b_debug.exe.asm
	crop_known_differencies out/.lcontext_b_size.exe.asm

	crop_known_differencies out/.lcontext_d.asm
	crop_known_differencies out/.lcontext_d_debug.asm
	crop_known_differencies out/.lcontext_d_size.asm
	crop_known_differencies out/.lcontext_d.exe.asm
	crop_known_differencies out/.lcontext_d_debug.exe.asm
	crop_known_differencies out/.lcontext_d_size.exe.asm

	# Generated assembler listings for B and D stages should be identical.
	diff_plain out/.lcontext_b.asm.cleared           out/.lcontext_d.asm.cleared
	diff_plain out/.lcontext_b_debug.asm.cleared     out/.lcontext_d_debug.asm.cleared
	diff_plain out/.lcontext_b_size.asm.cleared      out/.lcontext_d_size.asm.cleared
	diff_plain out/.lcontext_b.exe.asm.cleared       out/.lcontext_d.exe.asm.cleared
	diff_plain out/.lcontext_b_debug.exe.asm.cleared out/.lcontext_d_debug.exe.asm.cleared
	diff_plain out/.lcontext_b_size.exe.asm.cleared  out/.lcontext_d_size.exe.asm.cleared

	# The binaries should be identical too.
	diff_plain out/lcontext_b           out/lcontext_d
	diff_plain out/lcontext_b_debug     out/lcontext_d_debug
	diff_plain out/lcontext_b_size      out/lcontext_d_size
	diff_pe out/lcontext_b.exe       out/lcontext_d.exe
	diff_pe out/lcontext_b_debug.exe out/lcontext_d_debug.exe
	diff_pe out/lcontext_b_size.exe  out/lcontext_d_size.exe
}

build_samples()
{
	printf "=> ${CODE_COLOR_YELLOW}Building samples${CODE_COLOR_NOCOLOR}\n"

	mk out/lcontext_c samples/z_t1  --win32-c out/ z_t1.exe
	mk out/lcontext_c samples/z_t2  --win32-w out/ z_t2.exe
	mk out/lcontext_c samples/z_t3  --win32-c out/ z_t3.exe
	mk out/lcontext_c samples/z_t4  --win32-c out/ z_t4.exe
	mk out/lcontext_c samples/z_t5  --win32-c out/ z_t5.exe
	mk out/lcontext_c samples/z_t6  --win32-c out/ z_t6.exe
}

do_test_with_compiler ()
{
	local failed=1
	local passed=0
	local c="$1" ; shift
	local test_name="$1" ; shift
	local condition="$1" ; shift

	local tests_out_dir="tests/out/$c"
	mkdir -p "$tests_out_dir"

	local stdout_log="$tests_out_dir"/"$test_name"."$c".stdout
	local stderr_log="$tests_out_dir"/"$test_name"."$c".stderr

	local COMPILER_FLAGS="`grep -- 'COMPILER_FLAGS:' "tests/$f" | sed 's/^.*COMPILER_FLAGS://'`"

	if [ "x$condition" = "xcompilation_should_fail" ] ; then
		if (mk out/lcontext_"$c" tests/"$test_name"\
				--linux "$tests_out_dir"/ "$test_name" \
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
				--linux "$tests_out_dir"/ "$test_name" \
				$COMPILER_FLAGS \
				1> "$stdout_log" \
				2> "$stderr_log") ; then
			$CAT_TEST_LOGS "$stdout_log"
			$CAT_TEST_LOGS "$stderr_log"
			return $failed
		fi
		if [ "x`"$tests_out_dir"/"$test_name"`" = "x$1" ] ; then
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
	do_test_with_compiler a "$@" \
	&& do_test_with_compiler c "$@" \
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

concat_list()
{
	if [ -z "$1" ] ; then
		printf "%s" "$3"
	else
		printf "%s%s%s" "$1$2$3"
	fi
}


do_tests ()
{
	printf "=> ${CODE_COLOR_YELLOW}Running tests${CODE_COLOR_NOCOLOR}\n"

	mkdir -p tests/out

	passed_tests=
	failed_tests=
	expected_to_fail_tests=
	passed_expected_to_fail_tests=
	nr_passed=0
	nr_failed=0
	nr_expected_to_fail=0
	nr_passed_expected_to_fail=0
	for f in `(cd tests && grep -l  -- 'TEST:' *.* )` ; do
		local t="`basename -s .ctx  -- "$f"`"
		local c="`grep  -- 'TEST:' "tests/$f" | sed 's/^.*TEST://'`"
		local expected_to_fail="`grep -- 'EXPECTED-TO-FAIL:' "tests/$f"`"
		if ! (eval do_test $t $c) ; then
			if [ -z "$expected_to_fail" ] ; then
				failed_tests="`concat_list "$failed_tests" ", " "$t"`"
				nr_failed=`expr $nr_failed + 1`
			else
				expected_to_fail_tests="`concat_list "$expected_to_fail_tests" ", " "$t"`"
				nr_expected_to_fail=`expr $nr_expected_to_fail + 1`
			fi
		else
			if [ -z "$expected_to_fail" ] ; then
				passed_tests="`concat_list "$passed_tests" ", " "$t"`"
				nr_passed=`expr $nr_passed + 1`
			else
				passed_expected_to_fail_tests="`concat_list "$passed_expected_to_fail_tests" ", " "$t"`"
				nr_passed_expected_to_fail=`expr $nr_passed_expected_to_fail + 1`
			fi
		fi
	done

	printf " ${CODE_COLOR_GREEN}Passed tests${CODE_COLOR_NOCOLOR}: %s\n" "$nr_passed"
	if [ "$nr_expected_to_fail" -ne 0 ] ; then
		printf " ${CODE_COLOR_YELLOW}Expected to fail${CODE_COLOR_NOCOLOR}: %s (%s)\n" "$nr_expected_to_fail" "$expected_to_fail_tests"
	fi
	if [ "$nr_passed_expected_to_fail" -ne 0 ] ; then
		printf " ${CODE_COLOR_PURPLE}Expected to fail, but passed sucessfully${CODE_COLOR_NOCOLOR}: %s (%s)\n" "$nr_passed_expected_to_fail" "$passed_expected_to_fail_tests"
	fi
	if [ "$nr_failed" -ne 0 ] ; then
		printf " ${CODE_COLOR_RED}Failed tests (regressions)${CODE_COLOR_NOCOLOR}: %s (%s)\n" "$nr_failed" "$failed_tests"
		return 1
	fi
	return 0
}

run_valgrind()
{
	if [ -z "$VALGRIND" ] ;  then
		printf "=> ${CODE_COLOR_YELLOW}Running valgrind: skipped${CODE_COLOR_NOCOLOR}\n"
		return
	fi

	printf "=> ${CODE_COLOR_YELLOW}Running valgrind --tool=callgrind${CODE_COLOR_NOCOLOR}\n"
	"$VALGRIND" --tool=callgrind \
		--callgrind-out-file=callgrind.out.lcontext_c \
		--dump-instr=yes \
		out/lcontext_c ctx4lnx.ctx --optimize speed --linux --output out/lcontext_c.callgrind.asm

	printf "=> ${CODE_COLOR_YELLOW}Running valgrind --tool=memcheck${CODE_COLOR_NOCOLOR}\n"
	"$VALGRIND" --tool=memcheck \
		out/lcontext_c ctx4lnx.ctx --optimize speed --linux --output out/lcontext_c.memcheck.asm
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
build_samples
do_tests
run_valgrind

if [ "x$1" = "x--save-to-precompiled" ] ; then
	printf "=> ${CODE_COLOR_YELLOW}Saving binaries:${CODE_COLOR_NOCOLOR}\n"
	date_tag="`date -u +%F`"
	rel_tag=1
	save_dir=""
	save_path=""
	while true ; do
		if [ "$rel_tag" = 1 ] ; then
			save_dir="$date_tag"
		else
			save_dir="$date_tag-$rel_tag"
		fi
		save_path="../precompiled/$save_dir"
		if [ ! -e "$save_path" ] ; then
			break
		fi
		rel_tag=`expr $rel_tag + 1`
	done

	printf "    Target path: $save_path\n"

	mkdir "$save_path"
	cp -a \
		out/lcontext_c \
		out/lcontext_c_debug \
		out/lcontext_c_debug.exe \
		out/lcontext_c.exe \
		"$save_path"
	chmod a-x "$save_path"/*.exe

	echo "$save_dir" > "../precompiled/latest.tmp" && mv "../precompiled/latest.tmp" "../precompiled/latest"

	printf "    Binaries: `ls "$save_path" | tr "\n" " "`\n"
fi

