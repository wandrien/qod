#!/bin/bash

set -e
set -x

mk()
{
    $1 $2.ctx $3 --output $4.$5.asm --output-tree $4.$5.tree --output-include-list $4.$5.includes $6 $7 && \
    fasm  $4.$5.asm $4$5
}

iconv -f cp866 -t utf8 < messages_cp866.ctxi > messages_utf8.ctxi

mk ../bin/lcontext_c ctx4lnx    --linux out/ lcontext_a

mk out/lcontext_a ctx4lnx       --linux out/ lcontext_b
mk out/lcontext_b ctx4lnx       --linux out/ lcontext_c

mk out/lcontext_b ctx4lnx       --linux out/ lcontext_c_debug --optimize none

mk out/lcontext_c ctx4win       --win32-c out/ lcontext_c.exe
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
	echo " => failed $*"
	exit 1;
}

passed ()
{
	echo " => passed $*"
}


do_test ()
{
	if [ "x$1" = "xcompilation_should_fail" ] ; then
		if (mk out/lcontext_c tests/"$2"  --linux tests/out/ "$2" 1> tests/out/"$2".stdout 2> tests/out/"$2".stderr) ; then
			failed "$2"
		fi
		if (cat tests/out/"$2".stdout tests/out/"$2".stderr | grep -q "$3") ; then
			passed "$2"
		else
			failed "$2"
		fi
	fi
}

set +x

do_test compilation_should_fail div_0 'Попытка деления на ноль'
do_test compilation_should_fail div_0_2 'Попытка деления на ноль'
do_test compilation_should_fail div_0_3 'Попытка деления на ноль'
do_test compilation_should_fail wrong_continue 'continue вне цикла'
do_test compilation_should_fail wrong_exit 'exit вне цикла'


