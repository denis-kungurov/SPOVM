#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include "winfun.h"
#elif __linux__
#include "linfun.h"
#endif

void printStr(char** args);

int main(int argc, char** argv)
{
	puts("blabla");
    char arg[100] = { 0 };
    char str[50] = "";
    HANDLE semaphore, memory;
    void* memstart;
    
    if (argc > 1)
    {
        printStr(argv);
        return 0;
    }

    semaphore = initSemaphore(argv[0]);
    memory = initSharedMemory(argv[0]);
    sprintf(arg, "%d %d", semaphore, memory);
    memstart = getSharedMemory(memory);
    waitSemaphore(semaphore);
    init(argv[0], arg);
    while (1)
    {
        fgets(str, 50, stdin);
        memcpy(memstart, str, 50);
        releaseSemaphore(semaphore);
        waitSemaphore(semaphore);
    }
    return 0;
}

void printStr(char** args)
{
    HANDLE handle, memory;
    char buff[100] = { 0 };
    void* memstart;

    #ifdef _WIN32
    handle = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, TRUE, L"SampleSemaphore");
    memory = OpenFileMappingW(FILE_MAP_ALL_ACCESS, TRUE, L"SampleMemory");
    #elif __linux__
    sscanf(args[1],"%d %d", &handle, &memory);
    #endif
    if (!(memstart = getSharedMemory(memory)))
        puts("WHYYYYYYYYYYYYYY");
    while (1)
    {
        waitSemaphore(handle);
        memcpy(buff, memstart, 50);
        puts(buff);
        releaseSemaphore(handle);
    }
    CloseHandle(handle);
}
