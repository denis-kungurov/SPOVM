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
    if (!CreateProcess(source, arg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
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
    handle = CreateFileMappingW(INVALID_HANDLE_VALUE, &attr, PAGE_READWRITE, 1, 0, L"SampleMemory");
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

Thread getThread(th_args* arg, LPTHREAD_START_ROUTINE fun)
{
    SECURITY_ATTRIBUTES attr;
    Thread result;

    attr.bInheritHandle = TRUE;
    attr.nLength = sizeof(attr);
    attr.lpSecurityDescriptor = NULL;
    result.threadHandle = CreateThread(&attr, 0, fun, arg, 0, NULL);
    return result;
}

void JoinAllThreads(int count, Thread* threads)
{
    int i;
    HANDLE* handles;

    handles = (HANDLE*)malloc(count * sizeof(HANDLE));
    for (i = 0; i < count; i++)
        handles[i] = threads[i].threadHandle;
    WaitForMultipleObjects(count, handles, TRUE, INFINITE);
    free(handles);
}

void JoinThread(Thread thread)
{
    WaitForSingleObject(thread.threadHandle, INFINITE);
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

void InitAsyncFileInfo(AsyncFileInfo* fileInfo, HANDLE* event)
{
    memset(&fileInfo->info, 0, sizeof(fileInfo->info));
    *event = CreateEventW(NULL, TRUE, TRUE, NULL);
    fileInfo->info.hEvent = *event;
}

LibObject OpenLibrary(char* path)
{
    LibObject result;
    wchar_t* wpath = convertToWstr(path);
    result.lib = LoadLibraryW(wpath);
    return result;
}

void CloseLibrary(LibObject lib)
{
	FreeLibrary(lib.lib);
}

void* getFunctionAdress(LibObject lib, char* functionName)
{
    return GetProcAddress(lib.lib, functionName);
}