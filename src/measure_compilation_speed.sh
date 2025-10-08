#!/bin/bash

O="/tmp/qodc.tmp.asm"

repetitions=30

run_test() {
	for a in {1..30} ; do
		../build/compiler/qodc_c --optimize none  --output "$O" qodc.qd
	done
}

LANG=C

sleep 2
T="`(time run_test) 2>&1 | grep real | grep -P -o '(?<=0m)[0-9.]+'`"

lines=`cloc --json --read-lang-def=../extra/qod.cloc-lang-def --exclude-dir=tests,samples . | jq .Qod.code`

total_lines=$(($lines * $repetitions))

lines_per_second="`echo "$total_lines / $T"| bc`"
time_per_repetition="`echo "scale = 3; $T / $repetitions"| bc | sed 's/^[.]/0./'`"

echo "total_lines         = $total_lines"
echo "total time          = $T"
echo "time per repetition = $time_per_repetition"
echo "lines per second    = $lines_per_second"
