//#define CCCCCC const
//#define BB CCCCCC
//#define AAA BB
//#define EXPORT AAA
//#define DD(z) EXPORT z
//#define EEEE(y) DD(y)
//#define FFF(x) EEEE(x)
//
//extern FFF(wxChar*) wxEmptyString;


//#define wxUSE_UNICODE
//
//#define wxMAJOR_VERSION      2
//#define wxMINOR_VERSION      8
//#define wxRELEASE_NUMBER     11
//#define wxSUBRELEASE_NUMBER  0
//
//#ifndef wxABI_VERSION
//#define wxABI_VERSION ( wxMAJOR_VERSION * 10000 + wxMINOR_VERSION * 100 + 99 )
//#endif

//#if wxABI_VERSION >= 20804
//    // conversion to/from UTF-8:
//#if wxUSE_UNICODE
//    static wxString FromUTF8(const char *utf8)
//      { return wxString(utf8, wxConvUTF8); }
//    static wxString FromUTF8(const char *utf8, size_t len)
//      { return wxString(utf8, wxConvUTF8, len); }
//    const wxCharBuffer utf8_str() const { return mb_str(wxConvUTF8); }
//    const wxCharBuffer ToUTF8() const { return utf8_str(); }
//#elif wxUSE_WCHAR_T // ANSI
//    static wxString FromUTF8(const char *utf8)
//      { return wxString(wxConvUTF8.cMB2WC(utf8)); }
//    static wxString FromUTF8(const char *utf8, size_t len)
//    {
//      size_t wlen;
//      wxWCharBuffer buf(wxConvUTF8.cMB2WC(utf8, len == npos ? wxNO_LEN : len, &wlen));
//      return wxString(buf.data(), wxConvLibc, wlen);
//    }
//    const wxCharBuffer utf8_str() const
//      { return wxConvUTF8.cWC2MB(wc_str(wxConvLibc)); }
//    const wxCharBuffer ToUTF8() const { return utf8_str(); }
//#endif // Unicode/ANSI
//#endif // wxABI_VERSION >= 20804


//#define AAA(arg) unsigned arg
//AAA(int)  abc() {}


//#define EXPORT
//#define WXDLLIMPEXP_DATA_BASE(x) EXPORT x
//extern WXDLLIMPEXP_DATA_BASE(const wxChar*) wxEmptyString;


//#define wxDEPRECATED(x, y)  x y
//wxDEPRECATED(void , static wxChar* SetLogBuffer(wxChar* buf, size_t size = 0));


//#define AAA(Z) int AAAZ ()
//#define BBB(Y) AAA(Y)
//#define CCC(X) unsinged BBB(X)
//
//#define EEE(Z, K) int AAAZ (int K, float Z)
//#define FFF(Y) EEE(Y, china)
//
//AAA(a);
//BBB(b);
//CCC(c);
//EEE(d, e);
//FFF(f);


//#define AAA
////#define BBB
//
//void test(int testInt1 /*jfefiejfije*/
//          // test
//#ifdef AAA // ij
//#ifdef BBB
//, float testFloat1)
//#else
//
///*fijeif*/, int testInt2) // ijije
//#endif
//// test
//#else // hello
//// hello
//) // hello
//// hello
//#endif
//// hello
//{
//};


//#if 0
//    #define Expression_1
//#elif 0
//    #define Expression_2
//#elif 0
//    #define Expression_3
//#elif 0
//    #define Expression_4
//#if 0
//    #define Expression_4_if
//#else
//    #define Expression_4_else
//#endif
//#elif 0
//    #define Expression_5
//#else
//    #define Expression_1_else
//#if 0
//    #define Expression_1_else_if
//#elif 0
//    #define Expression_1_else_elif
//#else
//    #define Expression_1_else_else
//#if 0
//    #define Expression_1_else_else_if
//#elif 0
//    #define Expression_1_else_else_elif_1
//#elif 0
//    #define Expression_1_else_else_elif_2
//#else
//    #define Expression_1_else_else_else
//#if 1
//    #define Expression_1_else_else_else_if
//#endif
//#if 1
//    #define Expression_1_else_if2
//#else
//    #define Expression_1_else_else2
//#endif
//#endif
//#endif
//#endif


//#define DECLARE_LOG_FUNCTION(level) \
//void wxLog##level(const wxChar *szFormat, va_list argptr); \
//void wxTrace##level(const wxChar *szFormat, ...)
//DECLARE_LOG_FUNCTION(FatalError);
//DECLARE_LOG_FUNCTION(Error);
//DECLARE_LOG_FUNCTION(Warning);
//DECLARE_LOG_FUNCTION(Message);
//DECLARE_LOG_FUNCTION(Info);
//DECLARE_LOG_FUNCTION(Verbose);
