@echo off
cd src
cbp2make --local --config show
cbp2make --local -in CodeBlocks.cbp -out Makefile -windows -targets "All" --target-case keep
sed.exe -i "s/\*/\\*/g" Makefile
sed.exe -i -e "s/$(LIB)-l/$(LIB) -l/g" Makefile
sed.exe -i -e "s/devel\\libcodeblocks.a/devel\\codeblocks.dll/g" Makefile
sed.exe -i -e "/^LIB_SDK =/ s/$/ $(LIB)/" Makefile
mingw32-make clean
mingw32-make
if %errorlevel% == 0 goto build_ok
cd %ROOTDIR%
echo "MinGW make failed"
exit /b %errorlevel%
:build_ok
copy /Y %WX_LIB%\gcc_dll\wxmsw28u_gcc_custom.dll devel
update.bat
cd %ROOTDIR%
