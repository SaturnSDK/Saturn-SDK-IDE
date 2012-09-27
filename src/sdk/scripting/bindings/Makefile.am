if BUILD_GUI
noinst_LTLIBRARIES = libsqbindings.la libsqbindings_con.la
else
noinst_LTLIBRARIES = libsqbindings_con.la
endif

INCLUDES = $(WX_CXXFLAGS) \
			-I$(top_srcdir)/src/include \
			-I$(top_srcdir)/src/sdk/wxscintilla/include \
			-I$(top_srcdir)/src/include/scripting/include \
			-I$(top_srcdir)/src/include/scripting/bindings \
			-I$(top_srcdir)/src/include/scripting/sqplus

libsqbindings_la_CXXFLAGS = @CXXFLAGS@

if BUILD_GUI

libsqbindings_la_SOURCES = scriptbindings.cpp \
                            scriptbindings_base.cpp \
                            sc_consts.cpp \
                            sc_dialog.cpp \
                            sc_globals.cpp \
                            sc_globals_base.cpp \
                            sc_io.cpp \
                            sc_io_base.cpp \
                            sc_plugin.cpp \
                            sc_progress.cpp \
                            sc_util_dialogs.cpp \
                            sc_wxtypes.cpp
endif

CXXFLAGS =
libsqbindings_con_la_CXXFLAGS = @HEADLESS_CXXFLAGS@

libsqbindings_con_la_SOURCES = scriptbindings_base.cpp \
                            sc_consts.cpp \
                            sc_globals_base.cpp \
                            sc_io_base.cpp \
                            sc_progress.cpp \
                            sc_wxtypes.cpp

