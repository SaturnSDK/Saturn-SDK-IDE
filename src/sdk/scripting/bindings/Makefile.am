noinst_LTLIBRARIES = libsqbindings.la libsqbindings_con.la

INCLUDES = $(WX_CXXFLAGS) \
			-I$(top_srcdir)/src/include \
			-I$(top_srcdir)/src/sdk/wxscintilla/include \
			-I$(top_srcdir)/src/include/scripting/include \
			-I$(top_srcdir)/src/include/scripting/bindings \
			-I$(top_srcdir)/src/include/scripting/sqplus

libsqbindings_con_la_CXXFLAGS = @CXXFLAGS@ -U CB_PRECOMP -DCB_FOR_CONSOLE -Wno-invalid-pch

libsqbindings_la_CXXFLAGS = @CXXFLAGS@

CXXFLAGS =

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

libsqbindings_con_la_SOURCES = scriptbindings_base.cpp \
                            sc_consts.cpp \
                            sc_globals_base.cpp \
                            sc_io_base.cpp \
                            sc_progress.cpp \
                            sc_wxtypes.cpp
