#!/bin/bash

################################################################################
## CONFIGURATION
################################################################################

set -e
#set -x

# Autodetect presence of the optional tools
DEFAULT_WINE=""
if which wine > /dev/null 2> /dev/null ; then
	DEFAULT_WINE="wine"
fi

DEFAULT_VALGRIND=""
if which valgrind > /dev/null 2> /dev/null ; then
	DEFAULT_VALGRIND="valgrind"
fi

# Bundled tools
DEFAULT_BOOTSTRAP_COMPILER="../precompiled/`cat ../precompiled/latest`/qodc"
DEFAULT_FASM="../3rd-party/fasm/fasm"

# Default names for external tools
DEFAULT_NASM="nasm"
DEFAULT_LD="ld"

# Set tool names, respecting any user-provided values
export BOOTSTRAP_COMPILER="${BOOTSTRAP_COMPILER:-$DEFAULT_BOOTSTRAP_COMPILER}"
export FASM="${FASM:-$DEFAULT_FASM}"
export NASM="${NASM:-$DEFAULT_NASM}"
export LD="${LD:-$DEFAULT_LD}"
export WINE="${WINE-$DEFAULT_WINE}"
export VALGRIND="${VALGRIND-$DEFAULT_VALGRIND}"

# Options
export BUILD_EXE="${BUILD_EXE:-y}"
export BUILD_SAMPLES="${BUILD_SAMPLES:-y}"
export BOOTSTRAP_ASSEMBLER_MODE="${BOOTSTRAP_ASSEMBLER_MODE:-fasm}"
export ASSEMBLER_MODE="${ASSEMBLER_MODE:-fasm}"
export EXE_ASSEMBLER_MODE="${EXE_ASSEMBLER_MODE:-fasm}"
export QOD_FLAGS="${QOD_FLAGS:-}"
export TESTS_QOD_FLAGS="${TESTS_QOD_FLAGS:-}"

# Paths
BUILD_DIR="../build"
COMPILER_BUILD_DIR="$BUILD_DIR/compiler"
TESTS_BUILD_DIR="$BUILD_DIR/tests"
SAMPLES_BUILD_DIR="$BUILD_DIR/samples"

################################################################################

compiler_name="qodc"
compiler_a="$COMPILER_BUILD_DIR/${compiler_name}_a"
compiler_b="$COMPILER_BUILD_DIR/${compiler_name}_b"
compiler_c="$COMPILER_BUILD_DIR/${compiler_name}_c"
compiler_d="$COMPILER_BUILD_DIR/${compiler_name}_d"

STAGE_A_ONLY=f

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

diff_elf()
{
	echo "    DIFF  $1 $2"

	test -f "$1.stripped" && rm "$1.stripped"
	test -f "$2.stripped" && rm "$2.stripped"

	strip -s -o "$1.stripped" "$1" 2>/dev/null | true
	strip -s -o "$2.stripped" "$2" 2>/dev/null | true

	local f1="$1"
	local f2="$2"
	if [ -e "$1.stripped" -a -e "$2.stripped" ] ; then
		local f1="$1.stripped"
		local f2="$2.stripped"
	fi

	if cmp "$f1" "$f2" >/dev/null 2>&1 ; then
		return 0
	fi
	set -x
	cmp -l "$f1" "$f2"
}

diff_plain()
{
	echo "    DIFF  $1 $2"
	diff -u "$1" "$2"
}

assemble_and_link()
{
	local mode="$1"
	local asm="$2"
	local dst="$3"
	case "$mode" in
	fasm)
	    $FASM -m 200000 "$asm" "$dst" >/dev/null && chmod a+x "$dst"
		;;
	nasm)
		$NASM --reproducible -Ox -g -f elf32 "$asm" -o "$dst.o"
		$LD -m elf_i386 -static -o "$dst" "$dst.o"
		;;
	esac
}

