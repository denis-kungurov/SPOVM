#include "winfun.h"

HANDLE init(char* source, char* arg)
{
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    wchar_t* path, *args, *fargs;
	char* argv;

	wchar_t* argvv;
	

    path = convertToWstr(source);
    args = convertToWstr(arg);
	argv = (char*)calloc(strlen(arg) + 2, sizeof(char));
    fargs = (wchar_t*)calloc(wcslen(args) + 2, sizeof(wchar_t));
    wcscpy(fargs + 1, args);
	strcpy(argv + 1, arg);
	argv[0] = ' ';
    fargs[0] = ' ';
	argvv = convertToWstr(argv);
    if (!CreateProcess(path, argvv, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
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
    handle = CreateSemaphoreW(&attr, 1, 1, L"SampleSemaphore");
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
