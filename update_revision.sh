#!/bin/sh

#
# This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
# http://www.gnu.org/licenses/gpl-3.0.html
#
# $Revision$
# $Id$
# $HeadURL: svn+ssh://jenslody@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/src/dlgaboutplugin.cpp $
#

REV_FILE=./revision.m4

# let's import OLD_REV (if there)
if [ -f ./.last_revision ]; then
	. ./.last_revision
else
	OLD_REV=0
fi

if svn --xml info >/dev/null 2>&1; then
	REV=`svn --xml info | tr -d '\r\n' | sed -e 's/.*<commit.*revision="\([0-9]*\)".*<\/commit>.*/\1/'`
	LCD=`svn --xml info | tr -d '\r\n' | sed -e 's/.*<commit.*<date>\([0-9\-]*\)\T\([0-9\:]*\)\..*<\/date>.*<\/commit>.*/\1 \2/'`
elif svn --version --quiet >/dev/null 2>&1; then
	REV=`svn info | grep "^Revision:" | cut -d" " -f2`
	LCD=`svn info | grep "^Last Changed Date:" | cut -d" " -f4,5`
else
	REV=0
	LCD=""
fi

if [ "x$REV" != "x$OLD_REV" -o ! -r $REV_FILE ]; then
	echo "m4_define([SVN_REV], $REV)" > $REV_FILE
	echo "m4_define([SVN_REVISION], 8.02svn$REV)" >> $REV_FILE
	echo "m4_define([SVN_DATE], $LCD)" >> $REV_FILE

	# Also change the revision number in debian/changelog for package versioning
	mv debian/changelog debian/changelog.tmp
	sed "1 s/(8.02svn[^-)]*/(8.02svn$REV/" < debian/changelog.tmp > debian/changelog
	rm debian/changelog.tmp
fi

echo "OLD_REV=$REV" > ./.last_revision

exit 0
