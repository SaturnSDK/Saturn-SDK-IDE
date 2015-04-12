@for /f %%i in ('git describe --tags ^| sed -e "s/_[0-9].*//"') do @set TAG_NAME=%%i

@for /f %%i in ('git describe --match "%TAG_NAME%_[0-9]*" HEAD ^| sed -e "s/-g.*//" -e "s/%%TAG_NAME%%_//"') do @set VERSION_NUM = %%i

@for /f %%i in ('git log --pretty^=format:"%%ci" -1 ^| sed -e "s/ [^ ]*%%//g"') do @set GIT_DATE=%%i

@for /f %%i in ('echo %VERSION_NUM% ^| sed "s/-[^.]*$//" ^| sed -r "s/.[^.]*$//" ^| sed -r "s/.[^.]*$//"') do @set MAJOR_BUILD_NUM=%%i

@for /f %%i in ('echo %VERSION_NUM% ^| sed "s/-[^.]*$//" ^| sed -r "s/.[^.]*$//" ^| sed -r "s/.[.]*//"') do @set MINOR_BUILD_NUM=%%i

@for /f %%i in ('echo %VERSION_NUM% ^| sed "s/-[^.]*$//" ^| sed -r "s/.*(.[0-9].)//"') do @set REVISION_BUILD_NUM=%%i

@for /f %%i in ('echo %VERSION_NUM% ^| sed -e "s/[0-9].[0-9].[0-9]//" -e "s/-//"') do @set BUILD_NUM=%%i

@if "%TAG_NAME%" == "" set TAG_NAME=unknown
@if "%MAJOR_BUILD_NUM%" == "" set MAJOR_BUILD_NUM=0
@if "%MINOR_BUILD_NUM%" == "" set MINOR_BUILD_NUM=0
@if "%REVISION_BUILD_NUM%" == "" set REVISION_BUILD_NUM=0
@if "%BUILD_NUM%" == "" set BUILD_NUM=0

@mkdir %ROOTDIR%\installerpackage\org.opengamedevelopers.sega.saturn.sdk.ide > nul 2> nul
@mkdir %ROOTDIR%\installerpackage\config > nul 2> nul
@mkdir %ROOTDIR%\installerpackage\org.opengamedevelopers.sega.saturn.sdk.ide\data > nul 2> nul
@mkdir %ROOTDIR%\installerpackage\org.opengamedevelopers.sega.saturn.sdk.ide\meta > nul 2> nul

@set PACKAGEXML=%ROOTDIR%\installerpackage\org.opengamedevelopers.sega.saturn.sdk.ide\meta\package.xml

@echo ^<?xml version="1.0" encoding="UTF-8"?^> > %PACKAGEXML%
@echo ^<Package^> >> %PACKAGEXML%
@echo	^<DisplayName^>SEGA Saturn SDK IDE^</DisplayName^> >> %PACKAGEXML%
@echo	^<Description^>Code::Blocks IDE customised for the SEGA Saturn^</Description^> >> %PACKAGEXML%
@echo	^<Version^>%MAJOR_BUILD_NUM%.%MINOR_BUILD_NUM%.%REVISION_BUILD_NUM%.%BUILD_NUM%^</Version^> >> %PACKAGEXML%
@echo	^<Name^>org.opengamedevelopers.sega.saturn.sdk.ide^</Name^> >> %PACKAGEXML%
@echo	^<ReleaseDate^>%GIT_DATE%^</ReleaseDate^> >> %PACKAGEXML%
@echo	^<Licenses^> >> %PACKAGEXML%
@echo		^<License name="GNU Public License Ver. 3" file="gplv3.txt" /^> >> %PACKAGEXML%
@echo	^</Licenses^> >> %PACKAGEXML%
@echo ^</Package^> >> %PACKAGEXML%

@wget -q -c -O %ROOTDIR%\installerpackage\org.opengamedevelopers.sega.saturn.sdk.ide\meta\gplv3.txt http://www.gnu.org/licenses/gpl-3.0.txt

@echo Packaging %INSTALLDIR ...

@%QTIFWDIR%\bin\archivegen %ROOTDIR%\installerpackage\org.opengamedevelopers.sega.saturn.sdk.ide\data\directory.7z %INSTALLDIR%

@echo OK!

@rmdir /q /s %ROOTDIR%\installerpackage\ide

@%QTIFWDIR%\bin\repogen.exe -p %ROOTDIR%\installerpackage -i org.opengamedevelopers.sega.saturn.sdk.ide %ROOTDIR%\installerpackage\ide
