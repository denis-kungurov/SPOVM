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

#define HANDLE int
typedef pthread_mutex_t* Mutex;
typedef pthread_mutex_t MutexObject;
typedef struct ThreadArgs
{
    int end;
    Mutex mutex;
    int i;
} th_args;

int init(char* source, char* arg);
int initSemaphore(char* str);
int initSharedMemory(char* str);
pthread_t* getThread(th_args* arg, void* (*fun)(void*));
void ExitThread(int val);
void InitializeMutex(MutexObject* mutex);
void LockMutex(Mutex mutex);
void UnlockMutex(Mutex mutex);
void* getSharedMemory(int id);
void waitSemaphore(int id);
void releaseSemaphore(int id);
int _getch();

#endif // LINFUN_H
