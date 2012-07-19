/*
 * This file is licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef COMPILER_PGIFORTRAN_H
#define COMPILER_PGIFORTRAN_H

#include <compiler.h>

class CompilerPGIFortran : public Compiler
{
    public:
        CompilerPGIFortran();
        virtual ~CompilerPGIFortran();

        virtual AutoDetectResult AutoDetectInstallationDir();
    protected:
        virtual Compiler* CreateCopy();
    private:
};

#endif // COMPILER_PGIFORTRAN_H

