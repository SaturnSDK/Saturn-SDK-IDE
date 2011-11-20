#ifndef WXSIMAGELISTDIALOG_H
#define WXSIMAGELISTDIALOG_H


//(*Headers(wxsImageListDialog)
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/colordlg.h>
#include <wx/panel.h>
#include <wx/filedlg.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include "wxsitem.h"
#include "wxstool.h"
#include <wx/bitmap.h>
#include <wx/imaglist.h>
#include "wxsbitmapiconeditordlg.h"

// when saving a wxArrayString in the *.wxs file it is stored as XML,
// and the TinyXml library compresses multiple spaces into a single space
// so, replace hard spaces with soft spaces so the TinyXml does not
// compress them

#define     kHardSpace      _(" ")
#define     kSoftSpace      _("\x95")

// and the XML lib uses the "%" for internal formatting

#define     kHardPercent    _("%")
#define     kSoftPercent    _("\x96")



// the main record data

class wxsImageListDialog: public wxDialog
{
	public:

		wxsImageListDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~wxsImageListDialog();

        bool            Execute(wxString &inName, wxArrayString &aImageData);


static  void            ImageToArray(wxImage &inImage, wxArrayString &outArray);
static  void            ArrayToImage(wxArrayString &inArray, wxImage &outImage);

static  void            BitmapToArray(wxBitmap &inBitmap, wxArrayString &outArray);
static  void            ArrayToBitmap(wxArrayString &inArray, wxBitmap &outBitmap);

static  void            ImageListToArray(wxImageList &inList, wxArrayString &outArray);
static  void            ArrayToImageList(wxArrayString &inArray, wxImageList &outList);

static  void            CopyImageList(wxImageList &inList, wxImageList &outList);

static  wxsItem        *FindTool(wxsItem *inItem, wxString inName);
static  int             CalcArraySize(wxArrayString &inArray);

static  bool            SaveXPM(wxImage * image, wxOutputStream& stream);





		//(*Declarations(wxsImageListDialog)
		wxStaticText* StaticText10;
		wxStaticText* StaticText22;
		wxStaticText* StaticText9;
		wxStaticText* StaticText20;
		wxButton* bOK;
		wxPanel* Panel5;
		wxBitmapButton* bRight;
		wxStaticText* StaticText13;
		wxButton* bCancel;
		wxStaticText* StaticText2;
		wxPanel* Panel4;
		wxStaticText* StaticText14;
		wxStaticText* StaticText26;
		wxStaticText* StaticText6;
		wxPanel* Panel11;
		wxBitmapButton* bSave;
		wxPanel* Panel9;
		wxStaticText* StaticText19;
		wxStaticText* StaticText8;
		wxStaticText* StaticText11;
		wxBitmapButton* bClear;
		wxStaticText* StaticText18;
		wxBitmapButton* bAdd;
		wxPanel* Panel8;
		wxPanel* Panel1;
		wxColourDialog* ColourDialog1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText27;
		wxFileDialog* FileDialog1;
		wxStaticText* StaticText3;
		wxStaticBox* StaticBox1;
		wxPanel* Panel6;
		wxStaticText* StaticText21;
		wxPanel* Panel3;
		wxButton* bColor;
		wxStaticBox* StaticBox2;
		wxStaticText* StaticText23;
		wxStaticText* StaticText24;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxPanel* Panel7;
		wxBitmapButton* bLeft;
		wxCheckBox* cxTransparent;
		wxStaticBox* StaticBox3;
		wxStaticText* StaticText28;
		wxStaticText* StaticText15;
		wxStaticText* StaticText12;
		wxPanel* Panel2;
		wxPanel* Panel10;
		wxStaticText* StaticText25;
		wxBitmapButton* bDel;
		wxStaticText* StaticText17;
		wxStaticText* StaticText4;
		wxPanel* Panel12;
		wxBitmapButton* bRead;
		wxStaticText* StaticText16;
		//*)

	protected:

