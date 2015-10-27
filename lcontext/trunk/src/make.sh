set -e
set -x

mk()
{
    $1 $2.ctx $3 --output $4.$5.asm --output-tree $4.$5.tree --output-include-list $4.$5.includes $6 $7
    fasm  $4.$5.asm $4$5
}


mk ../bin/lcontext_c ctx4lnx    --linux out/ lcontext_a

mk out/lcontext_a ctx4lnx       --linux out/ lcontext_b
mk out/lcontext_b ctx4lnx       --linux out/ lcontext_c

mk out/lcontext_b ctx4lnx       --linux out/ lcontext_c_debug --optimize none

mk out/lcontext_c ctx4win       --win32-c out/ lcontext_c.exe
mk out/lcontext_c ctx4win       --win32-c out/ lcontext_c_debug.exe --optimize none

mk out/lcontext_c samples/z_t1  --win32-c out/ z_t1.exe
mk out/lcontext_c samples/z_t2  --win32-w out/ z_t2.exe
mk out/lcontext_c samples/z_t3  --win32-c out/ z_t3.exe
mk out/lcontext_c samples/z_t4  --win32-c out/ z_t4.exe
mk out/lcontext_c samples/z_t5  --win32-c out/ z_t5.exe
mk out/lcontext_c samples/z_t6  --win32-c out/ z_t6.exe
