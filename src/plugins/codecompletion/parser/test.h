#define wxDEPRECATED(x, y)  x y
wxDEPRECATED(void , static wxChar* SetLogBuffer(wxChar* buf, size_t size = 0));


#define DECLARE_LOG_FUNCTION(level) \
    void AAA##level(const wxChar *szFormat, va_list argptr); \
    void BBB##level(const wxChar *szFormat, ...)
DECLARE_LOG_FUNCTION(FatalError);
DECLARE_LOG_FUNCTION(Error);
DECLARE_LOG_FUNCTION(Warning);
DECLARE_LOG_FUNCTION(Message);
DECLARE_LOG_FUNCTION(Info);
DECLARE_LOG_FUNCTION(Verbose);

#define AAA(arg) unsigned arg
AAA(int)  abc() {}

#define WXDLLIMPEXP_DATA_BASE(x) x
extern WXDLLIMPEXP_DATA_BASE(const wxChar*) wxEmptyString;

