@echo off

if "%1x" == "x" (
    set src_path=%~dp0src
    set gen_path=%~dp0gen
) else  (
    set src_path=%1..\src
    set gen_path=%1..\gen
)

echo %0 - src = %src_path%, gen = %gen_path%

rmdir /s /q %gen_path%
mkdir %gen_path%

start /B /D "%src_path%" /WAIT win_bison.exe --output=%gen_path%\y.tab.cpp --defines=%gen_path%\y.tab.h parser.y
start /B /D "%src_path%" /WAIT win_flex.exe --outfile=%gen_path%\lex.yy.cpp --wincompat lexer.l

echo const char build_date[] = ^"%date% %time%^"; > "%gen_path%\build_date.h"