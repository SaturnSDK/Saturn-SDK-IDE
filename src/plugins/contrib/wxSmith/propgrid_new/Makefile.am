INCLUDES = $(WX_CXXFLAGS) \
		-I$(srcdir)/contrib/include

AM_CPPFLAGS = -DWXMAKINGLIB_PROPGRID -DwxPG_USE_WXMODULE=0

noinst_LTLIBRARIES = libpropgrid.la

libpropgrid_la_LDFLAGS = -module -version-info 0:1:0 -shared -no-undefined -avoid-version

libpropgrid_la_SOURCES = ./src/advprops.cpp \
			./src/editors.cpp \
			./src/extras.cpp \
			./src/manager.cpp \
			./src/odcombo.cpp \
			./src/propgrid.cpp \
			./src/props.cpp \
			./src/xh_propgrid.cpp

EXTRA_DIST = ./README.txt \
			./CHANGES.txt


noinst_HEADERS = ./include/wx/propgrid/advprops.h \
			./include/wx/propgrid/editors.h \
			./include/wx/propgrid/extras.h \
			./include/wx/propgrid/manager.h \
			./include/wx/propgrid/odcombo.h \
			./include/wx/propgrid/pg_dox_mainpage.h \
			./include/wx/propgrid/propdev.h \
			./include/wx/propgrid/propgrid.h \
			./include/wx/propgrid/props.h \
			./include/wx/propgrid/xh_propgrid.h
