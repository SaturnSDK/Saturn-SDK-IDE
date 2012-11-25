#include "se_globals.h"

wxString GetParentDir(const wxString &path)
{
    wxString parent=wxFileName(parent).GetPath(0);
    if(path==parent||parent.IsEmpty())
        return wxEmptyString;
    else
        return parent;
}

bool DirIsChildOf(const wxString &path, const wxString &/*child*/)
{
    wxString parent=path;
    while(!parent.IsEmpty())
    {
        if(wxFileName(path).SameAs(parent))
            return true;
        parent=GetParentDir(parent);
    }
    return false;
}

bool WildCardListMatch(wxString list, wxString name, bool strip)
{
    if(list==_T("")) //any empty list matches everything by default
        return true;
    wxString wildlist=list;
    wxString wild=list.BeforeFirst(';');
    if(strip)
        wild=wild.Strip(wxString::both);
    while(wildlist!=_T(""))
    {
        if(wild!=_T("") && ::wxMatchWild(wild,name))
            return true;
        wildlist=wildlist.AfterFirst(';');
        wild=wildlist.BeforeFirst(';');
        if(strip)
            wild=wild.Strip(wxString::both);
    }
    return false;
}

bool PromptSaveOpenFile(wxString message, wxFileName path)
{
    EditorManager* em = Manager::Get()->GetEditorManager();
    EditorBase *eb=em->IsOpen(path.GetFullPath());
    if(eb)
    {
        if(eb->GetModified())
            switch(cbMessageBox(message,_("Save File?"),wxYES_NO|wxCANCEL))
            {
                case wxYES:
                    if(!eb->Save())
                        cbMessageBox(_("Save failed - proceeding with unsaved file"));
                case wxNO:
                    eb->Close();
                    return true;
                case wxCANCEL:
                    return false;
                default:
                    break;
            }
    }
    return true;
}