mk()
{
	local compiler="$1" ; shift
	local src="$1" ; shift
	local platform="$1" ; shift
	local dst="$1" ; shift

	local assembler_mode=""
	if [ "$dst" = "$compiler_a" ] ; then
		assembler_mode="$BOOTSTRAP_ASSEMBLER_MODE"
	elif [ "$platform" = "--win32-c" ] ; then
		assembler_mode="$EXE_ASSEMBLER_MODE"
	else
		assembler_mode="$ASSEMBLER_MODE"
	fi

	local asm_opt=""
	if [ "$assembler_mode" != "fasm" ] ; then
		asm_opt="--asm $assembler_mode"
	fi

	echo "    BUILD $dst"
	mkdir -p "`dirname "$dst"`"
	$compiler $src.qd\
		$platform \
		--optimize speed \
		--output "$dst".asm \
		--output-tree "$dst".tree \
		--output-include-list "$dst".includes \
		--emit-source-line-notes \
		$asm_opt \
		$QOD_FLAGS \
		"$@" && \

	assemble_and_link "$assembler_mode" "$dst.asm" "$dst"
}

stage_a()
{
	printf "=> ${CODE_COLOR_YELLOW}Stage A${CODE_COLOR_NOCOLOR}\n"
	mk $BOOTSTRAP_COMPILER ctx4lnx --linux "$compiler_a"
}

stage_b()
{
	printf "=> ${CODE_COLOR_YELLOW}Stage B${CODE_COLOR_NOCOLOR}\n"
	mk "$compiler_a" ctx4lnx --linux   "$compiler_b"
	mk "$compiler_a" ctx4lnx --linux   "$compiler_b"_debug --optimize none
	mk "$compiler_a" ctx4lnx --linux   "$compiler_b"_size  --optimize size
	if [ "$BUILD_EXE" = y ] ; then
		mk "$compiler_a" ctx4win --win32-c "$compiler_b".exe
		mk "$compiler_a" ctx4win --win32-c "$compiler_b"_debug.exe --optimize none
		mk "$compiler_a" ctx4win --win32-c "$compiler_b"_size.exe --optimize size
	fi
}

stage_c()
{
	printf "=> ${CODE_COLOR_YELLOW}Stage C${CODE_COLOR_NOCOLOR}\n"
	mk "$compiler_b" ctx4lnx --linux   "$compiler_c" #--warn-unused-globals
	mk "$compiler_b" ctx4lnx --linux   "$compiler_c"_debug --optimize none
	mk "$compiler_b" ctx4lnx --linux   "$compiler_c"_size --optimize size
	if [ "$BUILD_EXE" = y ] ; then
		mk "$compiler_b" ctx4win --win32-c "$compiler_c".exe #--warn-unused-globals
		mk "$compiler_b" ctx4win --win32-c "$compiler_c"_debug.exe --optimize none
		mk "$compiler_b" ctx4win --win32-c "$compiler_c"_size.exe --optimize size
	fi

	# Generated assembler listings for B and C stages should be identical.
	diff_plain "$compiler_b".asm           "$compiler_c".asm
	diff_plain "$compiler_b"_debug.asm     "$compiler_c"_debug.asm
	diff_plain "$compiler_b"_size.asm      "$compiler_c"_size.asm
	if [ "$BUILD_EXE" = y ] ; then
		diff_plain "$compiler_b".exe.asm       "$compiler_c".exe.asm
		diff_plain "$compiler_b"_debug.exe.asm "$compiler_c"_debug.exe.asm
		diff_plain "$compiler_b"_size.exe.asm  "$compiler_c"_size.exe.asm
	fi

	# The binaries should be identical too.
	diff_elf "$compiler_b"           "$compiler_c"
	diff_elf "$compiler_b"_debug     "$compiler_c"_debug
	diff_elf "$compiler_b"_size      "$compiler_c"_size
	if [ "$BUILD_EXE" = y ] ; then
		diff_pe "$compiler_b".exe       "$compiler_c".exe
		diff_pe "$compiler_b"_debug.exe "$compiler_c"_debug.exe
		diff_pe "$compiler_b"_size.exe  "$compiler_c"_size.exe
	fi
}

