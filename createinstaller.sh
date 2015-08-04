#!/bin/bash
set -e

export TAG_NAME=`git describe --tags | sed -e 's/_[0-9].*//'`
export VERSION_NUM=`git describe --match "${TAG_NAME}_[0-9]*" HEAD | sed -e 's/-g.*//' -e "s/${TAG_NAME}_//"`
export MAJOR_BUILD_NUM=`echo $VERSION_NUM | sed 's/-[^.]*$//' | sed -r 's/.[^.]*$//' | sed -r 's/.[^.]*$//'`
export MINOR_BUILD_NUM=`echo $VERSION_NUM | sed 's/-[^.]*$//' | sed -r 's/.[^.]*$//' | sed -r 's/.[.]*//'`
export REVISION_BUILD_NUM=`echo $VERSION_NUM | sed 's/-[^.]*$//' | sed -r 's/.*(.[0-9].)//'`
export BUILD_NUM=`echo $VERSION_NUM | sed -e 's/[0-9].[0-9].[0-9]//' -e 's/-//'`

if [ -z $TAG_NAME ]; then
	TAG_NAME=unknown
fi

if [ -z $MAJOR_BUILD_NUM ]; then
	MAJOR_BUILD_NUM=0
fi

if [ -z $MINOR_BUILD_NUM ]; then
	MINOR_BUILD_NUM=0
fi

if [ -z $REVISION_BUILD_NUM ]; then
	REVISION_BUILD_NUM=0
fi

if [ -z $BUILD_NUM ]; then
	BUILD_NUM=0
fi

mkdir -p $ROOTDIR/installerpackage/{org.opengamedevelopers.sega.saturn.sdk.ide/{data,meta},config}

cat > $ROOTDIR/installerpackage/org.opengamedevelopers.sega.saturn.sdk.ide/meta/package.xml << __EOF__
<?xml version="1.0" encoding="UTF-8"?>
<Package>
	<DisplayName>SEGA Saturn SDK IDE</DisplayName>
	<Description>Code::Blocks IDE customised for the SEGA Saturn</Description>
	<Version>${MAJOR_BUILD_NUM}.${MINOR_BUILD_NUM}.${REVISION_BUILD_NUM}.${BUILD_NUM}</Version>
	<Name>org.opengamedevelopers.sega.saturn.sdk.ide</Name>
	<ReleaseDate>`git log --pretty=format:"%ci" -1 | sed -e 's/ [^ ]*$//g'`</ReleaseDate>
	<Script>installscript.qs</Script>
	<Licenses>
		<License name="GNU Public License Ver. 3" file="gplv3.txt" />
	</Licenses>
</Package>
__EOF__

cat > $ROOTDIR/installerpackage/org.opengamedevelopers.sega.saturn.sdk.ide/meta/installscript.qs << __EOF__
function Component( )
{
}

Component.prototype.createOperations = function( )
{
	component.createOperations( );

	if( installer.value( "os" ) === "x11" )
	{
		component.addOperation( "Move", "@TargetDir@/ide/SaturnIDE.desktop",
			"@homeDir@/.local/share/applications/SaturnIDE.desktop" );
		component.addOperation( "Replace", "@homeDir@/.local/share/applications/SaturnIDE.desktop",
			"SATURN_ROOT", "@TargetDir@" );
	}

	if( installer.value( "os" ) === "win" )
	{
		component.addOperation( "CreateShortcut", "@TargetDir@/ide/codeblocks.exe",
			"@StartMenuDir@/Saturn SDK IDE.lnk", "iconPath=@TargetDir@/ide/codeblocks.exe"
			"iconId=1" );
	}
}
__EOF__

cat > ${INSTALLDIR}/SaturnIDE.desktop << __EOF__
[Desktop Entry]
Name=SEGA Saturn SDK IDE
GenericName=SEGA Saturn Tool
Comment=Code::Blocks IDE customised for the SEGA Saturn SDK
Exec=SATURN_ROOT/ide/bin/codeblocks
Icon=SATURN_ROOT/ide/share/pixmaps/codeblocks.png
Terminal=false
Type=Application
Categories=Development;
__EOF__

wget -c -O $ROOTDIR/installerpackage/org.opengamedevelopers.sega.saturn.sdk.ide/meta/gplv3.txt https://www.gnu.org/licenses/gpl-3.0.txt

printf "Packaging $INSTALLDIR ... "
$QTIFWDIR/bin/archivegen $ROOTDIR/installerpackage/org.opengamedevelopers.sega.saturn.sdk.ide/data/directory.7z $INSTALLDIR
if [ $? -ne "0" ]; then
	printf "FAILED\n"
	exit 1
fi

printf "OK\n"

rm -rf $ROOTDIR/installerpackage/ide

$QTIFWDIR/bin/repogen -p $ROOTDIR/installerpackage -i org.opengamedevelopers.sega.saturn.sdk.ide $ROOTDIR/installerpackage/ide

