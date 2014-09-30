#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main(int argc, char** argv)
{
    int id, i = 0, number, pid = 0;
    struct sembuf dec, inc, w8;

    pid = getpid();
    dec.sem_num = 0;
    dec.sem_op = -1;
    inc.sem_num = 0;
    inc.sem_op = 1;
    w8.sem_num = 0;
    w8.sem_op = 0;
    id = atoi(argv[1]);
    number = atoi(argv[2]);
    if(argc < 2)
        return 0;
    while(1)
    {
        semop(id, &w8, 1);
        semop(id, &inc, 1);
        printf("%d ", pid);
        for(i = 0; i < 5; i++)
        {
            printf("%d", number);
        }
        printf("\n");
        semop(id, &dec, 1);
    }
    return 0;
}