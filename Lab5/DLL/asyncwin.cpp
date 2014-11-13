#define ASYNCWIN_EXPORT
#include "asyncwin.h"

wchar_t* convertToWstr(char* source);

void AsyncRead(char* path, AsyncFileInfo* info, char* buff, int charsToRead)
{
    wchar_t* wpath;
    wpath = convertToWstr(path);
    HANDLE file = CreateFileW(wpath, GENERIC_READ, 0, NULL, 3, FILE_FLAG_OVERLAPPED, NULL);
    ReadFile(file, buff, charsToRead, NULL,&info->info);
    free(wpath);
}

void AsyncWrite(char* path, AsyncFileInfo* info, char* buff, int charsToWrite)
{
    wchar_t* wpath;
    wpath = convertToWstr(path);
    HANDLE file = CreateFileW(wpath, GENERIC_WRITE, 0, NULL, 3, FILE_FLAG_OVERLAPPED, NULL);
    WriteFile(file, buff, charsToWrite, NULL, &info->info);
    free(wpath);
} 

wchar_t* convertToWstr(char* source)
{
    wchar_t* out;

    out = (wchar_t*)calloc(strlen(source) + 1, sizeof(wchar_t));
    mbstowcs(out, source, strlen(source));
    return out;
}