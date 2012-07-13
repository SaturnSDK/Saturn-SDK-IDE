/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef COMPILERDMD_H
#define COMPILERDMD_H

#include <compiler.h>

class CompilerDMD : public Compiler
{
    public:
        CompilerDMD();
        virtual ~CompilerDMD();
        virtual void Reset();
        virtual AutoDetectResult AutoDetectInstallationDir();
    protected:
        Compiler * CreateCopy();
    private:
};

#endif // COMPILERDMD_H
