#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
//#pragma comment(lib,"C:\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\winv6.3\\um\\x86\\uuid.lib")
#endif
#ifdef _WIN32
#include "winfun.h"
#elif __linux__
#define WINAPI
#define Sleep(x) sleep(x)
#include "linfun.h"
#endif

unsigned long WINAPI printStr(void* arg);
th_args* initArg(int i, Mutex mutex);
void cleanArgs(int count, th_args** args);

int main(int argc, char** argv)
{
    HANDLE threadPool[100];
    th_args* argPool[100];
    int poolPtr = 0, cnt;
    int end = 0;
    MutexObject mutex;
    InitializeMutex(&mutex);

    while (!end)
        switch (_getch()){
        case '+':
            if (poolPtr > 100)
                break;
            argPool[poolPtr] = initArg(poolPtr, &mutex);
            threadPool[poolPtr] = getThread(argPool[poolPtr], printStr);
            poolPtr++;
            break;
        case '-':
            if (poolPtr == 0)
                break;
            argPool[poolPtr - 1]->end = 1;
            poolPtr--;
            break;
        case 'q':
            end = 1;
            break;
    }
    cnt = poolPtr - 1;
    for (; poolPtr > 0; poolPtr--)
    {
        argPool[poolPtr - 1]->end = 1;
    }
    #ifdef _WIN32
    WaitForMultipleObjects(poolPtr - 1, threadPool, TRUE, INFINITE);
    #elif __linux__
    for (poolPtr = 0; poolPtr < cnt; poolPtr++)
        pthread_join(*threadPool[poolPtr], NULL);
    #endif
    cleanArgs(cnt, argPool);
    return 0;
}

th_args* initArg(int i, Mutex mutex)
{
    th_args* arg = (th_args*)malloc(sizeof(th_args));
    arg->end = 0;
    arg->i = i;
    arg->mutex = mutex;
    return arg;
}

void cleanArgs(int count, th_args** args)
{
    int i = 0;

    for (i = 0; i < count; i++)
        free(args[i]);
}

unsigned long WINAPI printStr(void* arg)
{
    th_args* data = (th_args*)arg;
    char str[] = { "Thread" };
    int j;
    while (!(data->end))
    {
        LockMutex(data->mutex);
        for (j = 0; j < 6; j++)
            printf("%c", str[j]);
        printf("%d\n", data->i);
        UnlockMutex(data->mutex);
		Sleep(1);
    }
    free(arg);
    ExitThread(0);
    return 0;
}
