@echo off


call :mk ..\bin\lcontext.exe    ctx4win --win32-c out\ lcontext_a.exe

call :mk out\lcontext_a.exe ctx4win       --win32-c out\ lcontext_b.exe
call :mk out\lcontext_b.exe ctx4win       --win32-c out\ lcontext_c.exe
call :mk out\lcontext_c.exe ctx4lnx       --linux   out\ lcontext_c

call :mk out\lcontext_c.exe samples\z_t1  --win32-c out\ z_t1.exe
call :mk out\lcontext_c.exe samples\z_t2  --win32-w out\ z_t2.exe
call :mk out\lcontext_c.exe samples\z_t3  --win32-c out\ z_t3.exe
call :mk out\lcontext_c.exe samples\z_t4  --win32-c out\ z_t4.exe
rem out\z_t4.exe > out\z_t4_1.ctx
rem call :mk out\lcontext_c.exe out\z_t4_1    --win32-c out\ z_t4_1.exe

goto :end


:mk
echo %2 ==^> %4%5
%1    %2.ctx %3 --output %4.%5.asm --output-tree %4.%5.tree --output-include-list %4.%5.includes
if not %ERRORLEVEL%==0 goto err
fasm  %4.%5.asm %4%5 > nul
if not %ERRORLEVEL%==0 goto err
exit /b


:err
echo.
echo Error code %ERRORLEVEL%
:end
rem exit