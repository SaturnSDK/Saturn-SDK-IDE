#!/bin/sh
mkdir build
cd build
../configure --host=$HOSTMACH --target=$TARGETMACH --build=$BUILDMACH --with-wx-config=wx-config-2.8 --prefix=${INSTALLDIR} --program-prefix=
make
make install
# rpath needs to be set for Code::Blocks to use the correct libcodeblocks.so
chrpath -r '${ORIGIN}/../lib' ${INSTALLDIR}/bin/codeblocks
