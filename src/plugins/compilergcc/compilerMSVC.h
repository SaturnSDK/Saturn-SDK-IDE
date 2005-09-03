#ifdef __WXMSW__
// this compiler is valid only in windows

#ifndef COMPILERMSVC_H
#define COMPILERMSVC_H

#include <compiler.h>

class CompilerMSVC : public Compiler
{
	public:
		CompilerMSVC();
		virtual ~CompilerMSVC();
        virtual void Reset();
		virtual void LoadDefaultRegExArray();
        virtual AutoDetectResult AutoDetectInstallationDir();
	protected:
        Compiler * CreateCopy();
	private:
};

#endif // COMPILERMSVC_H

#endif // __WXMSW__
