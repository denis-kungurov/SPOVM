#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include "winfun.h"
#define LIBNAME "SPO5DLL.dll"
#define LINFUN
#define SIGUSR1 NULL
#define SIGUSR2 NULL
#elif __linux__
#define WINAPI
#define LIBNAME "./spo5so.so"
#define Sleep(x) sleep(x)
#define LINFUN (void* (*)(void*))
#include "linfun.h"
#endif

char sharedBuff[1000] = { 0 };
int ReadRdy = 0;
int WriteRdy = 0;

unsigned long WINAPI printStr(void* arg);
void initArg(th_args* arg, int i, Mutex mutex, Mutex mutex2);
LibObject initLib();
typedef void(*AsyncProc)(char*, AsyncFileInfo*, char*, int);
void(*ReadAsync)(char* path, AsyncFileInfo* info, char* buff, int charsToRead);
void(*WriteAsync)(char* path, AsyncFileInfo* info, char* buff, int charsToWrite);
void clrBuff();
#ifdef __linux__
void setSigaction(struct sigaction* action, int event);
#endif

int main()
{
    char files[][8] = {{"in1.txt"}, {"in2.txt"}, {"in3.txt"}, {"in4.txt"}};
    HANDLE event = SIGUSR1;
    Thread thread;
    th_args arg;
    MutexObject mutex, mutex2;
    AsyncFileInfo info;
    LibObject lib;
    int i = 0;
    #ifdef __linux__
    struct sigaction r;
    setSigaction(&r, event);
    struct sigaction y;
    setSigaction(&y, SIGUSR2);
    #endif
    InitializeMutex(&mutex);
    InitializeMutex(&mutex2);
    initArg(&arg, 0, &mutex, &mutex2);
    InitAsyncFileInfo(&info, &event);
    lib = initLib();

    LockMutex(&mutex);
    thread = getThread(&arg, LINFUN printStr);
    for(i = 0; i < 4; i++)
    {
        if(i > 0)
        {
            LockMutex(&mutex);
			ReadRdy = 0;
            UnlockMutex(&mutex2);
        }
        clrBuff();
        ReadAsync(files[i], &info, sharedBuff, 500);
		#ifdef __linux__
        while(!ReadRdy);
        ReadRdy = 0;
		close(info.info.aio_fildes);
		#elif _WIN32
		WaitForSingleObject(event, INFINITE);
		#endif
        puts(files[i]);
        InitAsyncFileInfo(&info, &event);
        puts(sharedBuff);
        UnlockMutex(&mutex);
        LockMutex(&mutex2);
		ReadRdy = 1;
    }
    arg.end = 1;
    ReadRdy = 1;
    JoinThread(thread);
    CloseLibrary(lib);

    return 0;
}

void clrBuff()
{
    int i = 0;
    for(i = 0; i < 1000; i++)
        sharedBuff[i] = 0;
}

void handler(int i)
{
    if (i == 10)
        ReadRdy = 1;
    if (i == 12)
        WriteRdy = 1;
}

#ifdef __linux__
void setSigaction(struct sigaction* action, int event)
{
    action->sa_handler = handler;
    action->sa_flags = 0;
    sigemptyset(&action->sa_mask);
    sigaddset(&action->sa_mask, event);
    sigaction(event, action, NULL);
}
#endif

void initArg(th_args *arg, int i, Mutex mutex, Mutex mutex2)
{
    arg->end = 0;
    arg->i = i;
    arg->mutex = mutex;
    arg->mutex2 = mutex2;
}

LibObject initLib()
{
    LibObject library = OpenLibrary(LIBNAME);
    ReadAsync = (AsyncProc)getFunctionAdress(library, "AsyncRead");
    WriteAsync = (AsyncProc)getFunctionAdress(library, "AsyncWrite");
    return library;
}

unsigned long WINAPI printStr(void* arg)
{

    HANDLE event = SIGUSR2;
    th_args* data = (th_args*)arg;
    AsyncFileInfo info;
    InitAsyncFileInfo(&info, &event);
    while(!data->end)
    {
        LockMutex(data->mutex2);
        LockMutex(data->mutex);
        puts(sharedBuff);
        WriteAsync("out.txt", &info, sharedBuff, strlen(sharedBuff));
		#ifdef __linux__
		while (!ReadRdy);
		ReadRdy = 0;
		close(info.info.aio_fildes);
		#elif _WIN32
		WaitForSingleObject(event, INFINITE);
		#endif
        InitAsyncFileInfo(&info, &event);
        UnlockMutex(data->mutex);
        UnlockMutex(data->mutex2);
        while(!ReadRdy);
    }
    ExitThread(0);
    return 0;
}
