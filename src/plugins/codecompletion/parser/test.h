WINBASEAPI
__success(return != 0)
BOOL
WINAPI
GetExitCodeThread(
    __in  HANDLE hThread,
    __out LPDWORD lpExitCode
    );

WINBASEAPI
BOOL
WINAPI
GetThreadSelectorEntry(
    __in  HANDLE hThread,
    __in  DWORD dwSelector,
    __out LPLDT_ENTRY lpSelectorEntry
    );

WINBASEAPI
EXECUTION_STATE
WINAPI
SetThreadExecutionState(
    __in EXECUTION_STATE esFlags
    );