#!/bin/sh

rm -rf $INSTALLDIR
rm -rf build
./bootstrap
rm -rf tmp_build
mkdir -p tmp_build
cd tmp_build
../configure
cp src/include/autorevision.h ../src/include
cd ..
./build_win-ide.sh

if [[ "$CREATEINSTALLER" == "YES" ]]; then
	./createinstaller_win.sh
fi
