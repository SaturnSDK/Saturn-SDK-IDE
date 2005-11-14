#include "sdk_precomp.h"
#include "compiler.h"
#include "manager.h"
#include "messagemanager.h"
#include "old_configmanager.h"
#include "globals.h"
#include "customvars.h"
#include <wx/intl.h>
#include <wx/regex.h>

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(RegExArray);

wxString Compiler::CommandTypeDescriptions[COMPILER_COMMAND_TYPES_COUNT] =
{
    // These are the strings that describe each CommandType enumerator...
    // No need to say that it must have the same order as the enumerators!
    _("Compile single file to object file"),
    _("Generate dependencies for file"),
    _("Compile Win32 resource file"),
    _("Link object files to executable"),
    _("Link object files to console executable"),
    _("Link object files to dynamic library"),
    _("Link object files to static library")
};
long Compiler::CompilerIDCounter = 0; // built-in compilers can have IDs from 1 to 255
long Compiler::UserCompilerIDCounter = 255; // user compilers have IDs over 255 (256+)

Compiler::Compiler(const wxString& name)
    : m_Name(name),
    m_ID(++CompilerIDCounter),
    m_ParentID(-1)
{
	//ctor
    Manager::Get()->GetMessageManager()->DebugLog(_("Added compiler \"%s\""), m_Name.c_str());
}

Compiler::Compiler(const Compiler& other)
    : CompileOptionsBase(other),
    m_ID(++UserCompilerIDCounter),
    m_ParentID(other.m_ID)
{
    m_Name = _("Copy of ") + other.m_Name;
    m_MasterPath = other.m_MasterPath;
    m_Programs = other.m_Programs;
    m_Switches = other.m_Switches;
    m_Options = other.m_Options;
    m_IncludeDirs = other.m_IncludeDirs;
    m_LibDirs = other.m_LibDirs;
    m_CompilerOptions = other.m_CompilerOptions;
    m_LinkerOptions = other.m_LinkerOptions;
    m_LinkLibs = other.m_LinkLibs;
    m_CmdsBefore = other.m_CmdsBefore;
    m_CmdsAfter = other.m_CmdsAfter;
    m_RegExes = other.m_RegExes;
    for (int i = 0; i < COMPILER_COMMAND_TYPES_COUNT; ++i)
    {
        m_Commands[i] = other.m_Commands[i];
    }
}

Compiler::~Compiler()
{
	//dtor
}

