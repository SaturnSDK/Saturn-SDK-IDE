/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */
#include "debugger_interface_creator.h"
#include "backtracedlg.h"
#include "breakpointsdlg.h"
#include "cpuregistersdlg.h"
#include "disassemblydlg.h"
#include "examinememorydlg.h"
#include "threadsdlg.h"
#include "watchesdlg.h"

void RemoveDockWindow(wxWindow *window)
{
    if (window)
    {
        CodeBlocksDockEvent evt(cbEVT_REMOVE_DOCK_WINDOW);
        evt.pWindow = window;
        Manager::Get()->ProcessEvent(evt);
        window->Destroy();
    }
}

cbBacktraceDlg* DebugInterfaceFactory::CreateBacktrace()
{
    BacktraceDlg *dialog = new BacktraceDlg(Manager::Get()->GetAppWindow());
    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
    evt.name = _T("CallStackPane");
    evt.title = _("Call stack");
    evt.pWindow = dialog;
    evt.dockSide = CodeBlocksDockEvent::dsFloating;
    evt.desiredSize.Set(150, 150);
    evt.floatingSize.Set(450, 150);
    evt.minimumSize.Set(150, 150);
    Manager::Get()->ProcessEvent(evt);
    return dialog;
}

void DebugInterfaceFactory::DeleteBacktrace(cbBacktraceDlg *dialog)
{
    RemoveDockWindow(dialog->GetWindow());
}

cbBreakpointsDlg* DebugInterfaceFactory::CreateBreapoints()
{
    BreakpointsDlg *dialog = new BreakpointsDlg;

    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
    evt.name = _T("BreakpointsPane");
    evt.title = _("Breakpoints");
    evt.pWindow = dialog;
    evt.dockSide = CodeBlocksDockEvent::dsFloating;
    evt.desiredSize.Set(350, 250);
    evt.floatingSize.Set(350, 250);
    evt.minimumSize.Set(150, 150);
    Manager::Get()->ProcessEvent(evt);
    return dialog;
}

void DebugInterfaceFactory::DeleteBreakpoints(cbBreakpointsDlg *dialog)
{
    RemoveDockWindow(dialog->GetWindow());
}

cbCPURegistersDlg* DebugInterfaceFactory::CreateCPURegisters()
{
    CPURegistersDlg *dialog = new CPURegistersDlg(Manager::Get()->GetAppWindow());

    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
    evt.name = _T("CPURegistersPane");
    evt.title = _("CPU Registers");
    evt.pWindow = dialog;
    evt.dockSide = CodeBlocksDockEvent::dsFloating;
    evt.desiredSize.Set(350, 250);
    evt.floatingSize.Set(350, 250);
    evt.minimumSize.Set(150, 150);
    Manager::Get()->ProcessEvent(evt);
    return dialog;
}

void DebugInterfaceFactory::DeleteCPURegisters(cbCPURegistersDlg *dialog)
{
    RemoveDockWindow(dialog->GetWindow());
}

cbDisassemblyDlg* DebugInterfaceFactory::CreateDisassembly()
{
    DisassemblyDlg *dialog = new DisassemblyDlg(Manager::Get()->GetAppWindow());
    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);

    evt.name = _T("DisassemblyPane");
    evt.title = _("Disassembly");
    evt.pWindow = dialog;
    evt.dockSide = CodeBlocksDockEvent::dsFloating;
    evt.desiredSize.Set(350, 250);
    evt.floatingSize.Set(350, 250);
    evt.minimumSize.Set(150, 150);
    Manager::Get()->ProcessEvent(evt);
    return dialog;
}

void DebugInterfaceFactory::DeleteDisassembly(cbDisassemblyDlg *dialog)
{
    RemoveDockWindow(dialog->GetWindow());
}

cbExamineMemoryDlg* DebugInterfaceFactory::CreateMemory()
{
    ExamineMemoryDlg *dialog = new ExamineMemoryDlg(Manager::Get()->GetAppWindow());
    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
    evt.name = _T("ExamineMemoryPane");
    evt.title = _("Memory");
    evt.pWindow = dialog;
    evt.dockSide = CodeBlocksDockEvent::dsFloating;
    evt.desiredSize.Set(450, 250);
    evt.floatingSize.Set(450, 250);
    evt.minimumSize.Set(350, 150);
    Manager::Get()->ProcessEvent(evt);
    return dialog;
}

void DebugInterfaceFactory::DeleteMemory(cbExamineMemoryDlg *dialog)
{
    RemoveDockWindow(dialog->GetWindow());
}

cbThreadsDlg* DebugInterfaceFactory::CreateThreads()
{
    ThreadsDlg *dialog = new ThreadsDlg(Manager::Get()->GetAppWindow());

    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
    evt.name = _T("ThreadsPane");
    evt.title = _("Running threads");
    evt.pWindow = dialog;
    evt.dockSide = CodeBlocksDockEvent::dsFloating;
    evt.desiredSize.Set(350, 75);
    evt.floatingSize.Set(450, 75);
    evt.minimumSize.Set(250, 75);
    Manager::Get()->ProcessEvent(evt);
    return dialog;
}

void DebugInterfaceFactory::DeleteThreads(cbThreadsDlg *dialog)
{
    RemoveDockWindow(dialog->GetWindow());
}

cbWatchesDlg* DebugInterfaceFactory::CreateWatches()
{
    WatchesDlg *dialog = new WatchesDlg;
    CodeBlocksDockEvent evt(cbEVT_ADD_DOCK_WINDOW);
    evt.name = _T("WatchesPane");
    evt.title = _("Watches (new)");
    evt.pWindow = dialog;
    evt.dockSide = CodeBlocksDockEvent::dsFloating;
    evt.desiredSize.Set(150, 250);
    evt.floatingSize.Set(150, 250);
    evt.minimumSize.Set(150, 150);
    Manager::Get()->ProcessEvent(evt);
    return dialog;
}

void DebugInterfaceFactory::DeleteWatches(cbWatchesDlg *dialog)
{
    RemoveDockWindow(dialog->GetWindow());
}
