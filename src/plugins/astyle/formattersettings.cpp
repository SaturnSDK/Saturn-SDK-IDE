#include <sdk.h>
#include "formattersettings.h"
#include <old_configmanager.h>

FormatterSettings::FormatterSettings()
{
	//ctor
}

FormatterSettings::~FormatterSettings()
{
	//dtor
}

void FormatterSettings::ApplyTo(astyle::ASFormatter& formatter)
{
    int style = OldConfigManager::Get()->Read(_T("/astyle/style"), 0L);
    switch(style)
    {
        case 0: // ansi
            formatter.bracketIndent = false;
            formatter.indentLength = 4;
            formatter.indentString = "    ";
            formatter.bracketFormatMode = astyle::BREAK_MODE;
            formatter.classIndent = false;
            formatter.switchIndent = false;
            formatter.namespaceIndent = true;
            formatter.blockIndent = false;
            formatter.breakBlocks = false;
            formatter.breakElseIfs = false;
            formatter.padOperators = false;
            formatter.padParen = false;
            formatter.breakOneLineStatements = true;
            formatter.breakOneLineBlocks = true;
            break;

        case 1: // K&R
            formatter.bracketIndent = false;
            formatter.indentLength = 4;
            formatter.indentString = "    ";
            formatter.bracketFormatMode = astyle::ATTACH_MODE;
            formatter.classIndent = false;
            formatter.switchIndent = false;
            formatter.namespaceIndent = true;
            formatter.blockIndent = false;
            formatter.breakBlocks = false;
            formatter.breakElseIfs = false;
            formatter.padOperators = false;
            formatter.padParen = false;
            formatter.breakOneLineStatements = true;
            formatter.breakOneLineBlocks = true;
            break;

        case 2: // Linux
            formatter.bracketIndent = false;
            formatter.indentLength = 8;
            formatter.indentString = "        ";
            formatter.bracketFormatMode = astyle::BDAC_MODE;
            formatter.classIndent = false;
            formatter.switchIndent = false;
            formatter.namespaceIndent = true;
            formatter.blockIndent = false;
            formatter.breakBlocks = false;
            formatter.breakElseIfs = false;
            formatter.padOperators = false;
            formatter.padParen = false;
            formatter.breakOneLineStatements = true;
            formatter.breakOneLineBlocks = true;
            break;

        case 3: // GNU
            formatter.blockIndent = true;
            formatter.bracketIndent = false;
            formatter.indentLength = 2;
            formatter.indentString = "  ";
            formatter.bracketFormatMode = astyle::BREAK_MODE;
            formatter.classIndent = false;
            formatter.switchIndent = false;
            formatter.namespaceIndent = false;
            formatter.breakBlocks = false;
            formatter.breakElseIfs = false;
            formatter.padOperators = false;
            formatter.padParen = false;
            formatter.breakOneLineStatements = true;
            formatter.breakOneLineBlocks = true;
            break;

        case 4: // Java
            formatter.sourceStyle = astyle::STYLE_JAVA;
            formatter.modeSetManually = true;
            formatter.bracketIndent = false;
            formatter.indentLength = 4;
            formatter.indentString = "    ";
            formatter.bracketFormatMode = astyle::ATTACH_MODE;
            formatter.switchIndent = false;
            formatter.blockIndent = false;
            formatter.breakBlocks = false;
            formatter.breakElseIfs = false;
            formatter.padOperators = false;
            formatter.padParen = false;
            formatter.breakOneLineStatements = true;
            formatter.breakOneLineBlocks = true;
            break;

        default: // Custom
        {
            int spaceNum = OldConfigManager::Get()->Read(_T("/astyle/indentation"), 4);
            bool value;

            formatter.modeSetManually = false;
            formatter.indentLength = spaceNum;

            OldConfigManager::Get()->Read(_T("/astyle/use_tabs"), &value);
            if (value)
            {
                formatter.indentString = '\t';
            }
            else
            {
                formatter.indentString = string(spaceNum, ' ');
            }

            OldConfigManager::Get()->Read(_T("/astyle/force_tabs"), &value);
            if (value)
            {
                formatter.indentString = '\t';
                formatter.forceTabIndent = true;
            }
            else
            {
                formatter.forceTabIndent = false;
            }

            OldConfigManager::Get()->Read(_T("/astyle/convert_tabs"), &value);
            formatter.convertTabs2Space = value;

            OldConfigManager::Get()->Read(_T("/astyle/fill_empty_lines"), &value);
            formatter.emptyLineIndent = value;

            OldConfigManager::Get()->Read(_T("/astyle/indent_classes"), &value);
            formatter.classIndent = value;

            OldConfigManager::Get()->Read(_T("/astyle/indent_switches"), &value);
            formatter.switchIndent = value;

            OldConfigManager::Get()->Read(_T("/astyle/indent_case"), &value);
            formatter.caseIndent = value;

            OldConfigManager::Get()->Read(_T("/astyle/indent_brackets"), &value);
            formatter.bracketIndent = value;

            OldConfigManager::Get()->Read(_T("/astyle/indent_blocks"), &value);
            formatter.blockIndent = value;

            OldConfigManager::Get()->Read(_T("/astyle/indent_namespaces"), &value);
            formatter.namespaceIndent = value;

            OldConfigManager::Get()->Read(_T("/astyle/indent_labels"), &value);
            formatter.labelIndent = value;

            OldConfigManager::Get()->Read(_T("/astyle/indent_preprocessor"), &value);
            formatter.preprocessorIndent = value;

            wxString breakType = OldConfigManager::Get()->Read(_T("/astyle/break_type"));
            if (breakType == _T("Break"))
            {
            	formatter.bracketFormatMode = astyle::BREAK_MODE;
            }
            else if (breakType == _T("Attach"))
            {
            	formatter.bracketFormatMode = astyle::ATTACH_MODE;
            }
            else if (breakType == _T("Linux"))
            {
            	formatter.bracketFormatMode = astyle::BDAC_MODE;
            }
            else
            {
            	formatter.bracketFormatMode = astyle::NONE_MODE;
            }

            OldConfigManager::Get()->Read(_T("/astyle/break_blocks"), &value);
            formatter.breakBlocks = value;

            OldConfigManager::Get()->Read(_T("/astyle/break_elseifs"), &value);
            formatter.breakElseIfs = value;

            OldConfigManager::Get()->Read(_T("/astyle/pad_operators"), &value);
            formatter.padOperators = value;

            OldConfigManager::Get()->Read(_T("/astyle/pad_parentheses"), &value);
            formatter.padParen = value;

            OldConfigManager::Get()->Read(_T("/astyle/keep_complex"), &value);
            formatter.breakOneLineStatements = !value;

            OldConfigManager::Get()->Read(_T("/astyle/keep_blocks"), &value);
            formatter.breakOneLineBlocks = !value;
            break;
        }
    }
}
