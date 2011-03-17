#include "debuggergdb_test_common.h"

#include "parsewatchvalue.h"

SUITE(CDBWatchParser)
{

TEST(CDBSingleLineStructPointer)
{
    GDBWatch w(wxT("t"));
    CHECK(ParseCDBWatchValue(w, wxT("struct t * 0x123456")));
    CHECK_EQUAL(wxT("t=0x123456"), w);
}

TEST(CDBSingleLineClassPointer)
{
    GDBWatch w(wxT("t"));
    CHECK(ParseCDBWatchValue(w, wxT("class t * 0x123456")));
    CHECK_EQUAL(wxT("t=0x123456"), w);
}

} // SUITE(CDBWatchParser)

