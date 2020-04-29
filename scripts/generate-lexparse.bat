@echo off

if "%1x" == "x" (
    set src_path=%~dp0..\src
    set gen_path=%~dp0..\gen
) else  (
    set src_path=%1..\src
    set gen_path=%1..\gen
)

echo %0 - src = %src_path%, gen = %gen_path%

rmdir /s /q %gen_path%
mkdir %gen_path%

rem start /B /D "%src_path%" /WAIT win_bison.exe -v --output=%gen_path%\parser.tab.cpp --defines=%gen_path%\parser.tab.hh parser.yy
rem start /B /D "%src_path%" /WAIT win_flex.exe --outfile=%gen_path%\lexer.yy.cc --wincompat lexer.l

start /B /D "%gen_path%" /WAIT win_bison.exe -v --defines %src_path%\parser.yy
rem start /B /D "%src_path%" /WAIT win_flex.exe lexer.l

echo const char build_date[] = ^"%date% %time%^"; > "%gen_path%\build_date.h"