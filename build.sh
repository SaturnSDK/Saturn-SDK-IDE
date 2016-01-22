#!/bin/sh
export NCPU=`nproc`

rm -rf $INSTALLDIR
rm -rf build

./bootstrap

./build-ide.sh

if [ $? -ne 0 ]; then
	echo "Failed building Code::Blocks"
	exit 1
fi


if [[ "$CREATEINSTALLER" == "YES" ]]; then
	./createinstaller.sh
fi
