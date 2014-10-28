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
    int i;
} th_args;


HANDLE init(char* source, char* arg);
HANDLE initSemaphore(char* str);
HANDLE initSharedMemory(char* str);
HANDLE getThread(th_args* arg, LPTHREAD_START_ROUTINE fun);
void InitializeMutex(MutexObject* mutex);
void LockMutex(Mutex mutex);
void UnlockMutex(Mutex mutex);
void* getSharedMemory(HANDLE id);
void waitSemaphore(HANDLE id);
void releaseSemaphore(HANDLE id);
wchar_t* convertToWstr(char* source);

#endif //_WINFUN_H_
