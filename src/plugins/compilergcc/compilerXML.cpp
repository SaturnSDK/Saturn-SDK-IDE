#include <sdk.h>
#include <wx/textfile.h>

#include "compilerXML.h"

enum SearchMode
{
    master,
    extra,
    include,
    resource,
    lib,
    none
};

CompilerXML::CompilerXML(const wxString& name, const wxString& ID, const wxString& file)
    : Compiler(name, ID), m_fileName(file)
{
    Reset();
}

CompilerXML::~CompilerXML()
{
}

Compiler* CompilerXML::CreateCopy()
{
    Compiler* c = new CompilerXML(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

AutoDetectResult CompilerXML::AutoDetectInstallationDir()
{
    wxXmlDocument compiler;
    compiler.Load(m_fileName);
    wxXmlNode* node = compiler.GetRoot()->GetChildren();
    int depth = 0;
    SearchMode sm = none;
    wxString path;
    wxGetEnv(wxT("PATH"), &path);
    wxString origPath = path;
    if (!m_MasterPath.IsEmpty())
    {
        path += wxPATH_SEP + m_MasterPath;
        wxSetEnv(wxT("PATH"), path);
        m_MasterPath.Clear();
    }
    while (node)
    {
        if (node->GetName() == wxT("if") && node->GetChildren())
        {
            if (EvalXMLCondition(node))
            {
                node = node->GetChildren();
                ++depth;
                continue;
            }
            else if (node->GetNext() && node->GetNext()->GetName() == wxT("else") &&
                     node->GetNext()->GetChildren())
            {
                node = node->GetNext()->GetChildren();
                ++depth;
                continue;
            }
        }
        else if (node->GetName() == wxT("Path") && node->GetChildren())
        {
            wxString value = node->GetAttribute(wxT("type"), wxEmptyString);
            if (value == wxT("master"))
                sm = master;
            else if (value == wxT("extra"))
                sm = extra;
            else if (value == wxT("include"))
                sm = include;
            else if (value == wxT("resource"))
                sm = resource;
            else if (value == wxT("lib"))
                sm = lib;
            if (sm != master || m_MasterPath.IsEmpty())
            {
                node = node->GetChildren();
                ++depth;
                continue;
            }
            else
                sm = none;
        }
        else if (node->GetName() == wxT("Search") && sm != none)
        {
            wxString value;
            if (node->GetAttribute(wxT("envVar"), &value))
            {
                wxString pathValues;
                wxGetEnv(value, &pathValues);
                if (!pathValues.IsEmpty())
                {
                    wxArrayString pathArray = GetArrayFromString(pathValues, wxPATH_SEP);
                    wxString targ = GetExecName(node->GetAttribute(wxT("for"), wxEmptyString));
                    for (size_t i = 0; i < pathArray.GetCount(); ++i)
                    {
                        if ((targ.IsEmpty() && wxDirExists(pathArray[i])) || wxFileExists(pathArray[i] + wxFILE_SEP_PATH + targ))
                        {
                            if(AddPath(pathArray[i], sm))
                                break;
                        }
                        else if (sm == master && ((targ.IsEmpty() && wxDirExists(value + wxFILE_SEP_PATH + wxT("bin"))) || wxFileExists(pathArray[i] + wxFILE_SEP_PATH + wxT("bin") + wxFILE_SEP_PATH + targ)))
                        {
                            if(AddPath(pathArray[i] + wxFILE_SEP_PATH + wxT("bin"), sm))
                                break;
                        }
                    }
                }
            }
            else if (node->GetAttribute(wxT("path"), &value))
            {
                wxString targ = GetExecName(node->GetAttribute(wxT("for"), wxEmptyString));
                if ((targ.IsEmpty() && wxDirExists(value)) || wxFileExists(value + wxFILE_SEP_PATH + targ))
                    AddPath(value, sm);
                else if (sm == master && ((targ.IsEmpty() && wxDirExists(value + wxFILE_SEP_PATH + wxT("bin"))) || wxFileExists(value + wxFILE_SEP_PATH + wxT("bin") + wxFILE_SEP_PATH + targ)))
                    AddPath(value + wxFILE_SEP_PATH + wxT("bin"), sm);
            }
            else if (node->GetAttribute(wxT("file"), &value))
            {
                wxString regexp = node->GetAttribute(wxT("regex"), wxEmptyString);
                int idx = wxAtoi(node->GetAttribute(wxT("index"), wxT("0")));
                wxRegEx re;
                if (wxFileExists(value) && re.Compile(regexp))
                {
                    wxTextFile file(value);
                    for (size_t i = 0; i < file.GetLineCount(); ++i)
                    {
                        if (re.Matches(file.GetLine(i)))
                        {
                            AddPath(re.GetMatch(file.GetLine(i), idx), sm);
                            if (sm == master && !m_MasterPath.IsEmpty())
                                break;
                        }
                    }
                }
            }
#ifdef __WXMSW__ // for wxRegKey
            else if (node->GetAttribute(wxT("registry"), &value))
            {
                // to be written
            }
#endif // __WXMSW__
        }
        else if (node->GetName() == wxT("Add"))
        {
            wxString value;
            if (node->GetAttribute(wxT("cFlag"), &value))
                AddCompilerOption(value);
            else if (node->GetAttribute(wxT("lFlag"), &value))
                AddLinkerOption(value);
            else if (node->GetAttribute(wxT("lib"), &value))
                AddLinkLib(value);
            else if (sm != none)
            {
                wxString path;
                wxXmlNode* child = node->GetChildren();
                while (child)
                {
                    if (child->GetType() == wxXML_TEXT_NODE || child->GetType() == wxXML_CDATA_SECTION_NODE)
                        path << child->GetContent();
                    else if (node->GetName() == wxT("master"))
                        path << m_MasterPath;
                    else if (node->GetName() == wxT("separator"))
                        path << wxFILE_SEP_PATH;
                    child = child->GetNext();
                }
                AddPath(path.Trim().Trim(false), sm);
            }
        }
        else if (node->GetName() == wxT("Fallback") && sm != none)
        {
            wxString value = node->GetAttribute(wxT("path"), wxEmptyString);
            switch (sm)
            {
            case master:
                if (m_MasterPath.IsEmpty())
                    m_MasterPath = value;
                break;
            case extra:
                if (m_ExtraPaths.IsEmpty())
                    AddPath(value, sm);
                break;
            case include:
                if (m_IncludeDirs.IsEmpty())
                    AddPath(value, sm);
                break;
            case resource:
                if (m_ResIncludeDirs.IsEmpty())
                    AddPath(value, sm);
                break;
            case lib:
                if (m_LibDirs.IsEmpty())
                    AddPath(value, sm);
                break;
            default:
                break;
            }
        }
        while ( (!node->GetNext() || (sm == master && !m_MasterPath.IsEmpty())) &&
                depth > 0 )
        {
            node = node->GetParent();
            if(node->GetName() == wxT("Path"))
            {
                sm = none;
            }
            --depth;
        }
        node = node->GetNext();
    }
    wxSetEnv(wxT("PATH"), origPath);

    return wxFileExists(m_MasterPath + wxFILE_SEP_PATH + wxT("bin") + wxFILE_SEP_PATH + m_Programs.C) ? adrDetected : adrGuessed;
}

bool CompilerXML::AddPath(const wxString& path, int sm)
{
    switch (sm)
    {
    case master:
        if (path.AfterLast(wxFILE_SEP_PATH) == wxT("bin"))
            m_MasterPath = path.BeforeLast(wxFILE_SEP_PATH);
        else
            m_MasterPath = path;
        return true;
    case extra:
        if (m_ExtraPaths.Index(path, !platform::windows) == wxNOT_FOUND)
            m_ExtraPaths.Add(path);
        break;
    case include:
        AddIncludeDir(path);
        break;
    case resource:
        AddResourceIncludeDir(path);
        break;
    case lib:
        AddLibDir(path);
        break;
    default:
        break;
    }
    return false;
}
