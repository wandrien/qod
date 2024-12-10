@echo off

set /p LATEST=<..\precompiled\latest
set BOOTSTRAP_COMPILER="..\precompiled\"%LATEST%"\qodc.exe"
set FASM="..\3rd-party\fasm\FASM.exe"

set BUILD_DIR=..\build
set COMPILER_BUILD_DIR=%BUILD_DIR%\compiler
set SAMPLES_BUILD_DIR=%BUILD_DIR%\samples

mkdir %BUILD_DIR%
mkdir %COMPILER_BUILD_DIR%
mkdir %SAMPLES_BUILD_DIR%

set compiler_name=qodc
set compiler_a=%COMPILER_BUILD_DIR%\%compiler_name%_a
set compiler_b=%COMPILER_BUILD_DIR%\%compiler_name%_b
set compiler_c=%COMPILER_BUILD_DIR%\%compiler_name%_c
set compiler_d=%COMPILER_BUILD_DIR%\%compiler_name%_d


call :mk %BOOTSTRAP_COMPILER% ctx4win --win32-c %compiler_a%.exe

call :mk %compiler_a%.exe ctx4win --win32-c %compiler_b%.exe
call :mk %compiler_b%.exe ctx4win --win32-c %compiler_c%.exe
call :mk %compiler_c%.exe ctx4lnx --linux   %compiler_c%
call :mk %compiler_c%.exe ctx4win --win32-c %compiler_c%_debug.exe --optimize none

call :mk %compiler_c%.exe samples\z_t1  --win32-c %SAMPLES_BUILD_DIR%\z_t1.exe
call :mk %compiler_c%.exe samples\z_t2  --win32-w %SAMPLES_BUILD_DIR%\z_t2.exe
call :mk %compiler_c%.exe samples\z_t3  --win32-c %SAMPLES_BUILD_DIR%\z_t3.exe
call :mk %compiler_c%.exe samples\z_t4  --win32-c %SAMPLES_BUILD_DIR%\z_t4.exe
rem out\z_t4.exe > out\z_t4_1.ctx
rem call :mk out\lcontext_c.exe out\z_t4_1    --win32-c out\ z_t4_1.exe --show-parse-progress
call :mk %compiler_c%.exe samples\z_t5  --win32-c %SAMPLES_BUILD_DIR%\z_t5.exe

goto :end


:mk
echo %2 ==^> %4.asm
%1 %2.ctx %3 --output %4.asm --output-tree %4.tree --output-include-list %4.includes %5 %6 %7 %8 %9
if not %ERRORLEVEL%==0 goto err
%FASM% -m 200000 %4.asm %4 > nul
if not %ERRORLEVEL%==0 goto err
exit /b


:err
echo.
echo Error code %ERRORLEVEL%
:end
rem exit
