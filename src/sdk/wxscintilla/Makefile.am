INCLUDES = $(WX_CXXFLAGS) \
			-I$(srcdir)/include \
			-I$(srcdir)/src/scintilla/include \
			-I$(srcdir)/src/scintilla/lexlib \
			-I$(srcdir)/src/scintilla/src

AM_CPPFLAGS = -DSCI_LEXER -DLINK_LEXERS -DGTK -D__WX__

noinst_LTLIBRARIES = libwxscintilla.la

libwxscintilla_la_SOURCES = src/PlatWX.cpp \
			src/ScintillaWX.cpp \
			src/wxscintilla.cpp \
			src/scintilla/lexer/LexAPDL.cxx \
			src/scintilla/lexer/LexASY.cxx \
			src/scintilla/lexer/LexAU3.cxx \
			src/scintilla/lexer/LexAVE.cxx \
			src/scintilla/lexer/LexAbaqus.cxx \
			src/scintilla/lexer/LexAda.cxx \
			src/scintilla/lexer/LexAsm.cxx \
			src/scintilla/lexer/LexAsn1.cxx \
			src/scintilla/lexer/LexBaan.cxx \
			src/scintilla/lexer/LexBash.cxx \
			src/scintilla/lexer/LexBasic.cxx \
			src/scintilla/lexer/LexBullant.cxx \
			src/scintilla/lexer/LexCLW.cxx \
			src/scintilla/lexer/LexCOBOL.cxx \
			src/scintilla/lexer/LexCPP.cxx \
			src/scintilla/lexer/LexCSS.cxx \
			src/scintilla/lexer/LexCaml.cxx \
			src/scintilla/lexer/LexCmake.cxx \
			src/scintilla/lexer/LexConf.cxx \
			src/scintilla/lexer/LexCrontab.cxx \
			src/scintilla/lexer/LexCsound.cxx \
			src/scintilla/lexer/LexD.cxx \
			src/scintilla/lexer/LexEScript.cxx \
			src/scintilla/lexer/LexEiffel.cxx \
			src/scintilla/lexer/LexErlang.cxx \
			src/scintilla/lexer/LexFlagship.cxx \
			src/scintilla/lexer/LexForth.cxx \
			src/scintilla/lexer/LexFortran.cxx \
			src/scintilla/lexer/LexGAP.cxx \
			src/scintilla/lexer/LexGui4Cli.cxx \
			src/scintilla/lexer/LexHTML.cxx \
			src/scintilla/lexer/LexHaskell.cxx \
			src/scintilla/lexer/LexInno.cxx \
			src/scintilla/lexer/LexKix.cxx \
			src/scintilla/lexer/LexLisp.cxx \
			src/scintilla/lexer/LexLout.cxx \
			src/scintilla/lexer/LexLua.cxx \
			src/scintilla/lexer/LexMMIXAL.cxx \
			src/scintilla/lexer/LexMPT.cxx \
			src/scintilla/lexer/LexMSSQL.cxx \
			src/scintilla/lexer/LexMagik.cxx \
			src/scintilla/lexer/LexMarkdown.cxx \
			src/scintilla/lexer/LexMatlab.cxx \
			src/scintilla/lexer/LexMetapost.cxx \
			src/scintilla/lexer/LexMySQL.cxx \
			src/scintilla/lexer/LexNimrod.cxx \
			src/scintilla/lexer/LexNsis.cxx \
			src/scintilla/lexer/LexOpal.cxx \
			src/scintilla/lexer/LexOthers.cxx \
			src/scintilla/lexer/LexPB.cxx \
			src/scintilla/lexer/LexPLM.cxx \
			src/scintilla/lexer/LexPOV.cxx \
			src/scintilla/lexer/LexPS.cxx \
			src/scintilla/lexer/LexPascal.cxx \
			src/scintilla/lexer/LexPerl.cxx \
			src/scintilla/lexer/LexPowerPro.cxx \
			src/scintilla/lexer/LexPowerShell.cxx \
			src/scintilla/lexer/LexProgress.cxx \
			src/scintilla/lexer/LexPython.cxx \
			src/scintilla/lexer/LexR.cxx \
			src/scintilla/lexer/LexRebol.cxx \
			src/scintilla/lexer/LexRuby.cxx \
			src/scintilla/lexer/LexSML.cxx \
			src/scintilla/lexer/LexSQL.cxx \
			src/scintilla/lexer/LexScriptol.cxx \
			src/scintilla/lexer/LexSmalltalk.cxx \
			src/scintilla/lexer/LexSorcus.cxx \
			src/scintilla/lexer/LexSpecman.cxx \
			src/scintilla/lexer/LexSpice.cxx \
			src/scintilla/lexer/LexTACL.cxx \
			src/scintilla/lexer/LexTADS3.cxx \
			src/scintilla/lexer/LexTAL.cxx \
			src/scintilla/lexer/LexTCL.cxx \
			src/scintilla/lexer/LexTeX.cxx \
			src/scintilla/lexer/LexTxt2tags.cxx \
			src/scintilla/lexer/LexVB.cxx \
			src/scintilla/lexer/LexVHDL.cxx \
			src/scintilla/lexer/LexVerilog.cxx \
			src/scintilla/lexer/LexYAML.cxx \
			src/scintilla/lexerlib/Accessor.cxx \
			src/scintilla/lexerlib/CharacterSet.cxx \
			src/scintilla/lexerlib/LexerBase.cxx \
			src/scintilla/lexerlib/LexerModule.cxx \
			src/scintilla/lexerlib/LexerNoExceptions.cxx \
			src/scintilla/lexerlib/LexerSimple.cxx \
			src/scintilla/lexerlib/PropSetSimple.cxx \
			src/scintilla/lexerlib/StyleContext.cxx \
			src/scintilla/lexerlib/WordList.cxx \
			src/scintilla/src/AutoComplete.cxx \
			src/scintilla/src/CallTip.cxx \
			src/scintilla/src/Catalogue.cxx \
			src/scintilla/src/CellBuffer.cxx \
			src/scintilla/src/CharClassify.cxx \
			src/scintilla/src/ContractionState.cxx \
			src/scintilla/src/Decoration.cxx \
			src/scintilla/src/Document.cxx \
			src/scintilla/src/Editor.cxx \
			src/scintilla/src/ExternalLexer.cxx \
			src/scintilla/src/Indicator.cxx \
			src/scintilla/src/KeyMap.cxx \
			src/scintilla/src/LineMarker.cxx \
			src/scintilla/src/PerLine.cxx \
			src/scintilla/src/PositionCache.cxx \
			src/scintilla/src/RESearch.cxx \
			src/scintilla/src/RunStyles.cxx \
			src/scintilla/src/ScintillaBase.cxx \
			src/scintilla/src/Selection.cxx \
			src/scintilla/src/Style.cxx \
			src/scintilla/src/StyleContext.cxx \
			src/scintilla/src/UniConversion.cxx \
			src/scintilla/src/ViewStyle.cxx \
			src/scintilla/src/XPM.cxx

