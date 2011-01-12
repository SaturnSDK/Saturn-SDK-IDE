AM_CPPFLAGS = -DWXMAKINGDLL_PROPGRID -DwxPG_USE_WXMODULE=0

INCLUDES = $(WX_CXXFLAGS) \
		-I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/plugins/contrib/wxSmith \
		-I$(top_srcdir)/src/sdk/wxpropgrid/include \
		-I$(top_srcdir)/src/plugins/contrib/wxSmithContribItems/wximagepanel/wxImagePanel/include

wxsmithcontribitemsdir=$(libdir)/wxSmithContribItems

wxsmithcontribitems_LTLIBRARIES = libwximagepanel.la

libwximagepanel_la_LDFLAGS = -version-info 0:1:0

libwximagepanel_la_LIBADD = $(WX_LIBS)

libwximagepanel_la_SOURCES = \
./src/wxImagePanel.cpp

sdkdir=$(includedir)/wxSmithContribItems/wximagepanel/wxImagePanel/include/wx

sdk_HEADERS = \
./include/wx/wxImagePanel.h

