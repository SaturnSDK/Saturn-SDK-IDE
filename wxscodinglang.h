#ifndef WXSCODINGLANG_H
#define WXSCODINGLANG_H

/** \brief This enumeration contains all coding languages supported in wxSmith */
enum wxsCodingLang
{
    wxsCPP
};

/** \brief Namespace with functions generating marks for blocks of auto-generated code */
namespace wxsCodeMarks
{
    /** \brief Helper function for generating block begin */
    wxString Beg(wxsCodingLang Lang,const wxString& BlockName);

    /** \brief Helper function for generating block begin */
    wxString Beg(wxsCodingLang Lang,const wxString& BlockName,const wxString& Param);

    /** \brief Helper functino for generating block end */
    wxString End(wxsCodingLang Lang);
}

#endif
