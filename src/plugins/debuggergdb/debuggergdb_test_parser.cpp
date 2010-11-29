#include <UnitTest++.h>
#include "parsewatchvalue.h"


inline std::ostream& operator<<(std::ostream &stream, wxString const &s)
{
    return stream << s.utf8_str().data();
}

inline std::ostream& operator<<(std::ostream &stream, wxChar const *s)
{
    return stream << wxString(s).utf8_str().data();
}

wxString WatchToString(cbWatch const &watch)
{
    wxString s;
    watch.GetSymbol(s);
    s += wxT("=");

    wxString value;
    watch.GetValue(value);
    s += value;

    if (watch.GetChildCount() > 0)
    {
        s += wxT(" {");
        s += WatchToString(*watch.GetChild(0));

        for (int ii = 1; ii < watch.GetChildCount(); ++ii)
        {
            s += wxT(",");
            s += WatchToString(*watch.GetChild(ii));
        }

        s += wxT("}");
    }

    return s;
}

inline std::ostream& operator<<(std::ostream &stream, cbWatch const &w)
{
    return stream << WatchToString(w);
}

inline bool operator == (wxString const &s, cbWatch const &w)
{
    return s == WatchToString(w);
}

SUITE(WatchToString)
{

GDBWatch* MakeWatch(wxString const &symbol, wxString const &value)
{
    GDBWatch *w = new GDBWatch(symbol);
    w->SetValue(value);
    return w;
}

TEST(Simple)
{
    GDBWatch w(wxT("a"));
    w.SetValue(wxT("\"5\""));

    CHECK_EQUAL(wxT("a=\"5\""), w);
}

TEST(SimpleChildren)
{
    GDBWatch w(wxT("a"));
    w.AddChild(MakeWatch(wxT("b"), wxT("\"5\"")));
    w.AddChild(MakeWatch(wxT("c"), wxT("\"6\"")));

    CHECK_EQUAL(wxT("a= {b=\"5\",c=\"6\"}"), w);
}

TEST(SimpleChildrenValue)
{
    GDBWatch w(wxT("a"));
    w.SetValue(wxT("\"value\""));
    w.AddChild(MakeWatch(wxT("b"), wxT("\"5\"")));
    w.AddChild(MakeWatch(wxT("c"), wxT("\"6\"")));

    CHECK_EQUAL(wxT("a=\"value\" {b=\"5\",c=\"6\"}"), w);
}

TEST(ComplexChildren)
{
    GDBWatch w(wxT("a"));

    GDBWatch *c = new GDBWatch(wxT("b"));
    c->AddChild(MakeWatch(wxT("b1"), wxT("\"5\"")));
    c->AddChild(MakeWatch(wxT("b2"), wxT("\"6\"")));
    w.AddChild(c);

    c = new GDBWatch(wxT("c"));
    c->AddChild(MakeWatch(wxT("c1"), wxT("\"5\"")));
    c->AddChild(MakeWatch(wxT("c2"), wxT("\"6\"")));
    w.AddChild(c);

    CHECK_EQUAL(wxT("a= {b= {b1=\"5\",b2=\"6\"},c= {c1=\"5\",c2=\"6\"}}"), w);
}

}


SUITE(GDBWatchParser)
{

TEST(Simple)
{
    GDBWatch w(wxT("a"));
    CHECK(ParseGDBWatchValue(w, wxT("0x60d088 \"test_test_test2\"")));
    CHECK_EQUAL(wxT("a=0x60d088 \"test_test_test2\""), w);
}

TEST(SimpleMembers)
{
    GDBWatch w(wxT("cmp"));
    CHECK(ParseGDBWatchValue(w, wxT("{\n  a = 5, \n  b = 7, \n  c = 1, \n  d = 2 \n}")));
    CHECK_EQUAL(wxT("cmp= {a=5,b=7,c=1,d=2}"), w);
}

TEST(SingleInheritance)
{
    GDBWatch w(wxT("t"));
    CHECK(ParseGDBWatchValue(w, wxT("{\n  <BaseA> = {\n    a = 15,\n    b = 20\n  },\n")
                                wxT("  members of DerivedA:\n  c = 5,\n  d = 10\n}")));
    CHECK_EQUAL(wxT("t= {<BaseA>= {a=15,b=20},c=5,d=10}"), w);
}

TEST(MultipleInheritance)
{
    GDBWatch w(wxT("t"));
    CHECK(ParseGDBWatchValue(w, wxT("{\n  <BaseA> = {\n    a = 15,\n    b = 20\n  }, \n")
                                wxT("  <BaseB> = {\n    a = 25,\n    b = 30\n  }, \n")
                                wxT("  members of DerivedAB: \n  e = 5,\n  f = 10\n}")));
    CHECK_EQUAL(wxT("t= {<BaseA>= {a=15,b=20},<BaseB>= {a=25,b=30},e=5,f=10}"), w);
}

TEST(IgnoreWarnings)
{
    GDBWatch w(wxT("t"));
    CHECK(ParseGDBWatchValue(w, wxT("warning: can't find linker symbol for virtual table for `wxString' value\n")
                                wxT("warning:   found `g_strEmpty' instead\n")
                                wxT("(type) {\n")
                                wxT("warning: can't find linker symbol for virtual table for `wxString' value\n")
                                wxT("warning:   found `g_strEmpty' instead\n")
                                wxT("   a = 5,\n    b = 10\n }")));
    CHECK_EQUAL(wxT("t=(type) {a=5,b=10}"), w);
}

TEST(NoDataFields)
{
    GDBWatch w(wxT("t"));
    CHECK(ParseGDBWatchValue(w, wxT("{ \n   field = {<No data fields>} \n}")));
    CHECK_EQUAL(wxT("t= {field= {[0]=<No data fields>}}"), w);
}

TEST(Empty)
{
    GDBWatch w(wxT("t"));
    CHECK(ParseGDBWatchValue(w, wxT("{ \n   empty = { \n } \n}")));
    CHECK_EQUAL(wxT("t= {empty=}"), w);
}

TEST(SimpleArray)
{
    GDBWatch w(wxT("t"));
    CHECK(ParseGDBWatchValue(w, wxT("{1, 2, 3, 4, 5, 6, 7}")));
    CHECK_EQUAL(wxT("t= {[0]=1,[1]=2,[2]=3,[3]=4,[4]=5,[5]=6,[6]=7}"), w);
}

TEST(TupleArray)
{
    GDBWatch w(wxT("t"));
    CHECK(ParseGDBWatchValue(w, wxT("{{\n      a = 1,\n      b = 2\n    }, {\n      a = 3,\n      b = 5\n    }}")));
    CHECK_EQUAL(wxT("t= {[0]= {a=1,b=2},[1]= {a=3,b=5}}"), w);
}

} // SUITE(GDBWatchParser)
