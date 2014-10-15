#ifndef _WINFUN_H_
#define _WINFUN_H_

#include <Windows.h>
#include <conio.h>

HANDLE init(char* source, char* arg);
HANDLE initSemaphore(char* str);
HANDLE initSharedMemory(char* str);
void* getSharedMemory(HANDLE id);
void waitSemaphore(HANDLE id);
void releaseSemaphore(HANDLE id);
wchar_t* convertToWstr(char* source);

#endif //_WINFUN_H_
