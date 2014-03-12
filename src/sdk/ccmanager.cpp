/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include "sdk_precomp.h"

#include "ccmanager.h"

#include "cbstyledtextctrl.h"
#include "editor_hooks.h"

namespace CCManagerHelper
{
    inline void RipplePts(int& ptA, int& ptB, int len, int delta)
    {
        if (ptA > len - delta)
            ptA += delta;
        if (ptB > len - delta)
            ptB += delta;
    }

    // wxScintilla::FindColumn seems to be broken; re-implement:
    // Find the position of a column on a line taking into account tabs and
    // multi-byte characters. If beyond end of line, return line end position.
    int FindColumn(int line, int column, wxScintilla* stc)
    {
        int lnEnd = stc->GetLineEndPosition(line);
        for (int pos = stc->PositionFromLine(line); pos < lnEnd; ++pos)
        {
            if (stc->GetColumn(pos) == column)
                return pos;
        }
        return lnEnd;
    }
}

template<> CCManager* Mgr<CCManager>::instance = 0;
template<> bool Mgr<CCManager>::isShutdown = false;

const int idCallTipTimer = wxNewId();
const int idAutoLaunchTimer = wxNewId();

// milliseconds
#define CALLTIP_REFRESH_DELAY 90

#define FROM_TIMER 1

// class constructor
CCManager::CCManager() :
    m_AutocompPosition(wxSCI_INVALID_POSITION),
    m_CallTipActive(wxSCI_INVALID_POSITION),
    m_CallTipTimer(this, idCallTipTimer),
    m_AutoLaunchTimer(this, idAutoLaunchTimer),
    m_pLastEditor(nullptr),
    m_pLastCCPlugin(nullptr)
{
/* temporary filler */
    const wxString ctChars = wxT(",;\n()");
    for (size_t i = 0; i < ctChars.Length(); ++i)
        m_CallTipChars.insert(ctChars[i]);
    const wxString alChars = wxT(".:<>\"#/");
    for (size_t i = 0; i < alChars.Length(); ++i)
        m_AutoLaunchChars.insert(alChars[i]);
/* end temporary */
    typedef cbEventFunctor<CCManager, CodeBlocksEvent> CCEvent;
    Manager::Get()->RegisterEventSink(cbEVT_APP_DEACTIVATED,    new CCEvent(this, &CCManager::OnDeactivateApp));
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_DEACTIVATED, new CCEvent(this, &CCManager::OnDeactivateEd));
    Manager::Get()->RegisterEventSink(cbEVT_EDITOR_TOOLTIP,     new CCEvent(this, &CCManager::OnEditorTooltip));
    Manager::Get()->RegisterEventSink(cbEVT_SHOW_CALL_TIP,      new CCEvent(this, &CCManager::OnShowCallTip));
    Manager::Get()->RegisterEventSink(cbEVT_COMPLETE_CODE,      new CCEvent(this, &CCManager::OnCompleteCode));
    m_EditorHookID = EditorHooks::RegisterHook(new EditorHooks::HookFunctor<CCManager>(this, &CCManager::OnEditorHook));
    Connect(idCallTipTimer,    wxEVT_TIMER, wxTimerEventHandler(CCManager::OnTimer));
    Connect(idAutoLaunchTimer, wxEVT_TIMER, wxTimerEventHandler(CCManager::OnTimer));
}

// class destructor
CCManager::~CCManager()
{
    Manager::Get()->RemoveAllEventSinksFor(this);
    EditorHooks::UnregisterHook(m_EditorHookID, true);
    Disconnect(idCallTipTimer);
}

cbCodeCompletionPlugin* CCManager::GetProviderFor(cbEditor* ed)
{
    if (!ed)
        ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed == m_pLastEditor)
        return m_pLastCCPlugin;
    m_pLastEditor = ed;
    m_pLastCCPlugin = nullptr;
    const PluginsArray& pa = Manager::Get()->GetPluginManager()->GetCodeCompletionOffers();
    for (size_t i = 0; i < pa.GetCount(); ++i)
    {
        if (static_cast<cbCodeCompletionPlugin*>(pa[i])->IsProviderFor(ed))
        {
            m_pLastCCPlugin = static_cast<cbCodeCompletionPlugin*>(pa[i]);
            break;
        }
    }
    return m_pLastCCPlugin;
}

