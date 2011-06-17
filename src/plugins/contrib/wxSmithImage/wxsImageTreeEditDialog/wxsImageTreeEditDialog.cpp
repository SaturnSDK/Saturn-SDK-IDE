#include "wxsImageTreeEditDialog.h"

//(*InternalHeaders(wxsImageTreeEditDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/msgdlg.h>



//(*IdInit(wxsImageTreeEditDialog)
const long wxsImageTreeEditDialog::ID_BUTTON1 = wxNewId();
const long wxsImageTreeEditDialog::ID_BUTTON2 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT1 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICBOX1 = wxNewId();
const long wxsImageTreeEditDialog::ID_TREECTRL1 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICBOX2 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT2 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT3 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT4 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT5 = wxNewId();
const long wxsImageTreeEditDialog::ID_BUTTON3 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT6 = wxNewId();
const long wxsImageTreeEditDialog::ID_CHECKBOX1 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT7 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT8 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT9 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT10 = wxNewId();
const long wxsImageTreeEditDialog::ID_COMBOBOX1 = wxNewId();
const long wxsImageTreeEditDialog::ID_COMBOBOX2 = wxNewId();
const long wxsImageTreeEditDialog::ID_COMBOBOX3 = wxNewId();
const long wxsImageTreeEditDialog::ID_COMBOBOX4 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT11 = wxNewId();
const long wxsImageTreeEditDialog::ID_IMAGEBUTTON1 = wxNewId();
const long wxsImageTreeEditDialog::ID_IMAGEBUTTON2 = wxNewId();
const long wxsImageTreeEditDialog::ID_IMAGEBUTTON3 = wxNewId();
const long wxsImageTreeEditDialog::ID_IMAGEBUTTON4 = wxNewId();
const long wxsImageTreeEditDialog::ID_IMAGEBUTTON5 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT12 = wxNewId();
const long wxsImageTreeEditDialog::ID_STATICTEXT13 = wxNewId();
const long wxsImageTreeEditDialog::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxsImageTreeEditDialog,wxDialog)
	//(*EventTable(wxsImageTreeEditDialog)
	//*)
END_EVENT_TABLE()

