cd src
cbp2make --local --config show
cbp2make --local -in CodeBlocks.cbp -out Makefile -windows -targets "All" --target-case keep
%GNUWIN32%\bin\sed.exe -i "s/\*/\\*/g" Makefile
%GNUWIN32%\bin\sed.exe -i -e "s/$(LIB)-l/$(LIB) -l/g" Makefile
%GNUWIN32%\bin\sed.exe -i -e "s/devel\\libcodeblocks.a/devel\\codeblocks.dll/g" Makefile
%GNUWIN32%\bin\sed.exe -i -e "/^LIB_SDK =/ s/$/ $(LIB)/" Makefile
for /f %%i in ('echo %MINGW32% ^| sed "s/\\/\\\\/g"') do set MINGW32PATH=%%i
%GNUWIN32%\bin\sed.exe -i -e "s/zip -/%MINGW32PATH%\\bin\\zip.exe -/" Makefile
exit /b 0
mingw32-make clean
if not %errorlevel% 0
{
	cd %ROOTDIR%
	echo "MinGW make failed"
	exit /b %errorlevel%
}

mingw32-make
if not %errorlevel% 0
{
	cd %ROOTDIR%
	echo "MinGW make failed"
	exit /b %errorlevel%
}
copy /Y %WX_LIB%\gcc_dll\wxmsw28u_gcc_custom.dll devel
update.bat
cd %ROOTDIR%
