#!/bin/sh
mkdir build
cd build
../configure --host=$HOSTMACH --target=$TARGETMACH --build=$BUILDMACH --prefix=$INSTALLDIR --program-prefix=
make
make install
