mkdir build
cd build
../configure --host=$HOSTMACH --target=$TARGETMACH --build=$BUILDMACH --prefix=$INSTALLDIR
make -j${NPROC}
make install -j${NPROC}