void Compiler::SaveSettings(const wxString& baseKey)
{
    wxString tmp;
    tmp.Printf(_T("%s/%3.3d"), baseKey.c_str(), (int)m_ID);
	OldConfigManager::Get()->Write(tmp + _T("/_name"), m_Name);
	OldConfigManager::Get()->Write(tmp + _T("/_parent"), m_ParentID);

	wxString key = GetStringFromArray(m_CompilerOptions);
	OldConfigManager::Get()->Write(tmp + _T("/compiler_options"), key);
	key = GetStringFromArray(m_LinkerOptions);
	OldConfigManager::Get()->Write(tmp + _T("/linker_options"), key);
	key = GetStringFromArray(m_IncludeDirs);
	OldConfigManager::Get()->Write(tmp + _T("/include_dirs"), key);
	key = GetStringFromArray(m_ResIncludeDirs);
	OldConfigManager::Get()->Write(tmp + _T("/res_include_dirs"), key);
	key = GetStringFromArray(m_LibDirs);
	OldConfigManager::Get()->Write(tmp + _T("/library_dirs"), key);
	key = GetStringFromArray(m_LinkLibs);
	OldConfigManager::Get()->Write(tmp + _T("/libraries"), key);
	key = GetStringFromArray(m_CmdsBefore);
	OldConfigManager::Get()->Write(tmp + _T("/commands_before"), key);
	key = GetStringFromArray(m_CmdsAfter);
	OldConfigManager::Get()->Write(tmp + _T("/commands_after"), key);

    OldConfigManager::Get()->Write(tmp + _T("/master_path"), m_MasterPath);
    OldConfigManager::Get()->Write(tmp + _T("/extra_paths"), GetStringFromArray(m_ExtraPaths, _T(";")));
    OldConfigManager::Get()->Write(tmp + _T("/c_compiler"), m_Programs.C);
    OldConfigManager::Get()->Write(tmp + _T("/cpp_compiler"), m_Programs.CPP);
    OldConfigManager::Get()->Write(tmp + _T("/linker"), m_Programs.LD);
    OldConfigManager::Get()->Write(tmp + _T("/lib_linker"), m_Programs.LIB);
    OldConfigManager::Get()->Write(tmp + _T("/res_compiler"), m_Programs.WINDRES);
    OldConfigManager::Get()->Write(tmp + _T("/make"), m_Programs.MAKE);
    OldConfigManager::Get()->Write(tmp + _T("/debugger"), m_Programs.DBG);

    for (int i = 0; i < COMPILER_COMMAND_TYPES_COUNT; ++i)
    {
        OldConfigManager::Get()->Write(tmp + _T("/macros/") + CommandTypeDescriptions[i], m_Commands[i]);
    }

    // switches
    OldConfigManager::Get()->Write(tmp + _T("/switches/includes"), m_Switches.includeDirs);
    OldConfigManager::Get()->Write(tmp + _T("/switches/libs"), m_Switches.libDirs);
    OldConfigManager::Get()->Write(tmp + _T("/switches/link"), m_Switches.linkLibs);
    OldConfigManager::Get()->Write(tmp + _T("/switches/define"), m_Switches.defines);
    OldConfigManager::Get()->Write(tmp + _T("/switches/generic"), m_Switches.genericSwitch);
    OldConfigManager::Get()->Write(tmp + _T("/switches/objectext"), m_Switches.objectExtension);
    OldConfigManager::Get()->Write(tmp + _T("/switches/deps"), m_Switches.needDependencies);
    OldConfigManager::Get()->Write(tmp + _T("/switches/forceCompilerQuotes"), m_Switches.forceCompilerUseQuotes);
    OldConfigManager::Get()->Write(tmp + _T("/switches/forceLinkerQuotes"), m_Switches.forceLinkerUseQuotes);
    OldConfigManager::Get()->Write(tmp + _T("/switches/logging"), m_Switches.logging);
    OldConfigManager::Get()->Write(tmp + _T("/switches/buildMethod"), m_Switches.buildMethod);
    OldConfigManager::Get()->Write(tmp + _T("/switches/libPrefix"), m_Switches.libPrefix);
    OldConfigManager::Get()->Write(tmp + _T("/switches/libExtension"), m_Switches.libExtension);
    OldConfigManager::Get()->Write(tmp + _T("/switches/linkerNeedsLibPrefix"), m_Switches.linkerNeedsLibPrefix);
    OldConfigManager::Get()->Write(tmp + _T("/switches/linkerNeedsLibExtension"), m_Switches.linkerNeedsLibExtension);

    // regexes
    OldConfigManager::Get()->DeleteGroup(tmp + _T("/regex"));
    wxString group;
    for (size_t i = 0; i < m_RegExes.Count(); ++i)
    {
        group.Printf(_T("%s/regex/%3.3d"), tmp.c_str(), i + 1);
        RegExStruct& rs = m_RegExes[i];
        OldConfigManager::Get()->Write(group + _T("/description"), rs.desc);
        OldConfigManager::Get()->Write(group + _T("/type"), rs.lt);
        OldConfigManager::Get()->Write(group + _T("/regex"), rs.regex);
        OldConfigManager::Get()->Write(group + _T("/msg1"), rs.msg[0]);
        OldConfigManager::Get()->Write(group + _T("/msg2"), rs.msg[1]);
        OldConfigManager::Get()->Write(group + _T("/msg3"), rs.msg[2]);
        OldConfigManager::Get()->Write(group + _T("/filename"), rs.filename);
        OldConfigManager::Get()->Write(group + _T("/line"), rs.line);
    }

    // custom vars
    m_pCustomVars->Save(tmp + _T("/custom_variables"));
}

