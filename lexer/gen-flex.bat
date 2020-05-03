@echo off

call set-env.bat

rmdir /s /q %gen_path%
mkdir %gen_path%

::start /B /D "%src_path%" /WAIT win_bison.exe -v --output=%gen_path%\y.tab.cpp --defines=%gen_path%\y.tab.h parser.y
::start /B /D "%src_path%" /WAIT win_flex.exe --outfile=%gen_path%\lex.yy.cpp --wincompat lexer.l

pushd "%gen_path%"
call win_flex.exe --wincompat %src_path%\lexer.l
popd

