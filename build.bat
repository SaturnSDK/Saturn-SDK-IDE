build-ide.bat

if not %errorlevel% 0
{
	cd %ROOTDIR%
	echo "Failed building Code::Blocks"
	exit /b %errorlevel%
}

@cd %ROOTDIR%

@if "%CREATEINSTALLER%" == "YES" goto installer
@exit /b 0

:installer
@rmdir /q /s %INSTALLDIR%
@mkdir %INSTALLDIR%
@xcopy %ROOTDIR%\src\output %INSTALLDIR% /s /e /i /h /y
@createinstaller.bat

