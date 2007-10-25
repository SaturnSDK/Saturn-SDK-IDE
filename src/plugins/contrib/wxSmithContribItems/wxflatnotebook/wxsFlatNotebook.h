#ifndef WXSFLATNOTEBOOK_H
#define WXSFLATNOTEBOOK_H

#include <wxwidgets/wxscontainer.h>


class wxsFlatNotebook : public wxsContainer
{
    public:
        wxsFlatNotebook(wxsItemResData* Data);
    //    virtual ~wxsFlatNotebook();
    protected:
    private:
        virtual void OnEnumContainerProperties(long Flags);
        virtual bool OnCanAddChild(wxsItem* Item,bool ShowMessage);
        virtual wxsPropertyContainer* OnBuildExtra();
        virtual wxString OnXmlGetExtraObjectClass();
        virtual void OnAddChildQPP(wxsItem* Child,wxsAdvQPP* QPP);
        virtual wxObject* OnBuildPreview(wxWindow* Parent,long PreviewFlags);
        virtual void OnBuildCreatingCode();
        virtual bool OnMouseClick(wxWindow* Preview,int PosX,int PosY);
        virtual bool OnIsChildPreviewVisible(wxsItem* Child);
        virtual bool OnEnsureChildPreviewVisible(wxsItem* Child);
        virtual void OnPreparePopup(wxMenu* Menu);
        virtual bool OnPopup(long Id);

        void UpdateCurrentSelection();

        wxsItem* m_CurrentSelection;
};



#endif // WXSFLATNOTEBOOK_H
