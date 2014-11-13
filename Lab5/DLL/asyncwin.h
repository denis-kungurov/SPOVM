#ifndef asyncwin_h__
#define asyncwin_h__

#include <Windows.h>
#include <stdio.h>
#include <wchar.h>

typedef struct AsyncFileInfo
{
    OVERLAPPED info;
} AsyncFileInfo;

#ifdef ASYNCWIN_EXPORT
extern "C" void  __declspec(dllexport) AsyncRead(char* path, AsyncFileInfo* info, char* buff, int charsToRead);
extern "C" void  __declspec(dllexport) AsyncWrite(char* path, AsyncFileInfo* info, char* buff, int charsToWrite);
#else
extern "C" void  __declspec(dllimport) AsyncRead(char* path, AsyncFileInfo* info, char* buff, int charsToRead);
extern "C" void  __declspec(dllimport) AsyncWrite(char* path, AsyncFileInfo* info, char* buff, int charsToWrite);
#endif

#endif // asyncwin_h__
