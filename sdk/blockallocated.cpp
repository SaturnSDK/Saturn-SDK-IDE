/*
* Block allocator template classes for use with:
* Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* Author:  Thomas Denk
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include <sdk_precomp.h>

#ifndef CB_PRECOMP
    #include "blockallocated.h"
#endif

namespace BlkAllc
{

void DebugLog(wxString cn, int blockSize, int poolSize, int max_refs, int total_refs, int ref_count)
        {
            wxString s;
            wxString cn2;

            if(total_refs == 0)
                return; // pointless

            wxRegEx r(_T("^[A-Z]?[0-9]+(.*)"));
            if(r.Matches(cn))
                cn2 = r.GetMatch(cn, 1);

            s.Printf(_T("%s\n\n%d reserved pools of size %d (%d total objects)\n"
            "Maximum number of allocated objects: %d\n"
            "Total number of allocations: %d\n"
            "Number of stale objects: %d %s"),
            cn2.c_str(),
            blockSize, poolSize, blockSize * poolSize,
            max_refs, total_refs, ref_count, (ref_count == 0 ? _T("") : _T("(memory leak)")));

            wxSafeShowMessage(_T("Block Allocator"), s);
		}
};

