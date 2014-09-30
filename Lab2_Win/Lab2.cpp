#include <stdlib.h>
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <wchar.h>
#include <conio.h>


HANDLE init(char* source, int number)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	wchar_t* path, *wcNum;
	char strNum[5] = { 0 };
	sprintf(strNum+1, "%d", number);
	strNum[0] = ' ';

	wcNum = (wchar_t*)calloc(strlen(strNum) + sizeof(wchar_t), sizeof(wchar_t));
	mbstowcs(wcNum, strNum, strlen(strNum));
	path = (wchar_t*)calloc(strlen(source) + sizeof(wchar_t), sizeof(wchar_t));
	mbstowcs(path, source, strlen(source));

	if (!CreateProcess(path, wcNum, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		printf("WTF!\n%d", GetLastError());
	}
	return pi.hProcess;
}

HANDLE setSemaphore()
{
	SECURITY_ATTRIBUTES xx;
	HANDLE hSemaphore;
	xx.bInheritHandle = TRUE;
	xx.nLength = sizeof(xx);
	xx.lpSecurityDescriptor = NULL;
	hSemaphore = CreateSemaphore(&xx, 0, 1, L"MySem");
	return hSemaphore;
}

int main(int argc, char** argv)
{
	if (argv[1] != NULL )
	{
		while (1)
		{
			int number = atoi(argv[1]);
			HANDLE semId = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, L"MySem");
			WaitForSingleObject(semId, INFINITE);
			for (int i = 0; i < 5; i++)
			{
				printf("%d", number);
			}
			printf("\n");
			ReleaseSemaphore(semId, 1, NULL);
		}
		return 0;
	}
	else {
		HANDLE pids[100];
		int pidPtr = 0;
		int end = 0;
		HANDLE semId = setSemaphore();

		while (!end)
			switch (getch()){
			case '+':
				if (pidPtr > 100)
					break;
				pids[pidPtr] = init(argv[0],pidPtr);
				pidPtr++;
				break;
			case '-':
				if (!pidPtr)
					break;
				TerminateProcess(pids[pidPtr-1],0);
				pidPtr--;
				break;
			case 'w':
				ReleaseSemaphore(semId,1,NULL);
				break;
			case 'q':
				end = 1;
				break;
		}
		while (pidPtr)
		{
			TerminateProcess(pids[pidPtr - 1],0);
			pidPtr--;
		}
		CloseHandle(semId);
		return 0;
	}
}
