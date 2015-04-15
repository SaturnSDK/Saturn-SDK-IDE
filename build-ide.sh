#!/bin/sh
mkdir build
cd build
../configure --host=$HOSTMACH --target=$TARGETMACH --build=$BUILDMACH --prefix=$INSTALLDIR --with-wx-config=wx-config-2.8 --program-prefix=
make
make install
