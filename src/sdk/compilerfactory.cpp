/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "compilerfactory.h"
    #include "manager.h"
    #include "messagemanager.h"
    #include "configmanager.h"
#endif

#include "autodetectcompilers.h"

// statics
CompilersArray CompilerFactory::Compilers;
Compiler* CompilerFactory::s_DefaultCompiler = 0;

size_t CompilerFactory::GetCompilersCount()
{
    return Compilers.GetCount();
}

Compiler* CompilerFactory::GetCompiler(size_t index)
{
    if (index > Compilers.GetCount() - 1)
        return 0;
    return Compilers[index];
}

Compiler* CompilerFactory::GetCompiler(const wxString& id)
{
    const wxString lid = id.Lower();
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        if (Compilers[i]->GetID().IsSameAs(lid))
        {
            return Compilers[i];
        }
    }
    return 0;
}

int CompilerFactory::GetCompilerIndex(const wxString& id)
{
    const wxString lid = id.Lower();
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        if (Compilers[i]->GetID().IsSameAs(lid))
        {
            return i;
        }
    }
    return -1;
}

int CompilerFactory::GetCompilerIndex(Compiler* compiler)
{
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        if (Compilers[i] == compiler)
        {
            return i;
        }
    }
    return -1;
}

void CompilerFactory::RegisterCompiler(Compiler* compiler)
{
    CompilerFactory::Compilers.Add(compiler);
    // if it's the first one, set it as default
    if (!s_DefaultCompiler)
        s_DefaultCompiler = compiler;
}

void CompilerFactory::RegisterUserCompilers()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("compiler"));
    wxArrayString paths = cfg->EnumerateSubPaths(_T("/user_sets"));
    for (unsigned int i = 0; i < paths.GetCount(); ++i)
    {
        wxString base = _T("/user_sets/") + paths[i];
		wxString parent = cfg->Read(base + _T("/parent"), wxEmptyString);
        if (!parent.IsEmpty())
        {
            Compiler* compiler = GetCompiler(parent);
            wxString name = cfg->Read(base + _T("/name"), wxEmptyString);
            CreateCompilerCopy(compiler, name);
        }
	}
}

Compiler* CompilerFactory::CreateCompilerCopy(Compiler* compiler, const wxString& newName)
{
    if (!compiler)
        return 0;

    // abort if an existing compiler with the same name exists
    // this also avoids the possibility of throwing an exception
    // in the compiler->CreateCopy() call below...
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        if (Compilers[i]->GetName() == newName)
        {
            return 0;
        }
    }

    Compiler* newC = compiler->CreateCopy();
    if (!newName.IsEmpty())
    {
        Compiler::m_CompilerIDs.Remove(newC->GetID());
        newC->SetName(newName);
        newC->m_ID = newName;
        newC->MakeValidID();
    }
    RegisterCompiler(newC);
    newC->LoadSettings(_T("/user_sets"));
    Manager::Get()->GetMessageManager()->DebugLog(_T("Added compiler \"%s\""), newC->GetName().c_str());
    return newC; // return the index for the new compiler
}

void CompilerFactory::RemoveCompiler(Compiler* compiler)
{
    if (!compiler || compiler->m_ParentID.IsEmpty())
        return;
    Manager::Get()->GetConfigManager(_T("compiler"))->DeleteSubPath(_T("/user_sets/") + compiler->GetID());

    Compilers.Remove(compiler);
    Manager::Get()->GetMessageManager()->DebugLog(_T("Compiler \"%s\" removed"), compiler->GetName().c_str());
    delete compiler;

    SaveSettings();
}

void CompilerFactory::UnregisterCompilers()
{
    WX_CLEAR_ARRAY(CompilerFactory::Compilers);
    CompilerFactory::Compilers.Empty();
}

const wxString& CompilerFactory::GetDefaultCompilerID()
{
    if (s_DefaultCompiler)
        return s_DefaultCompiler->GetID();

    static wxString empty = wxEmptyString;
    return empty;
}

Compiler* CompilerFactory::GetDefaultCompiler()
{
    return s_DefaultCompiler;
}

void CompilerFactory::SetDefaultCompiler(size_t index)
{
    if (index < Compilers.GetCount())
        s_DefaultCompiler = Compilers[index];
}

void CompilerFactory::SetDefaultCompiler(const wxString& id)
{
    Compiler* compiler = GetCompiler(id.Lower());
    SetDefaultCompiler(compiler);
}

void CompilerFactory::SetDefaultCompiler(Compiler* compiler)
{
    if (compiler)
        s_DefaultCompiler = compiler;
}

void CompilerFactory::SaveSettings()
{
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        wxString baseKey = Compilers[i]->GetParentID().IsEmpty() ? _T("/sets") : _T("/user_sets");
        Compilers[i]->SaveSettings(baseKey);
    }
}

void CompilerFactory::LoadSettings()
{
    bool needAutoDetection = false;
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        wxString baseKey = Compilers[i]->GetParentID().IsEmpty() ? _T("/sets") : _T("/user_sets");
        Compilers[i]->LoadSettings(baseKey);
        if (Compilers[i]->GetMasterPath().IsEmpty())
            needAutoDetection = true;
    }

    // auto-detect missing compilers
    if (needAutoDetection)
    {
        AutoDetectCompilers adc(Manager::Get()->GetAppWindow());
        PlaceWindow(&adc);
        adc.ShowModal();
    }
}

Compiler* CompilerFactory::SelectCompilerUI(const wxString& message, const wxString& preselectedID)
{
    int selected = -1;
    const wxString lid = preselectedID.Lower();

    // first build a list of available compilers
    wxString* comps = new wxString[Compilers.GetCount()];
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        comps[i] = Compilers[i]->GetName();
        if (selected == -1)
        {
            if (lid.IsEmpty())
            {
                if (Compilers[i] == s_DefaultCompiler)
                    selected = i;
            }
            else
            {
                if (Compilers[i]->GetID().IsSameAs(lid))
                    selected = i;
            }
        }
    }
    // now display a choice dialog
    wxSingleChoiceDialog dlg(0,
                        _("Compiler selection"),
                        message,
                        CompilerFactory::Compilers.GetCount(),
                        comps);
    dlg.SetSelection(selected);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
        return Compilers[dlg.GetSelection()];
    return 0;
}
