#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio_ext.h> //for fpurge
#include <ncurses.h>

#define PATH_IN "Text.txt"

int cmp(const void *a, const void *b)
{
	return *(int*)a - *(int*)b;
}

int getArrayLength(int* array)
{
	int len = 0;
	while(array[len++]);
	return len - 1;
}

int* readFromFile(int* number)
{
	FILE* f_in = fopen(PATH_IN, "rt");
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
	for (int i = 0; i < getArrayLength(number); i++)
	{
		fprintf(f_out, "%d ", number[i]);
	}
	fclose(f_out);
}

void displayResult(int pid, int* massNum)
{
	int number = 0;
	for (int i = 0; i < getArrayLength(massNum); i++)
	{
		number += massNum[i];
		number *= 10;
	}
	number /= 10;
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

int main(int argc, char* argv[])
{
	int pid;
	int numberProc = enterNumber();
	for (int i = 0; i < numberProc; i++)
	{
		pid = fork();
		if (pid == -1){
			printw("Error creating child proc");
			refresh();
			return 0;
		}
		if (pid == 0){
			int number[9];
			char path_out[20];
			sprintf(path_out, "Out %d.txt", getpid());
			readFromFile(number);
			qsort(number,getArrayLength(number),sizeof(int),cmp);
			writeToFile(path_out,number);
			displayResult(getpid(),number);	
			return 0;
		}else if(pid > 0){
			//parent proc
		}
	}

}
