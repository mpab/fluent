@echo off
pushd vs
call env-vs
call msbuild fluent.sln
popd