stage_d()
{
	if [ -z "$WINE" -o "$BUILD_EXE" != y ] ;  then
		printf "=> ${CODE_COLOR_YELLOW}Stage D: skipped${CODE_COLOR_NOCOLOR}\n"
		return
	fi

	printf "=> ${CODE_COLOR_YELLOW}Stage D${CODE_COLOR_NOCOLOR}\n"
	mk "$WINE $compiler_b" ctx4lnx --linux   "$compiler_d"
	mk "$WINE $compiler_b" ctx4lnx --linux   "$compiler_d"_debug --optimize none
	mk "$WINE $compiler_b" ctx4lnx --linux   "$compiler_d"_size --optimize size
	mk "$WINE $compiler_b" ctx4win --win32-c "$compiler_d".exe
	mk "$WINE $compiler_b" ctx4win --win32-c "$compiler_d"_debug.exe --optimize none
	mk "$WINE $compiler_b" ctx4win --win32-c "$compiler_d"_size.exe --optimize size

	# We have some minor differences between asm listings generated
	# under different platforms, so we postprocess the listings now
	# to delete those differences.
	crop_known_differencies "$compiler_b".asm
	crop_known_differencies "$compiler_b"_debug.asm
	crop_known_differencies "$compiler_b"_size.asm
	crop_known_differencies "$compiler_b".exe.asm
	crop_known_differencies "$compiler_b"_debug.exe.asm
	crop_known_differencies "$compiler_b"_size.exe.asm

	crop_known_differencies "$compiler_d".asm
	crop_known_differencies "$compiler_d"_debug.asm
	crop_known_differencies "$compiler_d"_size.asm
	crop_known_differencies "$compiler_d".exe.asm
	crop_known_differencies "$compiler_d"_debug.exe.asm
	crop_known_differencies "$compiler_d"_size.exe.asm

	# Generated assembler listings for B and D stages should be identical.
	diff_plain "$compiler_b".asm.cleared           "$compiler_d".asm.cleared
	diff_plain "$compiler_b"_debug.asm.cleared     "$compiler_d"_debug.asm.cleared
	diff_plain "$compiler_b"_size.asm.cleared      "$compiler_d"_size.asm.cleared
	diff_plain "$compiler_b".exe.asm.cleared       "$compiler_d".exe.asm.cleared
	diff_plain "$compiler_b"_debug.exe.asm.cleared "$compiler_d"_debug.exe.asm.cleared
	diff_plain "$compiler_b"_size.exe.asm.cleared  "$compiler_d"_size.exe.asm.cleared

	# The binaries should be identical too.
	diff_elf "$compiler_b"           "$compiler_d"
	diff_elf "$compiler_b"_debug     "$compiler_d"_debug
	diff_elf "$compiler_b"_size      "$compiler_d"_size
	diff_pe "$compiler_b".exe       "$compiler_d".exe
	diff_pe "$compiler_b"_debug.exe "$compiler_d"_debug.exe
	diff_pe "$compiler_b"_size.exe  "$compiler_d"_size.exe
}

build_samples()
{
	if [ "$BUILD_SAMPLES" != y ] ;  then
		printf "=> ${CODE_COLOR_YELLOW}Building samples skipped${CODE_COLOR_NOCOLOR}\n"
		return
	fi

	printf "=> ${CODE_COLOR_YELLOW}Building samples${CODE_COLOR_NOCOLOR}\n"

	mk "$compiler_c" samples/z_t1  --win32-c "$SAMPLES_BUILD_DIR/z_t1.exe"
	mk "$compiler_c" samples/z_t2  --win32-w "$SAMPLES_BUILD_DIR/z_t2.exe"
	mk "$compiler_c" samples/z_t3  --win32-c "$SAMPLES_BUILD_DIR/z_t3.exe"
	mk "$compiler_c" samples/z_t4  --win32-c "$SAMPLES_BUILD_DIR/z_t4.exe"
	mk "$compiler_c" samples/z_t5  --win32-c "$SAMPLES_BUILD_DIR/z_t5.exe"
	mk "$compiler_c" samples/z_t6  --win32-c "$SAMPLES_BUILD_DIR/z_t6.exe"
}

