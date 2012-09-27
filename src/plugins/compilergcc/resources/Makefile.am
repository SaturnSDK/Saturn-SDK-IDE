if BUILD_GUI
plugindatadir = $(datadir)/@PACKAGE@
plugindata_DATA = compiler.zip
endif

conplugindatadir = $(datadir)/@PACKAGE@_con


conplugindata_DATA = compiler.zip

CLEANFILES = $(plugindata_DATA) $(conplugindata_DATA)

if BUILD_GUI

    EXTRA_DIST = advanced_compiler_options.xrc \
	compiler_menu.xrc \
	compiler_options.xrc \
	compiler_toolbar_16x16.xrc \
	compiler_toolbar.xrc \
	manifest.xml \
	images/compile.png \
	images/compilerun.png \
	images/rebuild.png \
	images/run.png \
	images/stop.png \
	images/16x16/compile.png \
	images/16x16/compilerun.png \
	images/16x16/rebuild.png \
	images/16x16/run.png \
	images/16x16/stop.png

compiler.zip: $(EXTRA_DIST)
	PWD=`pwd` cd $(srcdir) && zip $(PWD)/compiler.zip manifest.xml *.xrc images/*.png images/16x16/*.png > /dev/null

else

compiler.zip:
	PWD=`pwd` cd $(srcdir) && zip $(PWD)/compiler.zip manifest.xml > /dev/null

endif
