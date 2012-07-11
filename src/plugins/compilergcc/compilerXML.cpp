#if 0 // Disable - not yet completed

#include <sdk.h>

#include "compilerXML.h"

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

void CompilerXML::Reset()
{
    m_Options.ClearOptions();
    LoadDefaultOptions(GetID());
    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerXML::LoadDefaultRegExArray()
{
    /*wxXmlDocument compiler;
    compiler.Load(file);
    wxXmlNode* node = compiler.GetRoot()->GetChildren;*/
}

AutoDetectResult CompilerXML::AutoDetectInstallationDir()
{
    wxXmlDocument compiler;
    compiler.Load(m_fileName);
    wxXmlNode* node = compiler.GetRoot()->GetChildren();
    int depth = 0;
    SearchMode sm = none;
    while (node)
    {
        if (node->GetName() == wxT("if") && node->GetChildren())
        {
            if (EvalXMLCondition(node))
            {
                node = node->GetChildren();
                depth++;
                continue;
            }
            else if (node->GetNext() && node->GetNext()->GetName() == wxT("else") &&
                     node->GetNext()->GetChildren())
            {
                node = node->GetNext()->GetChildren();
                depth++;
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
                depth++;
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
                    for (size_t i = 0; i < pathArray.GetCount(); ++i)
                    {
                        if (wxFileExists(pathArray[i] + wxFILE_SEP_PATH + node->GetAttribute(wxT("for"), wxEmptyString) ))//m_Programs.C))
                        {
                            if(AddPath(pathArray[i], sm))
                                break;
                        }
                    }
                }
            }
#ifdef __WXMSW__ // for wxRegKey
            else if (node->GetAttribute(wxT("registry"), &value))
            {
                ;
            }
#endif // __WXMSW__
        }
        else if (node->GetName() == wxT("Add") && sm != none)
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
        while ( (!node->GetNext() || (sm == master && !m_MasterPath.IsEmpty())) &&
                depth > 0 )
        {
            node = node->GetParent();
            if(node->GetName() == wxT("Path"))
            {
                sm = none;
            }
            depth--;
        }
        node = node->GetNext();
    }

    return wxFileExists(m_MasterPath + wxFILE_SEP_PATH + wxT("bin") + wxFILE_SEP_PATH + m_Programs.C) ? adrDetected : adrGuessed;

    /*wxString pathValues;
    wxGetEnv(_T("PATH"), &pathValues);
    if (!pathValues.IsEmpty())
    {
        wxArrayString pathArray = GetArrayFromString(pathValues, wxPATH_SEP);
        for (size_t i = 0; i < pathArray.GetCount(); ++i)
        {
            if (wxFileExists(pathArray[i] + wxFILE_SEP_PATH + m_Programs.C))
            {
                if (pathArray[i].AfterLast(wxFILE_SEP_PATH).IsSameAs(_T("bin")))
                {
                    m_MasterPath = pathArray[i].BeforeLast(wxFILE_SEP_PATH);
                    return adrDetected;
                }
            }
        }
    }*/
}

bool CompilerXML::AddPath(const wxString& path, SearchMode sm)
{
    switch (sm)
    {
    case master:
        if (path.EndsWith(wxFILE_SEP_PATH + wxT("bin")))
        {
            m_MasterPath = path.BeforeLast(wxFILE_SEP_PATH);
            return true;
        }
        break;
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

#endif // Disable
