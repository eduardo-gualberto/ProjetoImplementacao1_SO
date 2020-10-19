/*
    compilar : gcc -pthread main.c SushiBar/SushiBar.c -o buid/main
    rodar    : ./build/main
*/

#include "SushiBar/SushiBar.h"
#include <time.h>
#include <math.h>
#include <unistd.h>

SushiBar *sb;

void *thread_entry(int tid)
{
    SushiBar_enter(sb, tid);

    sleep((rand() % 5) + 1); //comer....

    SushiBar_leave(sb, tid);
    return NULL;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int SEATS_N = 5, THREAD_N = 15;

    printf("\n\n");

    if (argc > 2)
    {
        SEATS_N = atoi(argv[1]); 
        THREAD_N = atoi(argv[2]);
        printf("Executing with %d seats and %d threads.\n\n", SEATS_N, THREAD_N);
    }
    else if (argc == 2)
    {
        printf("Usage: './main' wich defaults to 5 seats and 15 threads,\nor specify the amount of seats and threads with: './main <seats> <thread_num>'.\n");
        return 0;
    }
    else
    {
        printf("Usage: './main <seats> <thread_num>'.\nDefaults to seats = 5, thread_num = 15.\n\n");
    }

    sb = SushiBar_init(SEATS_N, THREAD_N);
    pthread_t t[THREAD_N];

    for (int i = 0; i < THREAD_N; i++)
        pthread_create(&t[i], NULL, thread_entry, i);

    for (int i = 0; i < THREAD_N; i++)
        pthread_join(t[i], NULL);

    SushiBar_destroy(sb);

    return 0;
}