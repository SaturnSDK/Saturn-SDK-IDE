pluginlibdir = $(pkglibdir)_con/plugins

INCLUDES = $(WX_CXXFLAGS) \
		-I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/include/scripting/include \
		-I$(top_srcdir)/src/include/scripting/sqplus \
		-I$(top_srcdir)/src/plugins/compilergcc/depslib/src

AM_CXXFLAGS = -DDEPSLIB_UNIX


pluginlib_LTLIBRARIES = libcompiler.la

CXXFLAGS =
libcompiler_la_CXXFLAGS = @HEADLESS_CXXFLAGS@

libcompiler_la_LDFLAGS = -module -version-info 0:1:0 -shared -no-undefined -avoid-version

libcompiler_la_LIBADD = $(PLUGIN_WX_LIBS) ../../../sdk/libcodeblocks_con.la \
				../depslib/src/libdepslib.la

libcompiler_la_SOURCES = ../compilerBCC.cpp \
			../compiler_defs.cpp \
			../compilerCYGWIN.cpp \
			../compilerDMC.cpp \
			../compilerDMD.cpp \
			../compilererrors.cpp \
			../compilergcc.cpp \
			../compilerGDC.cpp \
			../compilerLDC.cpp \
			../compilerICC.cpp \
			../compilerLCC.cpp \
			../compilerMINGW.cpp \
			../compilerMINGWgenerator.cpp \
			../compilerMSVC.cpp \
			../compilerOW.cpp \
			../compilerOWgenerator.cpp \
			../compilerSDCC.cpp \
			../compilerG95.cpp \
			../compilerGNUARM.cpp \
			../compilerGNUAVR.cpp \
			../compilerGNUFortran.cpp \
			../compilerGNUMSP430.cpp \
			../compilerGNUPOWERPC.cpp \
			../compilerGNUTRICORE.cpp \
			../compilerTcc.cpp \
			../directcommands.cpp

