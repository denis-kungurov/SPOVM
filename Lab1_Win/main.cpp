#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <process.h>

#define N 9


int cmp(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

int* readFromFile(char* path, int* number)
{
	FILE* f_in = fopen(path, "rt");
	for (int i = 0; i <= 20; i++)
	{
		fscanf(f_in, "%d", &number[i]);
	}
	fclose(f_in);
	return number;
}

void writeToFile(char* path, int* number)
{
	FILE* f_out = fopen(path, "w");
	for (int i = 0; i < N; i++)
	{
		fprintf(f_out, "%d ", number[i]);
	}
	fclose(f_out);
}

void displayResult(int pid, int* massNum)
{
	int number = 0;
	for (int i = 0; i < N; i++)
	{
		number += massNum[i];
		number *= 10;
	}
	printf("Process %d result: %d\n", pid,number);
}

int enterNumber()
{
	int number;
	printf("Enter number of process: ");
	while (true)
	{
		fflush(stdin);
		scanf("%d", &number);
		if (number <= 0 || number >= 100) printf("Enter correct, not-null, less 100 number: ");
		else break;
	}
	return number;
}

void _tmain(int argc, TCHAR *argv[])
{
	char* path_in = "Text.txt";
	if (argv[1] != NULL)
	{
		int number[N];
		char path_out[20];
		sprintf(path_out, "Out %d.txt", getpid());
		readFromFile(path_in,number);
		qsort(number,N,sizeof(int),cmp);
		writeToFile(path_out,number);
		displayResult(getpid(),number);
	}
	else {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		HANDLE *pii;
		pii = (HANDLE*)calloc(10, sizeof(HANDLE));
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		int number = enterNumber();

		for (int i = 0; i<number; i++)
		{
			// Start the child process. 
			if (!CreateProcess(argv[0],   // No module name (use command line)
				L" 1",        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi)           // Pointer to PROCESS_INFORMATION structure
				)
			{
				printf("CreateProcess failed (%d).\n", GetLastError());
				return;
			}
			pii[i] = pi.hProcess;
		}
		// Wait until child process exits.
		WaitForMultipleObjects(10, pii, true, INFINITE);

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}
