#include "SushiBar/SushiBar.h"
#include <time.h>
#include <math.h>
#include <unistd.h>

#define SEATS_N 5
#define THREAD_N 15

SushiBar *sb;

void *thread_entry(void *tid)
{
    SushiBar_enter(sb, *(int *)tid);
    sleep(3);
    SushiBar_leave(sb, *(int *)tid);
    return NULL;
}

int main(int argc, char *argv[])
{
    sb = SushiBar_init(SEATS_N);
    pthread_t t[THREAD_N];

    for (int i = 0; i < THREAD_N; i++)
    {
        pthread_create(&t[i], NULL, thread_entry, &i);
    }

    for (int i = 0; i < THREAD_N; i++)
        pthread_join(t[i], NULL);

    SushiBar_destroy(sb);

    return 0;
}