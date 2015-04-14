@echo off
uname -o >nul
if %errorlevel% == 0 goto check_cygwin
goto not_cygwin
:check_cygwin
for /f %%i in ('uname -o') do (set UNAME_OS=%%i)
if not "%UNAME_OS%" == "Cygwin" goto not_cygwin
:set_path
cygpath -pw "%ORIGINAL_PATH%" > original_path
set /p PATH=<original_path
del original_path
echo PATH set to:
echo %PATH%
:not_cygwin

build-ide.bat

if not %errorlevel% == 0
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

