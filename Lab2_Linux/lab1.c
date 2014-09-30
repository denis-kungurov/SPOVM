#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <termios.h>

#define HANDLE int
int init(char* source, int id, int number)
{
    pid_t childPid = 0;
    char str[10] = {0}, strNum[5]={0};

    sprintf(str, "%d", id);
    sprintf(strNum,"%d",number);
    childPid = fork();
    if(!childPid)
    {
        execl(source, source, str, strNum, NULL);
    }
    return childPid;
}

int getch()
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

int setSemaphore(char* path)
{
    int id;

    id = ftok(path, 'x');
    printf("id = %d\n", id);
    id = semget(id, 1, IPC_CREAT);
    semctl(id, 0, SETVAL, 0);
    return id;
}

int main(int argc, char** argv)
{
    HANDLE pids[100];
    int pidPtr = 0;
    int end = 0;
    int semId = setSemaphore(argv[0]);

    while(!end)
        switch(getch()){
        case '+':
            if(pidPtr > 100)
                break;
            pids[pidPtr] = init("/home/deon/Desktop/lab2/x", semId, pidPtr);
            pidPtr++;
            break;
        case '-':
            if(!pidPtr)
                break;
            kill(pids[pidPtr - 1], SIGABRT);
            pidPtr--;
            break;
        case 'q':
            end = 1;
            break;
        }
    while(pidPtr)
    {
        kill(pids[pidPtr - 1], SIGABRT);
        pidPtr--;
    }
    semctl(semId, 0, IPC_RMID);
    return 0;
}
