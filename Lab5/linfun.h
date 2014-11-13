#ifndef LINFUN_H
#define LINFUN_H

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <termios.h>
#include <sys/shm.h>
#include <errno.h>
#include <pthread.h>
#include <aio.h>
#include <dlfcn.h>

#define HANDLE int
typedef pthread_mutex_t* Mutex;
typedef pthread_mutex_t MutexObject;
typedef struct ThreadArgs
{
    int end;
    Mutex mutex;
    Mutex mutex2;
    int i;
} th_args;

typedef struct AsyncFileInfo
{
    struct aiocb info;
} AsyncFileInfo;

typedef struct Thread
{
    pthread_t* thread;
} Thread;

typedef struct LibObject
{
    void* lib;
} LibObject;

int init(char* source, char* arg);
int initSemaphore(char* str);
int initSharedMemory(char* str);
Thread getThread(th_args* arg, void* (*fun)(void*));
void WaitEvent(int event);
void JoinThread(Thread thread);
void ExitThread(int val);
void InitAsyncFileInfo(AsyncFileInfo* fileInfo, int* event);
void InitializeMutex(MutexObject* mutex);
void LockMutex(Mutex mutex);
void UnlockMutex(Mutex mutex);
void* getSharedMemory(int id);
void waitSemaphore(int id);
void releaseSemaphore(int id);
LibObject OpenLibrary(char* path);
void CloseLibrary(LibObject lib);
void* getFunctionAdress(LibObject lib, char* functionName);
int _getch();

#endif // LINFUN_H