// priority, then alphabetical
struct TokenSorter
{
    static bool isPureAlphabetical; // TODO: why does this fail if it is not static?

    TokenSorter()
    {
        // ctor
        isPureAlphabetical = true;
    }

    bool operator()(const cbCodeCompletionPlugin::CCToken& a, const cbCodeCompletionPlugin::CCToken& b)
    {
        int diff = a.weight - b.weight;
        if (diff == 0)
            diff = a.displayName.CmpNoCase(b.displayName);
        else if (isPureAlphabetical)
            isPureAlphabetical = false;
        return diff < 0;
    }
};
bool TokenSorter::isPureAlphabetical = true;

// cbEVT_COMPLETE_CODE
void CCManager::OnCompleteCode(CodeBlocksEvent& event)
{
    event.Skip();

    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return;
    cbCodeCompletionPlugin* ccPlugin = GetProviderFor(ed);
    if (!ccPlugin)
        return;

    cbStyledTextCtrl* stc = ed->GetControl();
    int tknEnd = stc->GetCurrentPos();
    int tknStart = stc->WordStartPosition(tknEnd, true);

    std::vector<cbCodeCompletionPlugin::CCToken> tokens = ccPlugin->GetAutocompList(event.GetInt() == FROM_TIMER,
                                                                                    ed, tknStart, tknEnd);
    if (tokens.empty())
        return;

    TokenSorter sortFunctor;
    std::sort(tokens.begin(), tokens.end(), sortFunctor);
    if (sortFunctor.isPureAlphabetical)
        stc->AutoCSetOrder(wxSCI_ORDER_PRESORTED);
    else
        stc->AutoCSetOrder(wxSCI_ORDER_CUSTOM);
    stc->AutoCompSetSeparator(wxT('|'));
    wxString items;
    // experimentally, the average length per token seems to be 23 for the main CC plugin
    items.Alloc(tokens.size() * 20); // TODO: measure performance
    for (size_t i = 0; i < tokens.size(); ++i)
        items += tokens[i].displayName + wxT("|");
    items.RemoveLast();

    stc->AutoCompSetIgnoreCase(true);
    stc->AutoCompSetMaxHeight(14);
    stc->AutoCompShow(tknEnd - tknStart, items);
}

// cbEVT_APP_DEACTIVATED
void CCManager::OnDeactivateApp(CodeBlocksEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (ed)
    {
        cbStyledTextCtrl* stc = ed->GetControl();
        if (stc->CallTipActive())
            stc->CallTipCancel();
        m_CallTipActive = wxSCI_INVALID_POSITION;
    }
    event.Skip();
}

// cbEVT_EDITOR_DEACTIVATED
void CCManager::OnDeactivateEd(CodeBlocksEvent& event)
{
    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinEditor(event.GetEditor());
    if (ed)
    {
        cbStyledTextCtrl* stc = ed->GetControl();
        if (stc->CallTipActive())
            stc->CallTipCancel();
        m_CallTipActive = wxSCI_INVALID_POSITION;
    }
    event.Skip();
}

// cbEVT_EDITOR_TOOLTIP
void CCManager::OnEditorTooltip(CodeBlocksEvent& event)
{
    event.Skip();
    m_CallTipActive = wxSCI_INVALID_POSITION;

    if (wxGetKeyState(WXK_CONTROL))
        return;

    EditorBase* base = event.GetEditor();
    cbEditor* ed = base && base->IsBuiltinEditor() ? static_cast<cbEditor*>(base) : nullptr;
    if (!ed || ed->IsContextMenuOpened())
        return;

    cbStyledTextCtrl* stc = ed->GetControl();
    if (stc->CallTipActive() && event.GetExtraLong() == 0)
        stc->CallTipCancel();

    cbCodeCompletionPlugin* ccPlugin = GetProviderFor(ed);
    if (!ccPlugin)
        return;

    int pos = stc->PositionFromPointClose(event.GetX(), event.GetY());
    if (pos < 0 || pos >= stc->GetLength())
        return;

    int hlStart, hlEnd, argsPos;
    hlStart = hlEnd = argsPos = wxSCI_INVALID_POSITION;
    const std::vector<cbCodeCompletionPlugin::CCToken>& tokens = ccPlugin->GetTokenAt(pos, ed);
    std::set<wxString> uniqueTips;
    for (size_t i = 0; i < tokens.size(); ++i)
        uniqueTips.insert(tokens[i].displayName);
    wxStringVec tips(uniqueTips.begin(), uniqueTips.end());

    const int style = event.GetInt();
    if (  tips.empty()
        && !(   stc->IsString(style)
             || stc->IsComment(style)
             || stc->IsCharacter(style)
             || stc->IsPreprocessor(style) ) )
    {
        tips = ccPlugin->GetCallTips(pos, style, ed, hlStart, hlEnd, argsPos);
    }
    if (!tips.empty())
    {
        DoShowTips(tips, stc, pos, argsPos, hlStart, hlEnd);
        event.SetExtraLong(1);
    }
}