do_test_with_compiler ()
{
	local failed=1
	local passed=0
	local c="$1" ; shift
	local test_name="$1" ; shift
	local condition="$1" ; shift

	local tests_out_dir="$TESTS_BUILD_DIR/$c"
	mkdir -p "$tests_out_dir"

	local stdout_log="$tests_out_dir"/"$test_name".stdout
	local stderr_log="$tests_out_dir"/"$test_name".stderr

	local COMPILER_FLAGS="`grep -- 'COMPILER_FLAGS:' "tests/$f" | sed 's/^.*COMPILER_FLAGS://'`"

	local COMPILER="$COMPILER_BUILD_DIR/${compiler_name}_$c"

	if [ "x$condition" = "xcompilation_should_fail" ] ; then
		if (mk "$COMPILER" tests/"$test_name"\
				--linux "$tests_out_dir/$test_name" \
				$TESTS_QOD_FLAGS \
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
		if ! (mk "$COMPILER" tests/"$test_name"\
				--linux "$tests_out_dir/$test_name" \
				$TESTS_QOD_FLAGS \
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
	if [ "$STAGE_A_ONLY" = t ] ; then
		do_test_with_compiler a "$@"
	else
		do_test_with_compiler a "$@" \
		&& do_test_with_compiler c "$@" \
		&& do_test_with_compiler c_debug "$@" \
		&& do_test_with_compiler c_size "$@"
	fi
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

	passed_tests=
	failed_tests=
	expected_to_fail_tests=
	passed_expected_to_fail_tests=
	nr_passed=0
	nr_failed=0
	nr_expected_to_fail=0
	nr_passed_expected_to_fail=0
	for f in `(cd tests && grep -l  -- 'TEST:' *.* )` ; do
		local t="`basename -s .qd  -- "$f"`"
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

	for optim in speed size none ; do
		local callgrind_out="$compiler_c".$optim.callgrind.out
		local asm="$compiler_c".callgrind.$optim.asm
		printf "=> ${CODE_COLOR_YELLOW}Running valgrind --tool=callgrind${CODE_COLOR_NOCOLOR} (--optimize $optim)\n"
		"$VALGRIND" --tool=callgrind \
			--callgrind-out-file="$callgrind_out" \
			--dump-instr=yes \
			"$compiler_c" ctx4lnx.qd --optimize $optim --linux --output "$asm"
		sed -i 's|build/compiler//../build/compiler/|build/compiler/|' "$callgrind_out"
	done

	printf "=> ${CODE_COLOR_YELLOW}Running valgrind --tool=memcheck${CODE_COLOR_NOCOLOR}\n"
	"$VALGRIND" --tool=memcheck \
		"$compiler_c" ctx4lnx.qd --optimize speed --linux --output "$compiler_c".memcheck.asm
}

iconv -f cp866 -t utf8 < messages_cp866.qdi > messages_utf8.qdi

printf "=> ${CODE_COLOR_YELLOW}INFO:${CODE_COLOR_NOCOLOR}\n"
printf "\
    The precompiled compiler is used to build A.
    A is used to build B.
    B is used to build C.
    (OPTIONAL) The Windows version of B is also used to build D.
    Building B, C and D all should produce the same result both
    in the intermediate representation and the binary code.\n"

if [ "x$1" = "xtest-stage-a" ] ; then
	STAGE_A_ONLY=t
	stage_a
	do_tests
else
	stage_a
	stage_b
	stage_c
	stage_d
	build_samples
	do_tests
	run_valgrind
fi


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
		"$compiler_c" \
		"$compiler_c"_debug \
		"$compiler_c"_debug.exe \
		"$compiler_c".exe \
		"$save_path" && \
	(
		cd "$save_path" && \
		mv "$compiler_name"_c "$compiler_name" && \
		mv "$compiler_name"_c_debug "$compiler_name"_debug && \
		mv "$compiler_name"_c_debug.exe "$compiler_name"_debug.exe && \
		mv "$compiler_name"_c.exe "$compiler_name".exe
	)
	chmod a-x "$save_path"/*.exe

	echo "$save_dir" > "../precompiled/latest.tmp" && mv "../precompiled/latest.tmp" "../precompiled/latest"

	printf "    Binaries: `ls "$save_path" | tr "\n" " "`\n"
fi

