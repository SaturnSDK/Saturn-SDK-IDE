EXEEXT=".exe"
LIBEXT="dll"

echo Creating output directory tree

mkdir -p output/share/CodeBlocks/compilers
mkdir -p output/share/CodeBlocks/lexers
mkdir -p output/share/CodeBlocks/images/settings
mkdir -p output/share/CodeBlocks/images/codecompletion
mkdir -p output/share/CodeBlocks/images/12x12
mkdir -p output/share/CodeBlocks/images/16x16
mkdir -p output/share/CodeBlocks/plugins
mkdir -p output/share/CodeBlocks/templates
mkdir -p output/share/CodeBlocks/templates/wizard
mkdir -p output/share/CodeBlocks/scripts
mkdir -p devel/share/CodeBlocks/compilers
mkdir -p devel/share/CodeBlocks/lexers
mkdir -p devel/share/CodeBlocks/images/settings
mkdir -p devel/share/CodeBlocks/images/codecompletion
mkdir -p devel/share/CodeBlocks/images/12x12
mkdir -p devel/share/CodeBlocks/images/16x16
mkdir -p devel/share/CodeBlocks/plugins
mkdir -p devel/share/CodeBlocks/templates
mkdir -p devel/share/CodeBlocks/templates/wizard
mkdir -p devel/share/CodeBlocks/scripts

