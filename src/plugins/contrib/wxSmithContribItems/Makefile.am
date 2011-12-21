AM_CPPFLAGS = -DWXMAKINGDLL_PROPGRID -DwxPG_USE_WXMODULE=0

INCLUDES = $(WX_CXXFLAGS) \
		-I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/sdk/wxscintilla/include \
		-I$(top_srcdir)/src/sdk/wxpropgrid/include \
		-I$(top_srcdir)/src/plugins/contrib/wxSmith \
		-I$(top_srcdir)/src/plugins/contrib/wxSmith/wxwidgets \
		-I$(top_srcdir)/src/plugins/contrib/wxSmith/wxwidgets/properties \
		-I$(top_srcdir)/src/plugins/contrib/wxSmith/properties \
		-I$(srcdir)/../wxContribItems/wxchart/include \
		-I$(srcdir)/../wxContribItems/wxFlatNotebook/include \
		-I$(srcdir)/../wxContribItems/wxImagePanel/include \
		-I$(srcdir)/../wxContribItems/wxthings/include \
		-I$(srcdir)/../wxContribItems/KWIC/include \
		-I$(srcdir)/../wxContribItems/wxSpeedButton

pluginlibdir = $(pkglibdir)/plugins

pluginlib_LTLIBRARIES = libwxsmithcontribitems.la

libwxsmithcontribitems_la_LDFLAGS = -module -version-info 0:1:0 -shared -no-undefined -avoid-version

libwxsmithcontribitems_la_LIBADD = $(PLUGIN_WX_LIBS) ../../../sdk/libcodeblocks.la \
		       ../wxSmith/libwxsmithlib.la \
			../wxContribItems/wxchart/libwxchartctrl.la \
			../wxContribItems/wxFlatNotebook/libwxflatnotebook.la \
			../wxContribItems/wxImagePanel/libwximagepanel.la \
			../wxContribItems/wxSpeedButton/libwxspeedbutton.la \
			../wxContribItems/KWIC/libwxkwic.la \
			../wxContribItems/wxthings/libwxcustombutton.la

EXTRA_DIST = $(srcdir)/wxchart/*.xpm \
		$(srcdir)/wxchart/*.png \
		$(srcdir)/wximagebutton/*.xpm \
		$(srcdir)/wximagepanel/*.xpm \
		$(srcdir)/wxspeedbutton/*.xpm \
		$(srcdir)/wxthings/*.xpm \
		$(srcdir)/wxthings/*.png \
		$(srcdir)/wxflatnotebook/images/*.xpm \
		$(srcdir)/wxflatnotebook/images/*.png \
		$(srcdir)/wxSmithKWIC/images/*.xpm \
		$(srcdir)/*.cbp \
		manifest.xml

pkgdata_DATA = wxsmithcontribitems.zip
CLEANFILES = $(pkgdata_DATA)

wxsmithcontribitems.zip: manifest.xml
	PWD=`pwd` cd $(srcdir) && zip $(PWD)/wxsmithcontribitems.zip manifest.xml > /dev/null

libwxsmithcontribitems_la_SOURCES = \
./wxflatnotebook/wxsFlatNotebook.cpp \
./wxchart/wxschart.cpp \
./wxthings/wxscustombutton.cpp \
./wximagebutton/wxsimagebutton.cpp \
./wximagepanel/wxsimagepanel.cpp \
./wxspeedbutton/wxsSpeedButton.cpp \
./wxSmithKWIC/wxsangularmeter.cpp \
./wxSmithKWIC/wxsangularregulator.cpp \
./wxSmithKWIC/wxsbmpcheckbox.cpp \
./wxSmithKWIC/wxsbmpswitcher.cpp \
./wxSmithKWIC/wxslcdclock.cpp \
./wxSmithKWIC/wxslcddisplay.cpp \
./wxSmithKWIC/wxslinearmeter.cpp \
./wxSmithKWIC/wxslinearregulator.cpp \
./wxSmithContribItems.cpp

noinst_HEADERS = \
./wxflatnotebook/wxsFlatNotebook.h \
./wxchart/wxschart.h \
./wxthings/wxscustombutton.h \
./wximagebutton/wxsimagebutton.h \
./wximagepanel/wxsimagepanel.h \
./wxspeedbutton/wxsSpeedButton.h \
./wxSmithKWIC/wxsangularmeter.h \
./wxSmithKWIC/wxsangularregulator.h \
./wxSmithKWIC/wxsbmpcheckbox.h \
./wxSmithKWIC/wxsbmpswitcher.h \
./wxSmithKWIC/wxslcdclock.h \
./wxSmithKWIC/wxslcddisplay.h \
./wxSmithKWIC/wxslinearmeter.h \
./wxSmithKWIC/wxslinearregulator.h \
./wxSmithContribItems.h
