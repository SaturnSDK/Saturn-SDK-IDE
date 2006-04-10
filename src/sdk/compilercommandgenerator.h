#ifndef COMPILERCOMMANDGENERATOR_H
#define COMPILERCOMMANDGENERATOR_H

#include "settings.h"
#include <wx/string.h>
#include <wx/hashmap.h>
#include <compiletargetbase.h>

class cbProject;
class ProjectBuildTarget;
class ProjectFile;
class Compiler;

WX_DECLARE_HASH_MAP(ProjectBuildTarget*, wxString, wxPointerHash, wxPointerEqual, OptionsMap);
WX_DECLARE_STRING_HASH_MAP(wxString, BackticksMap);

/** Generate command-lines needed to produce a build.
  * This pre-generates everything when Init() is called.
  *
  * This is used by compilers to generate the needed
  * command lines for a build.
  */
class DLLIMPORT CompilerCommandGenerator
{
    public:
        CompilerCommandGenerator();
        virtual ~CompilerCommandGenerator();

        /** Initialize for use with the specified @c project. */
        virtual void Init(cbProject* project);

        /** Get the command line to compile/link the specific file. */
        virtual void GenerateCommandLine(wxString& macro,
                                        ProjectBuildTarget* target,
                                        ProjectFile* pf,
                                        const wxString& file,
                                        const wxString& object,
                                        const wxString& deps);
    protected:
        virtual void DoBuildScripts(CompileOptionsBase* base, const wxString& funcName);
        virtual wxString GetOrderedOptions(ProjectBuildTarget* target, OptionsRelationType rel, const wxString& project_options, const wxString& target_options);
        virtual wxString SetupOutputFilenames(Compiler* compiler, ProjectBuildTarget* target);
        virtual wxString SetupIncludeDirs(Compiler* compiler, ProjectBuildTarget* target);
        virtual wxString SetupLibrariesDirs(Compiler* compiler, ProjectBuildTarget* target);
        virtual wxString SetupResourceIncludeDirs(Compiler* compiler, ProjectBuildTarget* target);
        virtual wxString SetupCompilerOptions(Compiler* compiler, ProjectBuildTarget* target);
        virtual wxString SetupLinkerOptions(Compiler* compiler, ProjectBuildTarget* target);
        virtual wxString SetupLinkLibraries(Compiler* compiler, ProjectBuildTarget* target);
        virtual wxString SetupResourceCompilerOptions(Compiler* compiler, ProjectBuildTarget* target);
        virtual wxString FixupLinkLibraries(Compiler* compiler, const wxString& lib);

        OptionsMap m_Output; ///< output filenames, per-target
        OptionsMap m_StaticOutput; ///< static output filenames, per-target
        OptionsMap m_DefOutput; ///< def output filenames, per-target
        OptionsMap m_Inc; ///< compiler 'include' dirs, per-target
        OptionsMap m_Lib; ///< linker 'include' dirs, per-target
        OptionsMap m_RC; ///< resource compiler 'include' dirs, per-target
        OptionsMap m_CFlags; ///< compiler flags, per-target
        OptionsMap m_LDFlags; ///< linker flags, per-target
        OptionsMap m_LDAdd; ///< link libraries, per-target
        OptionsMap m_RCFlags; ///< resource compiler flags, per-target

        wxString m_PrjIncPath; ///< directive to add the project's top-level path in compiler search dirs (ready for the command line)
    private:
        void ExpandBackticks(wxString& str);
        BackticksMap m_Backticks;
};

#endif // COMPILERCOMMANDGENERATOR_H