ZIPCMD="zip"
RESDIR="devel/share/CodeBlocks"
echo Compressing core UI resources
${ZIPCMD} -jqu9 ${RESDIR}/resources.zip src/resources/*.xrc > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/manager_resources.zip sdk/resources/*.xrc sdk/resources/images/*.png > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/start_here.zip src/resources/start_here/* > /dev/null
echo Compressing plugins UI resources
${ZIPCMD} -jqu9 ${RESDIR}/astyle.zip plugins/astyle/resources/manifest.xml plugins/astyle/resources/*.xrc > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/autosave.zip plugins/autosave/manifest.xml plugins/autosave/*.xrc > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/classwizard.zip plugins/classwizard/resources/manifest.xml plugins/classwizard/resources/*.xrc > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/codecompletion.zip plugins/codecompletion/resources/manifest.xml plugins/codecompletion/resources/*.xrc > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/compiler.zip plugins/compilergcc/resources/manifest.xml plugins/compilergcc/resources/*.xrc > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/debugger.zip plugins/debuggergdb/resources/manifest.xml plugins/debuggergdb/resources/*.xrc > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/defaultmimehandler.zip plugins/defaultmimehandler/resources/manifest.xml plugins/defaultmimehandler/resources/*.xrc > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/occurrenceshighlighting.zip plugins/occurrenceshighlighting/resources/*.xrc plugins/occurrenceshighlighting/resources/manifest.xml > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/openfileslist.zip plugins/openfileslist/manifest.xml > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/projectsimporter.zip plugins/projectsimporter/resources/*.xrc plugins/projectsimporter/resources/manifest.xml > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/scriptedwizard.zip plugins/scriptedwizard/resources/manifest.xml > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/todo.zip plugins/todo/resources/manifest.xml plugins/todo/resources/*.xrc > /dev/null
${ZIPCMD} -jqu9 ${RESDIR}/abbreviations.zip plugins/abbreviations/resources/manifest.xml plugins/abbreviations/resources/*.xrc > /dev/null
echo Packing core UI bitmaps
cd src/resources
${ZIPCMD} -0 -qu ../../${RESDIR}/resources.zip images/*.png images/16x16/*.png images/12x12/*.png > /dev/null
cd ../../sdk/resources
${ZIPCMD} -0 -qu ../../${RESDIR}/manager_resources.zip images/*.png images/16x16/*.png images/12x12/*.png > /dev/null
echo Packing plugins UI bitmaps
cd ../../plugins/compilergcc/resources
${ZIPCMD} -0 -qu ../../../${RESDIR}/compiler.zip images/*.png images/16x16/*.png > /dev/null
cd ../../../plugins/debuggergdb/resources
${ZIPCMD} -0 -qu ../../../${RESDIR}/debugger.zip images/*.png images/16x16/*.png > /dev/null
cd ../../..

echo Copying files
# Create an exclude pattern file
echo .svn > excludes.txt
echo Makefile >> excludes.txt
echo Makefile.am >> excludes.txt
echo Makefile.in >> excludes.txt
cp -f sdk/resources/lexers/lexer_* ${RESDIR}/lexers > /dev/null
cp -f src/resources/images/*.png ${RESDIR}/images > /dev/null
cp -f src/resources/images/settings/*.png ${RESDIR}/images/settings > /dev/null
cp -f src/resources/images/16x16/*.png ${RESDIR}/images/16x16 > /dev/null
cp -f plugins/codecompletion/resources/images/*.png ${RESDIR}/images/codecompletion > /dev/null
cp -f plugins/compilergcc/resources/compilers/*.xml ${RESDIR}/compilers > /dev/null

cp -f ${RESDIR}/*.zip output/share/CodeBlocks > /dev/null
cp -f sdk/resources/lexers/lexer_* output/share/CodeBlocks/lexers > /dev/null
cp -f src/resources/images/*.png output/share/CodeBlocks/images > /dev/null
cp -f src/resources/images/settings/*.png output/share/CodeBlocks/images/settings > /dev/null
cp -f src/resources/images/16x16/*.png output/share/CodeBlocks/images/16x16 > /dev/null
cp -f plugins/codecompletion/resources/images/*.png output/share/CodeBlocks/images/codecompletion > /dev/null
cp -f plugins/compilergcc/resources/compilers/*.xml output/share/CodeBlocks/compilers > /dev/null

# several contrib plugins
if [ -d "devel/share/CodeBlocks/images/codesnippets" ]
then
  mkdir -p output/share/CodeBlocks/images/codesnippets
  cp -f devel/share/CodeBlocks/images/codesnippets/*.png output/share/CodeBlocks/images/codesnippets > /dev/null
fi
if [ -d "devel/share/CodeBlocks/images/DoxyBlocks" ]
then
  mkdir -p output/share/CodeBlocks/images/DoxyBlocks
  cp -f devel/share/CodeBlocks/images/DoxyBlocks/*.png       output/share/CodeBlocks/images/DoxyBlocks > /dev/null
  mkdir -p output/share/CodeBlocks/images/DoxyBlocks/16x16
  cp -f devel/share/CodeBlocks/images/DoxyBlocks/16x16/*.png output/share/CodeBlocks/images/DoxyBlocks/16x16 > /dev/null
fi
if [ -d "devel/share/CodeBlocks/images/ThreadSearch" ]
then
  mkdir -p output/share/CodeBlocks/images/ThreadSearch
  cp -f devel/share/CodeBlocks/images/ThreadSearch/*.png output/share/CodeBlocks/images/ThreadSearch > /dev/null
  mkdir -p output/share/CodeBlocks/images/ThreadSearch/16x16
  cp -f devel/share/CodeBlocks/images/ThreadSearch/16x16/*.png output/share/CodeBlocks/images/ThreadSearch/16x16 > /dev/null
  mkdir -p output/share/CodeBlocks/images/ThreadSearch/22x22
  cp -f devel/share/CodeBlocks/images/ThreadSearch/22x22/*.png output/share/CodeBlocks/images/ThreadSearch/22x22 > /dev/null
fi
if [ -d "devel/share/CodeBlocks/images/wxsmith" ]
then
  mkdir -p output/share/CodeBlocks/images/wxsmith
  cp -f devel/share/CodeBlocks/images/wxsmith/*.png output/share/CodeBlocks/images/wxsmith > /dev/null
fi
if [ -d "devel/share/CodeBlocks/lib_finder" ]
then
  mkdir -p output/share/CodeBlocks/lib_finder
  cp -f devel/share/CodeBlocks/lib_finder/*.xml output/share/CodeBlocks/lib_finder > /dev/null
fi
# misc. contrib plugin settings:
cp -f devel/share/CodeBlocks/images/16x16/*.png    output/share/CodeBlocks/images/16x16    > /dev/null
cp -f devel/share/CodeBlocks/images/settings/*.png output/share/CodeBlocks/images/settings > /dev/null

rsync -au --exclude-from=excludes.txt plugins/scriptedwizard/resources/ ${RESDIR}/templates/wizard > /dev/null
rsync -au --exclude-from=excludes.txt plugins/scriptedwizard/resources/ output/share/CodeBlocks/templates/wizard > /dev/null
rsync -au --exclude-from=excludes.txt templates/common/ output/share/CodeBlocks/templates > /dev/null
rsync -au --exclude-from=excludes.txt templates/unix/ output/share/CodeBlocks/templates > /dev/null
rsync -au --exclude-from=excludes.txt templates/common/ ${RESDIR}/templates > /dev/null
rsync -au --exclude-from=excludes.txt templates/unix/ ${RESDIR}/templates > /dev/null
cp -f scripts/*.script ${RESDIR}/scripts > /dev/null
cp -f scripts/*.script output/share/CodeBlocks/scripts > /dev/null
cp -f scripts/*.gdb ${RESDIR}/scripts > /dev/null
cp -f scripts/*.gdb output/share/CodeBlocks/scripts > /dev/null
cp -f tips.txt devel/share/CodeBlocks > /dev/null
cp -f tips.txt output/share/CodeBlocks > /dev/null
cp -f tools/ConsoleRunner/cb_console_runner${EXEEXT} output > /dev/null
cp -f tools/ConsoleRunner/cb_console_runner${EXEEXT} devel > /dev/null
cp -f devel/codeblocks${EXEEXT} output > /dev/null
if [ -f "devel/cb_share_config${EXEEXT}" ]
then
  cp -f devel/cb_share_config${EXEEXT} output > /dev/null
fi
if [ -f "devel/codesnippets${EXEEXT}" ]
then
  cp -f devel/codesnippets${EXEEXT} output > /dev/null
fi
cp -f devel/*.${LIBEXT} output > /dev/null
cp -f ${RESDIR}/plugins/*.${LIBEXT} output/share/CodeBlocks/plugins > /dev/null
# Now remove the exclude pattern file
rm excludes.txt

echo Stripping debug info from output tree
strip output/codeblocks${EXEEXT} > /dev/null
if [ -f "output/cb_share_config${EXEEXT}" ]
then
  strip output/cb_share_config${EXEEXT} > /dev/null
fi
strip output/cb_console_runner${EXEEXT} > /dev/null
if [ -f "output/codesnippets${EXEEXT}" ]
then
  strip output/codesnippets${EXEEXT} > /dev/null
fi
strip output/*.${LIBEXT} > /dev/null
strip output/share/CodeBlocks/plugins/*.${LIBEXT} > /dev/null

if [ "x$MSYSTEM" = "x" ] ; then
  echo Creating launch-scripts
  echo "#!/bin/sh" > output/run.sh
  echo 'APP_DIR=`dirname "$0"`' >> output/run.sh
  echo 'APP_DIR=`( cd "$APP_DIR" && pwd )`' >> output/run.sh
  echo "export LD_LIBRARY_PATH=\$APP_DIR:\$LD_LIBRARY_PATH" >> output/run.sh
  echo "\$APP_DIR/CodeBlocks \$@" >> output/run.sh
  chmod +x output/run.sh
  echo "#!/bin/sh" > devel/run.sh
  echo 'APP_DIR=`dirname "$0"`' >> devel/run.sh
  echo 'APP_DIR=`( cd "$APP_DIR" && pwd )`' >> devel/run.sh
  echo "export LD_LIBRARY_PATH=\$APP_DIR:\$LD_LIBRARY_PATH" >> devel/run.sh
  echo "\$APP_DIR/CodeBlocks \$@" >> devel/run.sh
  chmod +x devel/run.sh
fi

ZIPCMD=
RESDIR=