void CCManager::OnEditorHook(cbEditor* ed, wxScintillaEvent& event)
{
    wxEventType evtType = event.GetEventType();
    if (evtType == wxEVT_SCI_CHARADDED)
    {
        const wxChar ch = event.GetKey();
        if (m_CallTipChars.find(ch) != m_CallTipChars.end())
        {
            CodeBlocksEvent evt(cbEVT_SHOW_CALL_TIP);
            Manager::Get()->ProcessEvent(evt);
        }
        else
        {
            cbStyledTextCtrl* stc = ed->GetControl();
            const int pos = stc->GetCurrentPos();
            const int wordStartPos = stc->WordStartPosition(pos, true);
            // TODO: read settings
            if (   (pos - wordStartPos >= 3 && !stc->AutoCompActive())
                || pos - wordStartPos == 3 + 4 )
            {
                CodeBlocksEvent evt(cbEVT_COMPLETE_CODE);
                Manager::Get()->ProcessEvent(evt);
            }
            else if (m_AutoLaunchChars.find(ch) != m_AutoLaunchChars.end())
            {
                m_AutoLaunchTimer.Start(10, wxTIMER_ONE_SHOT);
                m_AutocompPosition = pos;
            }
        }
    }
    else if (evtType == wxEVT_SCI_KEY)
    {
        cbStyledTextCtrl* stc = ed->GetControl();
        switch (event.GetKey())
        {
            case wxSCI_KEY_LEFT:
            case wxSCI_KEY_RIGHT:
                if (!stc->CallTipActive())
                    m_CallTipActive = wxSCI_INVALID_POSITION;
                // fall through
            case wxSCI_KEY_UP:
            case wxSCI_KEY_DOWN:
                if (m_CallTipActive != wxSCI_INVALID_POSITION)
                    m_CallTipTimer.Start(CALLTIP_REFRESH_DELAY, wxTIMER_ONE_SHOT);
                break;

            default:
                break;
        }
    }
    event.Skip();
}

// cbEVT_SHOW_CALL_TIP
void CCManager::OnShowCallTip(CodeBlocksEvent& event)
{
    event.Skip();

    cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if (!ed)
        return;
    cbCodeCompletionPlugin* ccPlugin = GetProviderFor(ed);
    if (!ccPlugin)
        return;

    cbStyledTextCtrl* stc = ed->GetControl();
    int pos = stc->GetCurrentPos();
    int hlStart, hlEnd, argsPos;
    hlStart = hlEnd = argsPos = wxSCI_INVALID_POSITION;
    const wxStringVec& tips = ccPlugin->GetCallTips(pos, stc->GetStyleAt(pos), ed, hlStart, hlEnd, argsPos);
    if (!tips.empty() && (event.GetInt() != FROM_TIMER || argsPos == m_CallTipActive))
    {
        int lnStart = stc->PositionFromLine(stc->LineFromPosition(pos));
        while (wxIsspace(stc->GetCharAt(lnStart)))
            ++lnStart; // do not show too far left on multi-line call tips
        DoShowTips(tips, stc, std::max(pos, lnStart), argsPos, hlStart, hlEnd);
        m_CallTipActive = argsPos;
    }
    else if (m_CallTipActive != wxSCI_INVALID_POSITION)
    {
        stc->CallTipCancel();
        m_CallTipActive = wxSCI_INVALID_POSITION;
    }
}

