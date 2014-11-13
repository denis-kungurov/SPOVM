#ifndef _WINFUN_H_
#define _WINFUN_H_

#include <Windows.h>
#include <conio.h>
#include <stdio.h>

typedef LPCRITICAL_SECTION Mutex;
typedef CRITICAL_SECTION MutexObject;
typedef struct ThreadArgs
{
    int end;
    Mutex mutex;
	Mutex mutex2;
    int i;
} th_args;

typedef struct ThreadHandle
{
    HANDLE threadHandle;
} Thread;

typedef struct AsyncFileInfo
{
    OVERLAPPED info;
} AsyncFileInfo;

typedef struct LibObject
{
    HMODULE lib;
} LibObject;

HANDLE init(char* source, char* arg);
HANDLE initSemaphore(char* str);
HANDLE initSharedMemory(char* str);
Thread getThread(th_args* arg, LPTHREAD_START_ROUTINE fun);
void JoinAllThreads(int count, Thread* threads);
void JoinThread(Thread thread);
void InitAsyncFileInfo(AsyncFileInfo* fileInfo, HANDLE* event);
void InitializeMutex(MutexObject* mutex);
void LockMutex(Mutex mutex);
void UnlockMutex(Mutex mutex);
void* getSharedMemory(HANDLE id);
void waitSemaphore(HANDLE id);
void releaseSemaphore(HANDLE id);
void* getFunctionAdress(LibObject lib, char* functionName);
LibObject OpenLibrary(char* path);
void CloseLibrary(LibObject lib);
wchar_t* convertToWstr(char* source);

#endif //_WINFUN_H_