noinst_HEADERS = src/PlatWX.h \
			src/ScintillaWX.h \
			src/scintilla/include/ILexer.h \
			src/scintilla/include/Platform.h \
			src/scintilla/include/SciLexer.h \
			src/scintilla/include/Scintilla.h \
			src/scintilla/include/ScintillaWidget.h \
			src/scintilla/lexlib/Accessor.h \
			src/scintilla/lexlib/CharacterSet.h \
			src/scintilla/lexlib/LexAccessor.h \
			src/scintilla/lexlib/LexerBase.h \
			src/scintilla/lexlib/LexerModule.h \
			src/scintilla/lexlib/LexerNoExceptions.h \
			src/scintilla/lexlib/LexerSimple.h \
			src/scintilla/lexlib/OptionSet.h \
			src/scintilla/lexlib/PropSetSimple.h \
			src/scintilla/lexlib/StyleContext.h \
			src/scintilla/lexlib/WordList.h \
			src/scintilla/src/AutoComplete.h \
			src/scintilla/src/CallTip.h \
			src/scintilla/src/Catalogue.h \
			src/scintilla/src/CellBuffer.h \
			src/scintilla/src/CharClassify.h \
			src/scintilla/src/ContractionState.h \
			src/scintilla/src/Decoration.h \
			src/scintilla/src/Document.h \
			src/scintilla/src/Editor.h \
			src/scintilla/src/ExternalLexer.h \
			src/scintilla/src/FontQuality.h \
			src/scintilla/src/Indicator.h \
			src/scintilla/src/KeyMap.h \
			src/scintilla/src/LineMarker.h \
			src/scintilla/src/Partitioning.h \
			src/scintilla/src/PerLine.h \
			src/scintilla/src/PositionCache.h \
			src/scintilla/src/RESearch.h \
			src/scintilla/src/RunStyles.h \
			src/scintilla/src/ScintillaBase.h \
			src/scintilla/src/Selection.h \
			src/scintilla/src/SplitVector.h \
			src/scintilla/src/Style.h \
			src/scintilla/src/SVector.h \
			src/scintilla/src/UniConversion.h \
			src/scintilla/src/ViewStyle.h \
			src/scintilla/src/XPM.h
														
sdkdir=$(includedir)/codeblocks/wxscintilla/include/wx

sdk_HEADERS = include/wx/wxscintilla.h

EXTRA_DIST = $(srcdir)/Readme \
	$(srcdir)/build/*.txt \
	$(srcdir)/build/Makefile \
	$(srcdir)/build/wxscintilla.* \
	$(srcdir)/src/scintilla/*.txt \
	$(srcdir)/src/scintilla/README \
	$(srcdir)/src/scintilla/include/*.py \
	$(srcdir)/src/scintilla/include/*.iface \
	$(srcdir)/src/scintilla/src/*.py \
	$(srcdir)/src/scintilla/src/*.properties

# add upper-case copy
CLEANFILES = README
README: $(srcdir)/Readme
	cp -p $< $@
