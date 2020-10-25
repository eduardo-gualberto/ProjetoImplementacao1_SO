#include "SushiBar/SushiBar.h"
#include <time.h>
#include <sys/time.h>
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
        THREAD_N = atoi(argv[1]);
        SEATS_N = atoi(argv[2]);
        printf("Executing with %d threads and %d seats.\n\n", THREAD_N, SEATS_N);
    }
    else if (argc == 2)
    {
        printf("Usage: './compileandrun 1' wich defaults to 5 seats and 15 threads,\nor specify the amount of seats and threads with: './main <seats> <thread_num>'.\n");
        return 0;
    }
    else
    {
        printf("Usage: './compileandrun 1 <thread_num> <seats>'.\nDefaults to thread_num = 15 and seats = 5.\n\n");
    }

    sb = SushiBar_init(SEATS_N, THREAD_N);
    pthread_t t[THREAD_N];

    struct timeval begin, end;

    gettimeofday(&begin, NULL);
    // clock_t begin = clock();
    for (int i = 0; i < THREAD_N; i++)
        pthread_create(&t[i], NULL, thread_entry, i);

    for (int i = 0; i < THREAD_N; i++)
        pthread_join(t[i], NULL);
    // clock_t end = clock();
    gettimeofday(&end, NULL);

    // double duration = (double)(end - begin) / CLOCKS_PER_SEC;
    double duration = (double)(end.tv_sec - begin.tv_sec) + (double)(end.tv_usec - begin.tv_usec) / 10000;

    printf("Took %fs to execute.\n", duration);

    SushiBar_destroy(sb);

    return 0;
}