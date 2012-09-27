if !BUILD_GUI
SUBDIRS = base build_tools sdk src plugins scripts
else
SUBDIRS = include base build_tools mime sdk src plugins tools templates scripts wxsmith

dist_pkgdata_DATA = tips.txt
endif

EXTRA_DIST = CodeBlocks.cbp \
		CodeBlocks-unix.cbp \
		ContribPlugins.workspace \
		ContribPlugins-unix.workspace \
		exchndl.dll \
		update \
		update.bat \
		sdk.doxy