void Compiler::LoadSettings(const wxString& baseKey)
{
    wxString tmp;
    tmp.Printf(_T("%s/%3.3d"), baseKey.c_str(), (int)m_ID);
    if (!OldConfigManager::Get()->HasEntry(tmp) && !OldConfigManager::Get()->HasGroup(tmp))
        return;

    wxString sep = wxFileName::GetPathSeparator();

    if (m_ID > 255) // name changes are allowed only for user compilers
        m_Name = OldConfigManager::Get()->Read(tmp + _T("/_name"), m_Name);

    m_MasterPath = OldConfigManager::Get()->Read(tmp + _T("/master_path"), m_MasterPath);
    m_ExtraPaths = GetArrayFromString(OldConfigManager::Get()->Read(tmp + _T("/extra_paths"), _T("")), _T(";"));
    m_Programs.C = OldConfigManager::Get()->Read(tmp + _T("/c_compiler"), m_Programs.C);
    m_Programs.CPP = OldConfigManager::Get()->Read(tmp + _T("/cpp_compiler"), m_Programs.CPP);
    m_Programs.LD = OldConfigManager::Get()->Read(tmp + _T("/linker"), m_Programs.LD);
    m_Programs.LIB = OldConfigManager::Get()->Read(tmp + _T("/lib_linker"), m_Programs.LIB);
    m_Programs.WINDRES = OldConfigManager::Get()->Read(tmp + _T("/res_compiler"), m_Programs.WINDRES);
    m_Programs.MAKE = OldConfigManager::Get()->Read(tmp + _T("/make"), m_Programs.MAKE);
    m_Programs.DBG = OldConfigManager::Get()->Read(tmp + _T("/debugger"), m_Programs.DBG);

    SetCompilerOptions(GetArrayFromString(OldConfigManager::Get()->Read(tmp + _T("/compiler_options"), wxEmptyString)));
    SetLinkerOptions(GetArrayFromString(OldConfigManager::Get()->Read(tmp + _T("/linker_options"), wxEmptyString)));
    SetIncludeDirs(GetArrayFromString(OldConfigManager::Get()->Read(tmp + _T("/include_dirs"), m_MasterPath + sep + _T("include"))));
    SetResourceIncludeDirs(GetArrayFromString(OldConfigManager::Get()->Read(tmp + _T("/res_include_dirs"), m_MasterPath + sep + _T("include"))));
    SetLibDirs(GetArrayFromString(OldConfigManager::Get()->Read(tmp + _T("/library_dirs"), m_MasterPath + sep + _T("lib"))));
    SetLinkLibs(GetArrayFromString(OldConfigManager::Get()->Read(tmp + _T("/libraries"), _T(""))));
    SetCommandsBeforeBuild(GetArrayFromString(OldConfigManager::Get()->Read(tmp + _T("/commands_before"), wxEmptyString)));
    SetCommandsAfterBuild(GetArrayFromString(OldConfigManager::Get()->Read(tmp + _T("/commands_after"), wxEmptyString)));

    for (int i = 0; i < COMPILER_COMMAND_TYPES_COUNT; ++i)
    {
        m_Commands[i] = OldConfigManager::Get()->Read(tmp + _T("/macros/") + CommandTypeDescriptions[i], m_Commands[i]);
    }

    // switches
    m_Switches.includeDirs = OldConfigManager::Get()->Read(tmp + _T("/switches/includes"), m_Switches.includeDirs);
    m_Switches.libDirs = OldConfigManager::Get()->Read(tmp + _T("/switches/libs"), m_Switches.libDirs);
    m_Switches.linkLibs = OldConfigManager::Get()->Read(tmp + _T("/switches/link"), m_Switches.linkLibs);
    m_Switches.defines = OldConfigManager::Get()->Read(tmp + _T("/switches/define"), m_Switches.defines);
    m_Switches.genericSwitch = OldConfigManager::Get()->Read(tmp + _T("/switches/generic"), m_Switches.genericSwitch);
    m_Switches.objectExtension = OldConfigManager::Get()->Read(tmp + _T("/switches/objectext"), m_Switches.objectExtension);
    m_Switches.needDependencies = OldConfigManager::Get()->Read(tmp + _T("/switches/deps"), m_Switches.needDependencies);
    m_Switches.forceCompilerUseQuotes = OldConfigManager::Get()->Read(tmp + _T("/switches/forceCompilerQuotes"), m_Switches.forceCompilerUseQuotes);
    m_Switches.forceLinkerUseQuotes = OldConfigManager::Get()->Read(tmp + _T("/switches/forceLinkerQuotes"), m_Switches.forceLinkerUseQuotes);
    m_Switches.logging = (CompilerLoggingType)OldConfigManager::Get()->Read(tmp + _T("/switches/logging"), m_Switches.logging);
    m_Switches.buildMethod = (CompilerBuildMethod)OldConfigManager::Get()->Read(tmp + _T("/switches/buildMethod"), m_Switches.buildMethod);
    m_Switches.libPrefix = OldConfigManager::Get()->Read(tmp + _T("/switches/libPrefix"), m_Switches.libPrefix);
    m_Switches.libExtension = OldConfigManager::Get()->Read(tmp + _T("/switches/libExtension"), m_Switches.libExtension);
    m_Switches.linkerNeedsLibPrefix = OldConfigManager::Get()->Read(tmp + _T("/switches/linkerNeedsLibPrefix"), m_Switches.linkerNeedsLibPrefix);
    m_Switches.linkerNeedsLibExtension = OldConfigManager::Get()->Read(tmp + _T("/switches/linkerNeedsLibExtension"), m_Switches.linkerNeedsLibExtension);

    // regexes
    wxString group;
    int index = 1;
    bool cleared = false;
    while (true)
    {
        group.Printf(_T("%s/regex/%3.3d"), tmp.c_str(), index++);
        if (!OldConfigManager::Get()->HasGroup(group))
            break;
        else if (!cleared)
        {
            cleared = true;
            m_RegExes.Clear();
        }
        RegExStruct rs;
        rs.desc = OldConfigManager::Get()->Read(group + _T("/description"));
        rs.lt = (CompilerLineType)OldConfigManager::Get()->Read(group + _T("/type"), 0L);
        rs.regex = OldConfigManager::Get()->Read(group + _T("/regex"));
        rs.msg[0] = OldConfigManager::Get()->Read(group + _T("/msg1"), 0L);
        rs.msg[1] = OldConfigManager::Get()->Read(group + _T("/msg2"), 0L);
        rs.msg[2] = OldConfigManager::Get()->Read(group + _T("/msg3"), 0L);
        rs.filename = OldConfigManager::Get()->Read(group + _T("/filename"), 0L);
        rs.line = OldConfigManager::Get()->Read(group + _T("/line"), 0L);
        m_RegExes.Add(rs);
    }

    // custom vars
    m_pCustomVars->Load(tmp + _T("/custom_variables"));
    m_pCustomVars->SetModified(false);
}

CompilerLineType Compiler::CheckForWarningsAndErrors(const wxString& line)
{
    m_ErrorFilename.Clear();
    m_ErrorLine.Clear();
    m_Error.Clear();

    for (size_t i = 0; i < m_RegExes.Count(); ++i)
    {
        RegExStruct& rs = m_RegExes[i];
        if (rs.regex.IsEmpty())
            continue;
        wxRegEx regex(rs.regex);
        if (regex.Matches(line))
        {
            if (rs.filename > 0)
                 m_ErrorFilename = UnixFilename(regex.GetMatch(line, rs.filename));
            if (rs.line > 0)
                m_ErrorLine = regex.GetMatch(line, rs.line);
            for (int x = 0; x < 3; ++x)
            {
                if (rs.msg[x] > 0)
                {
                    if (!m_Error.IsEmpty())
                        m_Error << _T(" ");
                    m_Error << regex.GetMatch(line, rs.msg[x]);
                }
            }
            return rs.lt;
        }
    }
    return cltNormal; // default return value
}