		//(*Identifiers(wxsImageListDialog)
		static const long ID_STATICBOX3;
		static const long ID_STATICBOX2;
		static const long ID_STATICBOX1;
		static const long ID_PANEL1;
		static const long ID_STATICTEXT1;
		static const long ID_PANEL2;
		static const long ID_PANEL3;
		static const long ID_PANEL4;
		static const long ID_PANEL5;
		static const long ID_PANEL6;
		static const long ID_PANEL7;
		static const long ID_PANEL8;
		static const long ID_PANEL9;
		static const long ID_PANEL10;
		static const long ID_PANEL11;
		static const long ID_BITMAPBUTTON1;
		static const long ID_BITMAPBUTTON2;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXT12;
		static const long ID_BUTTON1;
		static const long ID_BUTTON4;
		static const long ID_BITMAPBUTTON3;
		static const long ID_BITMAPBUTTON4;
		static const long ID_STATICTEXT11;
		static const long ID_STATICTEXT13;
		static const long ID_STATICTEXT14;
		static const long ID_STATICTEXT15;
		static const long ID_STATICTEXT16;
		static const long ID_STATICTEXT17;
		static const long ID_STATICTEXT18;
		static const long ID_BITMAPBUTTON5;
		static const long ID_STATICTEXT20;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT21;
		static const long ID_PANEL12;
		static const long ID_STATICTEXT22;
		static const long ID_STATICTEXT23;
		static const long ID_STATICTEXT24;
		static const long ID_STATICTEXT25;
		static const long ID_STATICTEXT19;
		static const long ID_CHECKBOX1;
		static const long ID_STATICTEXT26;
		static const long ID_STATICTEXT27;
		static const long ID_STATICTEXT28;
		static const long ID_BITMAPBUTTON6;
		static const long ID_BITMAPBUTTON7;
		//*)

	private:

		//(*Handlers(wxsImageListDialog)
		void OnbAddClick(wxCommandEvent& event);
		void OnbReadClick(wxCommandEvent& event);
		void OnPanel1Paint(wxPaintEvent& event);
		void OnbColorClick(wxCommandEvent& event);
		void OnPanel1LeftUp(wxMouseEvent& event);
		void OncxTransparentClick(wxCommandEvent& event);
		void OnbOKClick(wxCommandEvent& event);
		void OnbCancelClick(wxCommandEvent& event);
		void OnbLeftClick(wxCommandEvent& event);
		void OnbRightClick(wxCommandEvent& event);
		void OnPanel2LeftUp(wxMouseEvent& event);
		void OnPanel3LeftUp(wxMouseEvent& event);
		void OnPanel4LeftUp(wxMouseEvent& event);
		void OnPanel5LeftUp(wxMouseEvent& event);
		void OnPanel8LeftUp(wxMouseEvent& event);
		void OnPanel6LeftUp(wxMouseEvent& event);
		void OnPanel7LeftUp(wxMouseEvent& event);
		void OnPanel9LeftUp(wxMouseEvent& event);
		void OnPanel10LeftUp(wxMouseEvent& event);
		void OnPanel11LeftUp(wxMouseEvent& event);
		void OnPanel12Paint(wxPaintEvent& event);
		void OnbDelClick(wxCommandEvent& event);
		void OnbClearClick(wxCommandEvent& event);
		void OnbSaveClick(wxCommandEvent& event);
		void PaintPreviewPanel(wxPaintEvent& event);
		//*)

        void                PreviewImport(void);
        void                PreviewList(void);
        void                PreviewSelected(void);
        void                UpdateEnabled(void);
        void                SelectPreviewPanel(int aIndex);
        void                PaintPanel(wxPaintDC &aDC, wxPanel *aPanel, wxBitmap &aBitmap, bool aHot = false);





		wxImageList             mImageList;                 // working copy of image list
		wxImage                 mImportImage;               // the thing that gets displayed
		wxColour                mImportMask;                // mask color

		wxsBitmapIconEditorDlg *mImageDialog;               // dialog to import external images
		wxsBitmapIconData       mImageData;                 // for the above dialog

		int                     mFirstImage;                // left-most image displayed in preview
		wxPanel                *mPreviewPanel[10];          // the list of previews
		wxStaticText           *mPreviewLabel[10];          // the list of previews
		int                     mPreviewSelect;             // one of these selected?

		DECLARE_EVENT_TABLE()
};

#endif
