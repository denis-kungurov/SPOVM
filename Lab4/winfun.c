#ifdef _MSC_VER
//#pragma comment(lib,"C:\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\winv6.3\\um\\x86\\uuid.lib")
#endif
#include "winfun.h"

HANDLE init(char* source, char* arg)
{
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    wchar_t* path, *args, *fargs;

    path = convertToWstr(source);
    args = convertToWstr(arg);
    fargs = (wchar_t*)calloc(wcslen(args) + 2, sizeof(wchar_t));
    wcscpy(fargs + 1, args);
    fargs[0] = ' ';
    if (!CreateProcess(path, fargs, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        printf("WTF!\n%d", GetLastError());
    }
    free(path);
    free(fargs);
    free(args);
    return pi.hProcess;
}

HANDLE initSemaphore(char* str)
{
    SECURITY_ATTRIBUTES attr;
    HANDLE handle;

    attr.bInheritHandle = TRUE;
    attr.nLength = sizeof(attr);
    attr.lpSecurityDescriptor = NULL;
    handle = CreateSemaphoreW(&attr, 0, 1, L"SampleSemaphore");
    return handle;
}

wchar_t* convertToWstr(char* source)
{
    wchar_t* out;

    out = (wchar_t*)calloc(strlen(source) + 1, sizeof(wchar_t));
    mbstowcs(out, source, strlen(source));
    return out;
}

HANDLE initSharedMemory(char *str)
{
    SECURITY_ATTRIBUTES attr;
    HANDLE handle;

    attr.bInheritHandle = TRUE;
    attr.nLength = sizeof(attr);
    attr.lpSecurityDescriptor = NULL;
    handle = CreateFileMapping(INVALID_HANDLE_VALUE, &attr, PAGE_READWRITE, 1, 0, L"SampleMemory");
    return handle;
}

void* getSharedMemory(HANDLE id)
{
    return MapViewOfFile(id, FILE_MAP_ALL_ACCESS, 0, 0, 100);
}

void waitSemaphore(HANDLE id)
{
    WaitForSingleObject(id, INFINITE);
}

void releaseSemaphore(HANDLE id)
{
    ReleaseSemaphore(id, 1, NULL);
}

HANDLE getThread(th_args* arg, LPTHREAD_START_ROUTINE fun)
{
    SECURITY_ATTRIBUTES attr;

    attr.bInheritHandle = TRUE;
    attr.nLength = sizeof(attr);
    attr.lpSecurityDescriptor = NULL;
    return CreateThread(&attr, 0, fun, arg, 0, NULL);
}

void InitializeMutex(MutexObject* mutex)
{
    InitializeCriticalSection(mutex);
}

void LockMutex(Mutex mutex)
{
    EnterCriticalSection(mutex);
}

void UnlockMutex(Mutex mutex)
{
    LeaveCriticalSection(mutex);
}