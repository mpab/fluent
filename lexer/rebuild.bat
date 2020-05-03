call gen-flex.bat
call gen-build.bat

call env-vs.bat
pushd %build_path%
msbuild lexer.sln
popd