void CCManager::OnTimer(wxTimerEvent& event)
{
    if (event.GetId() == idCallTipTimer)
    {
        CodeBlocksEvent evt(cbEVT_SHOW_CALL_TIP);
        evt.SetInt(FROM_TIMER);
        Manager::Get()->ProcessEvent(evt);
    }
    else if (event.GetId() == idAutoLaunchTimer)
    {
        cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
        if (ed && ed->GetControl()->GetCurrentPos() == m_AutocompPosition)
        {
            CodeBlocksEvent evt(cbEVT_COMPLETE_CODE);
            evt.SetInt(FROM_TIMER);
            Manager::Get()->ProcessEvent(evt);
        }
        m_AutocompPosition = wxSCI_INVALID_POSITION;
    }
    else // ?!
        event.Skip();
}

void CCManager::DoShowTips(const wxStringVec& tips, cbStyledTextCtrl* stc, int pos, int argsPos, int hlStart, int hlEnd)
{
    int maxLines = std::max(stc->LinesOnScreen() / 4, 5);
    int marginWidth = stc->GetMarginWidth(wxSCI_MARGIN_SYMBOL) + stc->GetMarginWidth(wxSCI_MARGIN_NUMBER);
    int maxWidth = (stc->GetSize().x - marginWidth) / stc->TextWidth(wxSCI_STYLE_LINENUMBER, wxT("W")) - 1;
    maxWidth = std::min(std::max(60, maxWidth), 135);
    wxString tip;
    int lineCount = 0;

    for (size_t i = 0; i < tips.size() && lineCount < maxLines; ++i)
    {
        if (tips[i].Length() > (size_t)maxWidth + 6) // line is too long, try breaking it
        {
            wxString tipLn = tips[i];
            while (!tipLn.IsEmpty())
            {
                wxString segment = tipLn.Mid(0, maxWidth);
                int index = segment.Find(wxT(' '), true); // break on a space
                if (index < 20) // no reasonable break?
                {
                    segment = tipLn.Mid(0, maxWidth * 6 / 5); // increase search width a bit
                    index = segment.Find(wxT(' '), true);
                }
                for (int commaIdx = index - 1; commaIdx > maxWidth / 2; --commaIdx) // check back for a comma
                {
                    if (segment[commaIdx] == wxT(',') && segment[commaIdx + 1] == wxT(' '))
                    {
                        index = commaIdx + 1; // prefer splitting on a comma, if that does not set us back too far
                        break;
                    }
                }
                if (index < 20 || segment == tipLn) // end of string, or cannot split
                {
                    tip += tipLn + wxT("\n");
                    CCManagerHelper::RipplePts(hlStart, hlEnd, tip.Length(), 1);
                    tipLn.Clear();
                }
                else // continue splitting
                {
                    tip += segment.Mid(0, index) + wxT("\n ");
                    CCManagerHelper::RipplePts(hlStart, hlEnd, tip.Length(), 2);
                    // already starts with a space, so all subsequent lines are prefixed by two spaces
                    tipLn = tipLn.Mid(index);
                }
                ++lineCount;
            }
        }
        else // just add the line
        {
            tip += tips[i] + wxT("\n");
            CCManagerHelper::RipplePts(hlStart, hlEnd, tip.Length(), 1);
            ++lineCount;
        }
    }
    tip.RemoveLast(); // trailing linefeed

    // try to show the tip at the start of the token/arguments, or at the margin if we are scrolled right
    // an offset of 2 helps deal with the width of the folding bar (TODO: does an actual calculation exist?)
    int line = stc->LineFromPosition(pos);
    if (argsPos == wxSCI_INVALID_POSITION)
        argsPos = stc->WordStartPosition(pos, true);
    else
        argsPos = std::min(CCManagerHelper::FindColumn(line, stc->GetColumn(argsPos), stc), stc->WordStartPosition(pos, true));
    int offset = stc->PointFromPosition(stc->PositionFromLine(line)).x > marginWidth ? 0 : 2;
    stc->CallTipShow(std::max(argsPos, stc->PositionFromPoint(wxPoint(marginWidth, stc->PointFromPosition(pos).y)) + offset), tip);
    if (hlStart >= 0 && hlEnd > hlStart)
        stc->CallTipSetHighlight(hlStart, hlEnd);
}
