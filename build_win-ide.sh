#!/bin/sh

cd src
rm -rf cbp2make.cfg
cbp2make --local --config show
cbp2make --local --config platform -windows -pwd "pwd" -mdf "mkdir -p \$dir" -rm "rm \$file" -rmf "rm -f \$file" -cp "cp -p \$src \$dst" -mv "mv \$src \$dst" -md "mkdir -p \$dir" -rmd "rm -rf \$dir"
sed -i -e 's|path_delimiter="\\"|path_delimiter="/"|g' cbp2make.cfg
sed -i -e 's/test_make_dir="if not exist $dir md $dir"/test_make_dir="test -d $dir || mkdir -p $dir"/g' cbp2make.cfg
sed -i -e 's/static_lib_ext="lib a"/static_lib_ext="a lib"/g' cbp2make.cfg
sed -i -e 's/program="gcc.exe"/program="i686-w64-mingw32-gcc"/g' cbp2make.cfg
sed -i -e 's/program="g++.exe"/program="i686-w64-mingw32-g++"/g' cbp2make.cfg
sed -i -e 's/program="ar.exe"/program="i686-w64-mingw32-ar"/g' cbp2make.cfg
sed -i -e 's/program="windres.exe"/program="i686-w64-mingw32-windres"/g' cbp2make.cfg
cbp2make --local -in CodeBlocks.cbp -out Makefile -windows -targets "All" --target-case keep
sed -i -e "s|cmd /c test|test|g" Makefile
sed -i "s|\\\|/|g" Makefile
sed -i "s|//|/|g" Makefile
sed -i -e "s/\$(LIB)-l/\$(LIB) -l/g" Makefile
sed -i -e "s|devel/libcodeblocks.a|devel/codeblocks.dll|g" Makefile
sed -i -e "/^LIB_SDK =/ s/$/ \$(LIB)/" Makefile
sed -i -e "s|cmd /c if not exist|test -d|g" Makefile
sed -i -e "s/ md / || mkdir -p /g" Makefile
sed -i -e "s/mkdir dev/|| mkdir -p dev/g" Makefile
sed -i -e 's|cmd /c "cd|sh -c "cd|g' Makefile
sed -i -e "s|cmd /c xcopy /D /Y|cp -rv|g" Makefile
sed -i -e "s|cmd /c||g" Makefile
sed -i -e 's|	build_tools/autorevision/autorevision +wx +int +t . include/autorevision.h|	#|g' Makefile
sed -i -e 's/\& zip/\&\& zip/g' Makefile
rm -rf .objs
rm -rf devel
make
