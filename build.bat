pushd vs2015
msbuild /p:Configuration="Release" /p:Platform="x64"
msbuild /p:Configuration="Debug" /p:Platform="x64"

copy x64\Debug\fluent.exe C:\Apps\bin\dfluent.exe
copy x64\Release\fluent.exe C:\Apps\bin\fluent.exe

popd
