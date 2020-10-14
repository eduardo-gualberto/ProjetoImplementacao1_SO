#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMERO_PESSOAS 10
#define X 2
int LUGARES_VAGOS = 5;
int cheio = 0;

pthread_barrier_t barreira;

void Comer(){
    LUGARES_VAGOS--;
    printf("%d lugares vagos.\n", LUGARES_VAGOS);

    if (LUGARES_VAGOS == 0){
        cheio = 1;
    }
}

void Ir_Embora (int thread_id){
    printf("Pessoa %d terminou de comer.\n", thread_id);
    LUGARES_VAGOS++;
    printf("Lugares %d vagos.\n", LUGARES_VAGOS);

    if (LUGARES_VAGOS == 5){
        cheio = 0;
    }
}

void *Sushi(void *thread)
{
    int thread_id = *(int *)thread;

    int wait_sec = rand() % 5 + 1;

    printf("Pessoa %d chegou no sushiBar.\n", thread_id);
    if (LUGARES_VAGOS < 1)
    {
        printf("Pessoa %d entrou na fila.\n", thread_id);
        while (cheio == 1){
            sleep(wait_sec);
            pthread_barrier_wait(&barreira);
            /* problema para sair do while
            quando as 5 mesas estão livres*/
        }
        printf("Pessoa %d esta saindo da fila.\n", thread_id);
        pthread_barrier_wait(&barreira);
    }
    Comer();

    sleep(wait_sec);
    pthread_barrier_wait(&barreira);

    Ir_Embora(thread_id);
    
    pthread_barrier_wait(&barreira);
    sleep(5);
    return NULL;
}

int main()
{
    pthread_t thread[NUMERO_PESSOAS];
    int ids[NUMERO_PESSOAS];
    srand(time(NULL));
    pthread_barrier_init(&barreira, NULL, NUMERO_PESSOAS + 1);

    for (int i = 0; i < NUMERO_PESSOAS; i++)
    {
        ids[i] = i;
        pthread_create(&thread[i], NULL, Sushi, &ids[i]);
    }
    pthread_barrier_wait(&barreira);

    for (int i = 0; i < NUMERO_PESSOAS; i++)
        pthread_join(thread[i], NULL);

    pthread_barrier_destroy(&barreira);
    return 0;
}