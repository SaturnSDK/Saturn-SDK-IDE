#!/bin/sh

rm -rf $INSTALLDIR
rm -rf build
./bootstrap
mkdir -p build
cd build
../configure
cp src/include/autorevision.h ../src/include
cd ${ROOTDIR}
./build_win-ide.sh

if [[ "$CREATEINSTALLER" == "YES" ]]; then
	./createinstaller_win.sh
fi
