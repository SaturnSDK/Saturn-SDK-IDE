#ifndef COMPILER_XML_H
#define COMPILER_XML_H

#include <compiler.h>

class CompilerXML : public Compiler
{
    public:
        CompilerXML(const wxString& name, const wxString& ID, const wxString& file);
        virtual ~CompilerXML();
        virtual void Reset();
        virtual void LoadDefaultRegExArray();
        virtual AutoDetectResult AutoDetectInstallationDir();

    protected:
        virtual Compiler* CreateCopy();

    private:
        enum SearchMode
        {
            master,
            extra,
            include,
            resource,
            lib,
            none
        };
        bool AddPath(const wxString& path, SearchMode sm);

        wxString m_fileName;
};

#endif // COMPILER_XML_H
