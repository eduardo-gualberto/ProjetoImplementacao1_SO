#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMERO_PESSOAS 12

int lugares = 5;
int cheio = 0;
int sairam = 0;
pthread_mutex_t trava;

void *Sushi(void *thread)
{
    int thread_id = *(int *)thread;
    int wait_sec = rand() % 5 + 1;
    //printf("Pessoa %d chegou no sushiBar.\n", thread_id);
    pthread_mutex_lock(&trava);
    if (cheio == 1)
    {
        printf("Pessoa %d entrou na fila.\n", thread_id);
        pthread_mutex_unlock(&trava);
        while (cheio == 1)
            sleep(wait_sec);
        printf("Pessoa %d esta saindo da fila.\n", thread_id);
        pthread_mutex_lock(&trava);
    }
    lugares--;
    if (lugares == 0)
        cheio = 1;
    printf("Pessoa %d esta comendo.\t\t\t(%d lugares vagos) CHEIO = %d\n", thread_id, lugares, cheio);
    pthread_mutex_unlock(&trava);

    sleep(wait_sec);

    pthread_mutex_lock(&trava);
    lugares++;
    if (cheio)
    {
        sairam++; //contando quantas pessoas acompanhadas foram embora
    }
    pthread_mutex_unlock(&trava);
    sleep(wait_sec);

    if (sairam >= 5)
    {
    /*
    se apos todos os 
    lugares estarem ocupados, 5 pessoas sairem
    quer dizer q n esta mais cheio
    */
        pthread_mutex_lock(&trava);
        cheio = sairam = 0;
        pthread_mutex_unlock(&trava);
    }
    printf("Pessoa %d terminou de comer.\t\t(%d lugares vagos) CHEIO = %d\n", thread_id, lugares, cheio);

    return NULL;
}

int main()
{
    pthread_t thread[NUMERO_PESSOAS];
    int ids[NUMERO_PESSOAS];
    srand(time(NULL));

    if (pthread_mutex_init(&trava, NULL) != 0)
        return 1;

    for (int i = 0; i < NUMERO_PESSOAS; i++)
    {
        ids[i] = i;
        pthread_create(&thread[i], NULL, Sushi, &ids[i]);
    }

    for (int i = 0; i < NUMERO_PESSOAS; i++)
        pthread_join(thread[i], NULL);

    pthread_mutex_destroy(&trava);
    return 0;
}