wxsImageTreeEditDialog::wxsImageTreeEditDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(wxsImageTreeEditDialog)
	Create(parent, id, _("Edit Tree Control Items"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(400,480));
	Move(wxDefaultPosition);
	wxFont thisFont(8,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(0,0), wxSize(400,480), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetMaxSize(wxSize(0,0));
	bOK = new wxButton(Panel1, ID_BUTTON1, _("OK"), wxPoint(48,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	bOK->SetDefault();
	bCancel = new wxButton(Panel1, ID_BUTTON2, _("Cancel"), wxPoint(280,440), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Edit Tree Items"), wxPoint(0,0), wxSize(400,20), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	wxFont StaticText1Font(10,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticText1->SetFont(StaticText1Font);
	StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, _("tree-name"), wxPoint(0,20), wxSize(250,400), 0, _T("ID_STATICBOX1"));
	Tree1 = new wxTreeCtrl(Panel1, ID_TREECTRL1, wxPoint(2,36), wxSize(246,380), wxTR_EDIT_LABELS|wxTR_DEFAULT_STYLE, wxDefaultValidator, _T("ID_TREECTRL1"));
	Tree1ID[0] = Tree1->AddRoot(_("(root)"));
	Tree1ID[1] = Tree1->AppendItem(Tree1ID[0], _("First level, no tabs"));
	Tree1ID[2] = Tree1->AppendItem(Tree1ID[1], _("Second level, 1 tab"));
	Tree1ID[3] = Tree1->AppendItem(Tree1ID[2], _("Third level, 2 tabs"));
	Tree1ID[4] = Tree1->AppendItem(Tree1ID[0], _("First level again, no tabs"));
	Tree1ID[5] = Tree1->AppendItem(Tree1ID[4], _("Second level again, 1 tab"));
	Tree1ID[6] = Tree1->AppendItem(Tree1ID[4], _("Another second level, 1 tab"));
	Tree1ID[7] = Tree1->AppendItem(Tree1ID[6], _("Third level again, 2 tabs"));
	Tree1->ExpandAll();
	StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, _("Attributes"), wxPoint(252,20), wxSize(148,400), 0, _T("ID_STATICBOX2"));
	StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Add Item"), wxPoint(290,40), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("Add Sub-Item"), wxPoint(290,70), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Delete Current Item"), wxPoint(290,100), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, _("Delete All Items"), wxPoint(290,130), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	bItemColor = new wxButton(Panel1, ID_BUTTON3, _("C"), wxPoint(256,156), wxSize(24,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	wxFont bItemColorFont(10,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial Black"),wxFONTENCODING_DEFAULT);
	bItemColor->SetFont(bItemColorFont);
	bItemColor->SetToolTip(_("Set Item Text Color"));
	StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, _("Set Item Text Color"), wxPoint(290,160), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	cxItemBold = new wxCheckBox(Panel1, ID_CHECKBOX1, _(".   Set Item Text Bold"), wxPoint(262,186), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	cxItemBold->SetValue(false);
	cxItemBold->SetToolTip(_("Set Item Text Bold"));
	StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, _("Normal Image"), wxPoint(310,300), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	StaticText8 = new wxStaticText(Panel1, ID_STATICTEXT8, _("Selected Image"), wxPoint(310,330), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	StaticText9 = new wxStaticText(Panel1, ID_STATICTEXT9, _("Expanded Image"), wxPoint(310,360), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	StaticText10 = new wxStaticText(Panel1, ID_STATICTEXT10, _("Sel+Exp Image"), wxPoint(310,390), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	cbNormal = new wxBitmapComboBox(Panel1, ID_COMBOBOX1, wxEmptyString, wxPoint(256,296), wxSize(48,22), 0, NULL, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX1"));
	cbSelected = new wxBitmapComboBox(Panel1, ID_COMBOBOX2, wxEmptyString, wxPoint(256,326), wxSize(48,22), 0, NULL, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX2"));
	cbExpanded = new wxBitmapComboBox(Panel1, ID_COMBOBOX3, wxEmptyString, wxPoint(256,356), wxSize(48,22), 0, NULL, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX3"));
	cbSelExpanded = new wxBitmapComboBox(Panel1, ID_COMBOBOX4, wxEmptyString, wxPoint(256,386), wxSize(48,22), 0, NULL, wxCB_READONLY, wxDefaultValidator, _T("ID_COMBOBOX4"));
	StaticText11 = new wxStaticText(Panel1, ID_STATICTEXT11, _("Edit Current Item"), wxPoint(290,220), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	bAddItem = new wxBitmapButton(Panel1, ID_IMAGEBUTTON1, wxNullBitmap, wxPoint(256,36), wxSize(24,23), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_IMAGEBUTTON1"));
	bAddItem->SetToolTip(_("Add A New Item"));
	bAddSubItem = new wxBitmapButton(Panel1, ID_IMAGEBUTTON2, wxNullBitmap, wxPoint(256,66), wxSize(24,23), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_IMAGEBUTTON2"));
	bAddSubItem->SetToolTip(_("Add A New Child"));
	bDelItem = new wxBitmapButton(Panel1, ID_IMAGEBUTTON3, wxNullBitmap, wxPoint(256,96), wxSize(24,23), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_IMAGEBUTTON3"));
	bDelItem->SetToolTip(_("Delete Current Item"));
	bDelAllItems = new wxBitmapButton(Panel1, ID_IMAGEBUTTON4, wxNullBitmap, wxPoint(256,126), wxSize(24,23), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_IMAGEBUTTON4"));
	bDelAllItems->SetToolTip(_("Delete All Items"));
	bEditItem = new wxBitmapButton(Panel1, ID_IMAGEBUTTON5, wxNullBitmap, wxPoint(256,216), wxSize(24,23), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_IMAGEBUTTON5"));
	bEditItem->SetToolTip(_("Start Editor On Current Item"));
	StaticText12 = new wxStaticText(Panel1, ID_STATICTEXT12, _("Image-List"), wxPoint(256,272), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	StaticText13 = new wxStaticText(Panel1, ID_STATICTEXT13, _("Label"), wxPoint(310,272), wxSize(82,14), wxST_NO_AUTORESIZE, _T("ID_STATICTEXT13"));
	StaticText13->SetForegroundColour(wxColour(0,0,255));
	static const char *ImageList1_0_XPM[] = {
	"16 16 3 1",
	". c Black",
	"X c #00C000",
	"_ c None",
	"________......._",
	"________.XXXXX._",
	"___..___.XXXXX._",
	"___..___.XXXXX._",
	"_......_.XXXXX._",
	"_......_.XXXXX._",
	"___..___.XXXXX._",
	"___..___.XXXXX._",
	"________.XXXXX._",
	"________.XXXXX._",
	"________.XXXXX._",
	"________.XXXXX._",
	"________.XX.XX._",
	"________.X._.X._",
	"________..___.._",
	"................"
	};
	static const char *ImageList1_1_XPM[] = {
	"16 16 3 1",
	". c Black",
	"_ c None",
	"X c #FF4040",
	"________......._",
	"________.XXXXX._",
	"__.___._.XXXXX._",
	"__.._.._.XXXXX._",
	"___...__.XXXXX._",
	"____.___.XXXXX._",
	"___...__.XXXXX._",
	"__.._.._.XXXXX._",
	"__.___._.XXXXX._",
	"________.XXXXX._",
	"________.XXXXX._",
	"________.XXXXX._",
	"________.XX.XX._",
	"________.X._.X._",
	"________..___.._",
	"................"
	};
	static const char *ImageList1_2_XPM[] = {
	"16 16 4 1",
	"o c Black",
	". c #000080",
	"X c #0000FF",
	"_ c None",
	"________......._",
	"________.XXXXX._",
	"___oo___.XXXXX._",
	"___oo___.XXXXX._",
	"_oooooo_.XXXXX._",
	"_oooooo_.XXXXX._",
	"___oo___.XXXXX._",
	"___oo___.XXXXX._",
	"________.XXXXX._",
	"________.X.XXX._",
	"________.XXXXX._",
	"________.XXXXX._",
	"________.XX.XX._",
	"________.X._.X._",
	"________..___.._",
	"________________"
	};
	static const char *ImageList1_3_XPM[] = {
	"16 16 22 1",
	"4 c Black",
	"3 c #A5AEBD",
	"= c #5478B4",
	"1 c #95A3BB",
	"O c #9AA7BC",
	": c #758EB7",
	"$ c #6986B6",
	"# c #4971B2",
	"* c #8A9CBA",
	"X c #8598B9",
	"o c #ABB2BE",
	"; c #7F95B9",
	"- c #4E74B3",
	"2 c #A0ABBC",
	"+ c #6F8AB7",
	"_ c None",
	". c #B5B9BF",
	"@ c #3E69B1",
	"< c #90A0BA",
	"> c #6483B5",
	", c #5A7BB4",
	"& c #5F7FB5",
	"________________",
	"____.Xo______OO_",
	"____+@#.____$@&_",
	"____*@@X__.=@=o_",
	"_____-@-_.=@=.__",
	"_____;@@X=@=.___",
	"_____.#@@@$.____",
	"______:@@>______",
	"_____:@@@+______",
	"___.,@#&@@._____",
	"__o=@=oO@@<_____",
	"_1#@=._.@@-_____",
	"*@@$____>@@2____",
	":#*_____3#,.____",
	"________________",
	"4444444444444444"
	};
	static const char *ImageList1_4_XPM[] = {
	"16 16 4 1",
	"_ c Black",
	". c #FFFFFF",
	"X c #000084",
	"o c #848484",
	"________________",
	"_.............._",
	"_.XXX.........._",
	"_.XXX.__o___o_._",
	"_.XXX.........._",
	"_.............._",
	"_.XXX.........._",
	"_.XXX.___o____._",
	"_.XXX.........._",
	"_.............._",
	"_.XXX.........._",
	"_.XXX._o____o_._",
	"_.XXX.........._",
	"_.............._",
	"________________",
	"________________"
	};
	static const char *ImageList1_5_XPM[] = {
	"16 16 6 1",
	". c Black",
	"o c #FFFFFF",
	"O c #FFFF00",
	"+ c #00FFFF",
	"X c #FF0000",
	"_ c None",
	"___.__.......___",
	"__.X._.ooooo..__",
	"___.O..ooooo.o._",
	"____.O.oo..o.oo.",
	"_____.O.oooo....",
	"___....O.oooooo.",
	"...OoO..O.oo..o.",
	"+.Oo.......oooo.",
	"+.oOoOoOoOo.ooo.",
	"+.OoOo.....O.oo.",
	"+.oOoOoOo.o..oo.",
	"+.OoOo...oooo.o.",
	"+..OoOoO.oooooo.",
	".._.....oo.o..o.",
	"______.oooooooo.",
	"______.........."
	};
	static const char *ImageList1_6_XPM[] = {
	"16 16 3 1",
	"X c #FFFFFF",
	"_ c None",
	". c #7F7F7F",
	"___.__.......___",
	"__..._.XXXXX..__",
	"___.X..X____.X._",
	"____._.X_.._.X_.",
	"_____._.__XX....",
	"___...._.____XX.",
	"..._XX.._.__.._.",
	"..XX.......__XX.",
	"..X__XXXXXX.___.",
	"..X___....._.__.",
	"..X____XX.X..X_.",
	"..X___..._X_X._.",
	"...____X.X____X.",
	"..X....._X._.._.",
	"_XX_XX.XX__X_XX.",
	"______.........."
	};
	ImageList1 = new wxImageList(16, 16, 8);
	ImageList1->Add(wxBitmap(ImageList1_0_XPM));
	ImageList1->Add(wxBitmap(ImageList1_1_XPM));
	ImageList1->Add(wxBitmap(ImageList1_2_XPM));
	ImageList1->Add(wxBitmap(ImageList1_3_XPM));
	ImageList1->Add(wxBitmap(ImageList1_4_XPM));
	ImageList1->Add(wxBitmap(ImageList1_5_XPM));
	ImageList1->Add(wxBitmap(ImageList1_6_XPM));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OnbOKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OnbCancelClick);
	Connect(ID_TREECTRL1,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OnTreeCtrl1SelectionChanged);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OnbItemColorClick);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OncxItemBoldClick);

	Connect(ID_COMBOBOX1,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OncbNormalSelect);

	Connect(ID_COMBOBOX2,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OncbSelectedSelect);

	Connect(ID_COMBOBOX3,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OncbExpandedSelect);

	Connect(ID_COMBOBOX4,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OncbSelExpandedSelect);
	bAddItem->SetBitmapLabel(ImageList1->GetBitmap(0));

	Connect(ID_IMAGEBUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OnbAddItemClick);
	bAddSubItem->SetBitmapLabel(ImageList1->GetBitmap(2));

	Connect(ID_IMAGEBUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OnbAddSubItemClick);
	bDelItem->SetBitmapLabel(ImageList1->GetBitmap(1));

	Connect(ID_IMAGEBUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OnbDelItemClick);
	bDelAllItems->SetBitmapLabel(ImageList1->GetBitmap(3));

	Connect(ID_IMAGEBUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OnbDelAllItemsClick);
	bEditItem->SetBitmapLabel(ImageList1->GetBitmap(5));

	Connect(ID_IMAGEBUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxsImageTreeEditDialog::OnbEditItemClick);
	//*)
}

// ----------------------------------------------------------------------------

wxsImageTreeEditDialog::~wxsImageTreeEditDialog()
{
	//(*Destroy(wxsImageTreeEditDialog)
	//*)
}

// ----------------------------------------------------------------------------

bool wxsImageTreeEditDialog::Execute(wxArrayString &aItems) {
int             i,n;
int             j,k;
int             jv, j1,j2,j3,j4;
wxColor         jc;
bool            jb;
wxString        jt;
wxTreeItemId    jp[32];

long            ll;
wxString        ss, tt;
wxTreeItemId    root;
wxTreeItemId    item;
wxTreeItemIdValue   cookie;
wxBitmap        bmp;
wxsImageList    *ilist;


// get name of combo-box and image-list

    n = aItems.GetCount();
    mTreeName  = _("<unknown>");
    mImageName = _("<none>");
    if (n >= 1) mTreeName  = aItems.Item(0);
    if (n >= 2) mImageName = aItems.Item(1);

// show the names

    ss = _("Tree Control: ") + mTreeName;
    StaticBox1->SetLabel(ss);

    ss = mImageName;
    StaticText13->SetLabel(ss);

// clear old junk

    Tree1->DeleteAllItems();

// a valid image-list given?

    mImageList.RemoveAll();
    ilist = (wxsImageList *) wxsImageListDialog::FindTool(NULL, mImageName);
    if (ilist != NULL) ilist->GetImageList(mImageList);
    SetImageList(mImageList);

// add all the new items

    n = aItems.GetCount();
    for(i=2; i<n; i++) {
        ss = aItems.Item(i);
        ParseTreeItem(ss, jv, jc, jb, j1, j2, j3, j4, jt);

        if (jv == 0) item = Tree1->AddRoot(jt);
        else         item = Tree1->AppendItem(jp[jv-1], jt);
        jp[jv] = item;

        if (jc.IsOk()) Tree1->SetItemTextColour(item, jc);
        Tree1->SetItemBold(item, jb);
        Tree1->SetItemImage(item, j1, wxTreeItemIcon_Normal);
        Tree1->SetItemImage(item, j2, wxTreeItemIcon_Selected);
        Tree1->SetItemImage(item, j3, wxTreeItemIcon_Expanded);
        Tree1->SetItemImage(item, j4, wxTreeItemIcon_SelectedExpanded);
    };

    Tree1->ExpandAll();

// show the dialog and wait for a response

    n = ShowModal();

// save all new stuff?

    if (n == wxOK) {

// must save combo-box name and image-list name

        aItems.Clear();
        aItems.Add(mTreeName);
        aItems.Add(mImageName);

// save the root item and all it's children
// this effectively saves every item in the tree
// I wanted to use a simple loop here, but it works MUCH easier with a recursive function

        root = Tree1->GetRootItem();
        if (root.IsOk()) EncodeTreeItems(root, 0, aItems);
    };

// done

    return (n == wxOK);
}


// ----------------------------------------------------------------------------

void    wxsImageTreeEditDialog::SetImageList(wxImageList &inImageList) {
int         i,n;
wxString    ss, tt;
wxBitmap    bmp;

// save the image list in the tree control

    Tree1->SetImageList(&inImageList);

// valid list given?

    n = inImageList.GetImageCount();
    if (n <= 0) {
        cbNormal->Enable(false);
        cbSelected->Enable(false);
        cbExpanded->Enable(false);
        cbSelExpanded->Enable(false);
    }
    else {
        cbNormal->Enable(true);
        cbSelected->Enable(true);
        cbExpanded->Enable(true);
        cbSelExpanded->Enable(true);
    };

// set images in the drop-down lists

    cbNormal->Clear();
    cbSelected->Clear();
    cbExpanded->Clear();
    cbSelExpanded->Clear();

    ss = _("<none>");
    cbNormal->Append(ss);
    cbSelected->Append(ss);
    cbExpanded->Append(ss);
    cbSelExpanded->Append(ss);

    for(i=0; i<n; i++) {
        ss.Printf(_("%d"), i);
        bmp = inImageList.GetBitmap(i);

        cbNormal->Append(ss, bmp);
        cbSelected->Append(ss, bmp);
        cbExpanded->Append(ss, bmp);
        cbSelExpanded->Append(ss, bmp);
    };

// default selections

    cbNormal->SetSelection(0);
    cbSelected->SetSelection(0);
    cbExpanded->SetSelection(0);
    cbSelExpanded->SetSelection(0);
}


// ----------------------------------------------------------------------------
// add a new item as a sibling of the current item

void wxsImageTreeEditDialog::OnbAddItemClick(wxCommandEvent& event) {
int             i,n;
wxTreeItemId    current;

// how many items?

    n = Tree1->GetCount();

// and current selection

    current = Tree1->GetSelection();

// add a root item?

    if (n <= 0) {
        current.Unset();
        AddItem(current);
    }

// no current item?

    else if (! current.IsOk()) {
        current = Tree1->GetRootItem();
        AddItem(current);
    }

// else a sibling

    else {
        current = Tree1->GetItemParent(current);
        AddItem(current);
    };
}

// ----------------------------------------------------------------------------
// add a new item as a child of the current item

void wxsImageTreeEditDialog::OnbAddSubItemClick(wxCommandEvent& event) {
int             i,n;
wxTreeItemId    current;

// how many items?

    n = Tree1->GetCount();

// and current selection

    current = Tree1->GetSelection();

// add a root item?

    if (n <= 0) {
        current.Unset();
        AddItem(current);
    }

// no current item?

    else if (! current.IsOk()) {
        current = Tree1->GetRootItem();
        AddItem(current);
    }

// else a child

    else {
        AddItem(current);
    };

// make sure it is expanded

    Tree1->Expand(current);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::AddItem(wxTreeItemId &inParent) {
int             i,n;
wxString        ss, tt;
wxTreeItemId    parent, current;
wxColour        cc;
bool            b;

// how many items?

    n = Tree1->GetCount();

// add a root item?

    if (n <= 0) {
        ss = _("root");
        current = Tree1->AddRoot(ss);
    }

// bad parent?

    else if (! inParent.IsOk()) {
        ss.Printf(_("item %d"), n);
        parent = Tree1->GetRootItem();
        current = Tree1->AppendItem(parent, ss);
    }

// else a child of whatever

    else {
        ss.Printf(_("item %d"), n);
        current = Tree1->AppendItem(inParent, ss);
    };

// if it failed, skip the rest of this

    if (! current.IsOk()) return;

// set text colour

    cc = bItemColor->GetForegroundColour();
    Tree1->SetItemTextColour(current, cc);

// bold or plain

    b = cxItemBold->GetValue();
    Tree1->SetItemBold(current, b);

// the images

    n = cbNormal->GetSelection() - 1;
    if (n >= 0) Tree1->SetItemImage(current, n, wxTreeItemIcon_Normal);

    n = cbSelected->GetSelection() - 1;
    if (n >= 0) Tree1->SetItemImage(current, n, wxTreeItemIcon_Selected);

    n = cbExpanded->GetSelection() - 1;
    if (n >= 0) Tree1->SetItemImage(current, n, wxTreeItemIcon_Expanded);

    n = cbSelExpanded->GetSelection() - 1;
    if (n >= 0) Tree1->SetItemImage(current, n, wxTreeItemIcon_SelectedExpanded);

// redraw the whole thing

    Tree1->Refresh();
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OnbDelItemClick(wxCommandEvent& event) {
int             i,n;
wxTreeItemId    current;

// current selection

    current = Tree1->GetSelection();

// delete it

    if (current.IsOk()) Tree1->Delete(current);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OnbDelAllItemsClick(wxCommandEvent& event) {
int         i,n;
wxString    ss;

    n = wxMessageBox(_T("Delete ALL Items In Tree?"), _T("Clear"), wxYES_NO);
    if (n == wxYES) Tree1->DeleteAllItems();
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OnbItemColorClick(wxCommandEvent& event) {
int             i,n;
wxColourData    cd;
wxColour        cc;
wxTreeItemId    current;

// ask user for a new color

    n = mColourDialog.ShowModal();
    if (n != wxID_OK) return;

// get the color

    cd = mColourDialog.GetColourData();
    cc = cd.GetColour();

// set the button text

    bItemColor->SetForegroundColour(cc);

// and the current item

    current = Tree1->GetSelection();
    if (current.IsOk()) Tree1->SetItemTextColour(current, cc);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OncxItemBoldClick(wxCommandEvent& event) {
int             i,n;
bool            b;
wxTreeItemId    current;

// get checkbox value

    b = cxItemBold->GetValue();

// and set the current item

    current = Tree1->GetSelection();
    if (current.IsOk()) Tree1->SetItemBold(current, b);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OnbEditItemClick(wxCommandEvent& event) {
int             i,n;
wxTreeItemId    current;

// current selection

    current = Tree1->GetSelection();

// delete it

    if (current.IsOk()) Tree1->EditLabel(current);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OncbNormalSelect(wxCommandEvent& event) {
int             i,n;
wxTreeItemId    current;

    n  = cbNormal->GetSelection();
    n -= 1;

    current = Tree1->GetSelection();
    if (current.IsOk()) Tree1->SetItemImage(current, n, wxTreeItemIcon_Normal);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OncbSelectedSelect(wxCommandEvent& event) {
int             i,n;
wxTreeItemId    current;

    n  = cbSelected->GetSelection();
    n -= 1;

    current = Tree1->GetSelection();
    if (current.IsOk()) Tree1->SetItemImage(current, n, wxTreeItemIcon_Selected);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OncbExpandedSelect(wxCommandEvent& event) {
int             i,n;
wxTreeItemId    current;

    n  = cbExpanded->GetSelection();
    n -= 1;

    current = Tree1->GetSelection();
    if (current.IsOk()) Tree1->SetItemImage(current, n, wxTreeItemIcon_Expanded);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OncbSelExpandedSelect(wxCommandEvent& event) {
int             i,n;
wxTreeItemId    current;

    n  = cbSelExpanded->GetSelection();
    n -= 1;

    current = Tree1->GetSelection();
    if (current.IsOk()) Tree1->SetItemImage(current, n, wxTreeItemIcon_SelectedExpanded);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OnTreeCtrl1SelectionChanged(wxTreeEvent& event) {
int             i,n;
wxTreeItemId    current;
wxColour        cc;
bool            b;

// get current item

    current = Tree1->GetSelection();
    if (! current.IsOk()) return;

// current text colour

    cc = Tree1->GetItemTextColour(current);
    bItemColor->SetForegroundColour(cc);

// bold or plain

    b = Tree1->IsBold(current);
    cxItemBold->SetValue(b);

// image indices

    n = Tree1->GetItemImage(current, wxTreeItemIcon_Normal);
    n += 1;
    cbNormal->SetSelection(n);

    n = Tree1->GetItemImage(current, wxTreeItemIcon_Selected);
    n += 1;
    cbSelected->SetSelection(n);

    n = Tree1->GetItemImage(current, wxTreeItemIcon_Expanded);
    n += 1;
    cbExpanded->SetSelection(n);

    n = Tree1->GetItemImage(current, wxTreeItemIcon_SelectedExpanded);
    n += 1;
    cbSelExpanded->SetSelection(n);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::ParseTreeItem(wxString aSource, int &outLevel, wxColour &outColour, bool &outBold, int &outImage1, int &outImage2, int &outImage3, int &outImage4, wxString &outText) {
int         i,n;
long        ll;
wxString    ss,tt;

// working copy

    ss = aSource;

// the depth level

    outLevel = 1;
    i  = ss.Find(_(","));
    if (i != wxNOT_FOUND) {
        tt = ss.Left(i);
        ss.erase(0, i+1);
        if (tt.ToLong(&ll)) outLevel = ll;
    };

// the color

    outColour.Set(_("?"));
    i  = ss.Find(_(","));
    if (i != wxNOT_FOUND) {
        tt = ss.Left(i);
        ss.erase(0, i+1);
        outColour.Set(tt);
    };

// bold or normal text

    n = 0;
    i  = ss.Find(_(","));
    if (i != wxNOT_FOUND) {
        tt = ss.Left(i);
        ss.erase(0, i+1);
        if (tt.ToLong(&ll)) n = ll;
    };
    outBold = (n != 0);

// 4 image indices

    outImage1 = -1;
    i  = ss.Find(_(","));
    if (i != wxNOT_FOUND) {
        tt = ss.Left(i);
        ss.erase(0, i+1);
        if (tt.ToLong(&ll)) outImage1 = ll;
    };

    outImage2 = -1;
    i  = ss.Find(_(","));
    if (i != wxNOT_FOUND) {
        tt = ss.Left(i);
        ss.erase(0, i+1);
        if (tt.ToLong(&ll)) outImage2 = ll;
    };

    outImage3 = -1;
    i  = ss.Find(_(","));
    if (i != wxNOT_FOUND) {
        tt = ss.Left(i);
        ss.erase(0, i+1);
        if (tt.ToLong(&ll)) outImage3 = ll;
    };

    outImage4 = -1;
    i  = ss.Find(_(","));
    if (i != wxNOT_FOUND) {
        tt = ss.Left(i);
        ss.erase(0, i+1);
        if (tt.ToLong(&ll)) outImage4 = ll;
    };

// everything else is the text

    ss.Trim(true);
    ss.Trim(false);
    outText = ss;
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::EncodeTreeItems(wxTreeItemId inParent, int inLevel, wxArrayString &outList) {
int                 i,n;
wxColour            cc;
wxString            ss, tt;
wxTreeItemId        child;
wxTreeItemIdValue   cookie;

// nothing yet

    ss = _("");

// start with this item

    tt.Printf(_("%d,"), inLevel);
    ss += tt;

    cc  = Tree1->GetItemTextColour(inParent);
    tt  = cc.GetAsString(wxC2S_HTML_SYNTAX);
    tt += _(",");
    ss += tt;

    if (Tree1->IsBold(inParent)) tt = _("1,");
    else                         tt = _("0,");
    ss += tt;

    n = Tree1->GetItemImage(inParent, wxTreeItemIcon_Normal);
    tt.Printf(_("%d,"), n);
    ss += tt;

    n = Tree1->GetItemImage(inParent, wxTreeItemIcon_Selected);
    tt.Printf(_("%d,"), n);
    ss += tt;

    n = Tree1->GetItemImage(inParent, wxTreeItemIcon_Expanded);
    tt.Printf(_("%d,"), n);
    ss += tt;

    n = Tree1->GetItemImage(inParent, wxTreeItemIcon_SelectedExpanded);
    tt.Printf(_("%d,"), n);
    ss += tt;

    tt = Tree1->GetItemText(inParent);
    ss += tt;

// save it

    outList.Add(ss);

// and all the children

    child = Tree1->GetFirstChild(inParent, cookie);
    while (child.IsOk()) {
        EncodeTreeItems(child, inLevel+1, outList);
        child = Tree1->GetNextChild(inParent, cookie);
    };
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OnbOKClick(wxCommandEvent& event) {

    EndModal(wxOK);
}

// ----------------------------------------------------------------------------

void wxsImageTreeEditDialog::OnbCancelClick(wxCommandEvent& event) {

    EndModal(wxCANCEL);
}
