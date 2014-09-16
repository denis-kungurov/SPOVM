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
	attron(COLOR_PAIR(getpid()%6 + 1));
	printw("Process %d result: %d\n", pid,number);
}

int enterNumber()
{
	int number;
	printw("Enter number of process: ");
	while (true)
	{
		fflush(stdin);
		scanw("%d", &number);
		if (number <= 0 || number >= 100) printw("Enter correct, not-null, less 100 number: ");
		else break;
	}
	return number;
}

void ncursesColorInitPairs()
{	
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
}

void createProcess(int numberProc)
{
	int pid;
	for (int i = 0; i < numberProc; i++)
	{
		pid = fork();
		if (pid == -1){
			printw("Error creating child proc");
			refresh();
			return;
		}
		if (pid == 0){
			int number[9];
			char path_out[20];
			sprintf(path_out, "Out %d.txt", getpid());
			readFromFile(number);
			qsort(number,getArrayLength(number),sizeof(int),cmp);
			writeToFile(path_out,number);
			displayResult(getpid(),number);	
			return;
		}else if(pid > 0){
			//parent proc
		}
	}
}

int main(int argc, char* argv[])
{
	initscr();
    start_color();
    ncursesColorInitPairs();
	attron(COLOR_PAIR(0));
	int numberProc = enterNumber();
	createProcess(numberProc);
	refresh();
	getch();
	endwin();
	return 0;
}
