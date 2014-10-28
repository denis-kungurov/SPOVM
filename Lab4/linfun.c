#include "linfun.h"
#include <stdlib.h>
#include <stdio.h>

int init(char* source, char* arg)
{
    pid_t childPid = 0;

    childPid = fork();
    if(!childPid)
    {
        execl(source, source, arg, NULL);
    }
    return childPid;
}

int _getch()
{
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int initSemaphore(char* path)
{
    int id;

    id = ftok(path, 'x');
    id = semget(id, 1, IPC_CREAT);
    semctl(id, 0, SETVAL, 1);
    return id;
}

int initSharedMemory(char *str)
{
    int id;

    id = ftok(str, 'y');
    id = shmget(5463567, 100, IPC_CREAT | 0600);
    return id;
}

void* getSharedMemory(int id)
{
    return shmat(id, NULL, 0);
}

void waitSemaphore(int id)
{
    struct sembuf wait, increment;

    wait.sem_num = 0;
    wait.sem_op = 0;
    increment.sem_num = 0;
    increment.sem_op = 1;
    semop(id, &wait, 1);
    semop(id, &increment, 1);
}

void releaseSemaphore(int id)
{
    struct sembuf decrement;

    decrement.sem_num = 0;
    decrement.sem_op = -1;
    semop(id, &decrement, 1);
}

pthread_t* getThread(th_args *arg, void* (*fun)(void *))
{
    pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, fun, (void*)arg);
    return thread;
}

void InitializeMutex(MutexObject *mutex)
{
    pthread_mutex_init(mutex, NULL);
}

void LockMutex(Mutex mutex)
{
    pthread_mutex_lock(mutex);
}

void UnlockMutex(Mutex mutex)
{
    pthread_mutex_unlock(mutex);
}

void ExitThread(int val)
{
    pthread_exit(NULL);
}
