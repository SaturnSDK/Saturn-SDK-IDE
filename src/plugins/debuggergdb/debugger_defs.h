#ifndef DEBUGGER_DEFS_H
#define DEBUGGER_DEFS_H

#include <wx/string.h>
#include <wx/dynarray.h>

////////////////////////////////////////////////////////////////////////////////
// Breakpoints
////////////////////////////////////////////////////////////////////////////////
struct DebuggerBreakpoint
{
    DebuggerBreakpoint()
        : line(0),
        enabled(true),
        useIgnoreCount(false),
        ignoreCount(0),
        useCondition(false),
        address(0),
        bpNum(-1)
    {}
    wxString filename;
	int line;
	bool enabled;
	bool useIgnoreCount;
	int ignoreCount;
	bool useCondition;
	wxString condition;
	wxString func;
	unsigned long int address; ///< actual break address

	long int bpNum;
};
WX_DEFINE_ARRAY(DebuggerBreakpoint*, BreakpointsList);

////////////////////////////////////////////////////////////////////////////////
// Watch variables
////////////////////////////////////////////////////////////////////////////////
enum WatchFormat
{
    Undefined = 0,
    Decimal,
    Unsigned,
    Hex,
    Binary,
    Char,

    // do not remove this
    Last,
    Any
};

struct Watch
{
    Watch(const wxString& k, WatchFormat f = Undefined) : keyword(k), format(f) {}
    Watch(const Watch& rhs) : keyword(rhs.keyword), format(rhs.format) {}
    static wxString FormatCommand(WatchFormat format)
    {
        switch (format)
        {
            case Decimal:       return _T("/d");
            case Unsigned:      return _T("/u");
            case Hex:           return _T("/x");
            case Binary:        return _T("/t");
            case Char:          return _T("/c");
            default:            return wxEmptyString;
        }
    }
    wxString keyword;
    WatchFormat format;
};
WX_DECLARE_OBJARRAY(Watch, WatchesArray);

////////////////////////////////////////////////////////////////////////////////
// Stack frames
////////////////////////////////////////////////////////////////////////////////
struct StackFrame
{
    StackFrame() : valid(false), number(0), address(0) {}
    void Clear()
    {
        valid = false;
        number = 0;
        address = 0;
        function.Clear();
        file.Clear();
        line.Clear();
    }
    bool valid;
    long int number;
    unsigned long int address;
    wxString function;
    wxString file;
    wxString line;
};

#endif // DEBUGGER_DEFS_H
