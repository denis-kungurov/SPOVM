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

#define HANDLE int

int init(char* source, char* arg);
int initSemaphore(char* str);
int initSharedMemory(char* str);
void* getSharedMemory(int id);
void waitSemaphore(int id);
void releaseSemaphore(int id);
int _getch();

#endif // LINFUN_H
