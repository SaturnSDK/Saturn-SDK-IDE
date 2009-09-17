pkgdata_DATA = debugger.zip
CLEANFILES = $(pkgdata_DATA)

DEBUG_RESOURCES = backtrace.xrc \
	debugger_options_dlg.xrc \
	debugger_project_options_dlg.xrc \
	edit_watches_dlg.xrc \
	manifest.xml \
	threads.xrc

EXTRA_DIST = $(DEBUG_RESOURCES)

debugger.zip: $(DEBUG_RESOURCES)
	PWD=`pwd` cd $(srcdir) && zip $(PWD)/debugger.zip manifest.xml *.xrc > /dev/null
