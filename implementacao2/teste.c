#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMERO_PESSOAS 15

int lugares = 5;
int cheio = 0;
int sairam = 0;
pthread_mutex_t trava;
pthread_barrier_t barreira;
/*
void Comer(int thread_id, int wait)
{
    sleep(wait);
    pthread_mutex_lock(&trava);
    printf("Pessoa %d esta comendo.", thread_id);
    lugares--;
    printf("\t\t(%d lugares vagos)\t", lugares);

    if (lugares == 0)
        cheio = 1;
    pthread_mutex_unlock(&trava);
    printf("\tcheio = %d\n", cheio);
}

void Ir_Embora(int thread_id, int wait)
{
    sleep(wait);
    pthread_mutex_lock(&trava);
    printf("Pessoa %d terminou de comer.", thread_id);
    lugares++;
    printf("\t(%d lugares vagos)\t", lugares);

    if (cheio)
    {
        sairam++; //contando quantas pessoas acompanhadas foram embora
    }
    if (sairam >= 5)
    {
        cheio = 0;
        sairam = 0;
    }
    pthread_mutex_unlock(&trava);
    printf("\tcheio = %d\n", cheio);
}
*/
void *Sushi(void *thread)
{
    int thread_id = *(int *)thread;
    int wait_sec = rand() % 5 + 1;
    //printf("Pessoa %d chegou no sushiBar.\n", thread_id);
    if (cheio == 1)
    {
        printf("Pessoa %d entrou na fila.\n", thread_id);
        while (cheio == 1)
            sleep(3);
        //printf("Pessoa %d esta saindo da fila.\n", thread_id);
    }
    //Comer(thread_id, wait_sec);
    pthread_mutex_lock(&trava);
    
    printf("Pessoa %d esta comendo.", thread_id);
    lugares--;
    printf("\t\t(%d lugares vagos)\t", lugares);

    if (lugares == 0)
        cheio = 1;
    printf("\tcheio = %d\n", cheio);
    pthread_mutex_unlock(&trava);

    sleep(wait_sec);

    //Ir_Embora(thread_id, wait_sec);
    pthread_mutex_lock(&trava);
    printf("Pessoa %d terminou de comer.", thread_id);
    lugares++;
    printf("\t(%d lugares vagos)\t", lugares);

    if (cheio)
    {
        sairam++; //contando quantas pessoas acompanhadas foram embora
    }
    if (sairam >= 5)
    /*
    se apos todos os 
    lugares estarem ocupados, 5 pessoas sairem
    quer dizer q n esta mais cheio
    */
    {
        cheio = 0;
        sairam = 0;
    }
    printf("\tcheio = %d\n", cheio);
    pthread_mutex_unlock(&trava);
    //printf("Pessoa %d foi embora.\n", thread_id);

    return NULL;
}

int main()
{
    pthread_t thread[NUMERO_PESSOAS];
    int ids[NUMERO_PESSOAS];
    srand(time(NULL));
    //pthread_barrier_init(&barreira, NULL, NUMERO_PESSOAS + 1);

    if (pthread_mutex_init(&trava, NULL) != 0)
        return 1;

    for (int i = 0; i < NUMERO_PESSOAS; i++)
    {
        ids[i] = i;
        pthread_create(&thread[i], NULL, Sushi, &ids[i]);
    }
    //pthread_barrier_wait(&barreira);

    for (int i = 0; i < NUMERO_PESSOAS; i++)
        pthread_join(thread[i], NULL);

    pthread_mutex_destroy(&trava);
    //pthread_barrier_destroy(&barreira);
    return 0;
}