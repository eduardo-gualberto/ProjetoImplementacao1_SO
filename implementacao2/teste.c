#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMERO_PESSOAS 22

int lugares = 5;
int cheio = 0;
int sairam = 0;
pthread_mutex_t trava;

int verifica_Cheio(){
    int esta_cheio;
    pthread_mutex_lock(&trava);
    esta_cheio = cheio;
    pthread_mutex_unlock(&trava);
    return esta_cheio;
}

//Solução q n eh justa, pois ordem de chegada n eh mantida.
void *Sushi(void *thread)
{
    int thread_id = *(int *)thread;
    int wait_sec = rand() % 5 + 1;  //tempo q a pessoa ficara no bar
    //printf("Pessoa %d chegou no sushiBar.\n", thread_id);
    pthread_mutex_lock(&trava);
    if (cheio)  //se o bar estiver cheio a pessoa vai para fila
    {
        printf("Pessoa \t%d\t entrou na fila.\n", thread_id + 1);
FILA:   pthread_mutex_unlock(&trava);
        while (verifica_Cheio())    //verificando se eh possivel entrar no bar
        {
            sleep(wait_sec);
            //printf("**Pessoa %d ainda esta na fila.\n", thread_id);
        }
        //duas pessoas ainda estao saindo da fila ao msm tempo
        //possivel solução: uso de função ou goto :P
        pthread_mutex_lock(&trava);
    }
    lugares--;  //pessoa entra no restaurante
    if (lugares == 0)
        cheio = 1;  //se n houver mais lugares quer dizer q esta cheio
    if (lugares < 0){
        //printf("\nPessoa \t%d\t esta voltando para a fila.\n\n", thread_id + 1);
        lugares++;
        goto FILA;      //GOTO provisorio, mas q salva o dia
    }
    printf("Pessoa \t%d\t esta comendo.\t\t\t(%d lugares vagos) CHEIO = %d\n", thread_id + 1, lugares, cheio);
    pthread_mutex_unlock(&trava);

    sleep(wait_sec);    //pessoa esta dentro do restaurante comendo

    pthread_mutex_lock(&trava);
    lugares++;  //pessoa esta indo embora
    if (cheio)
        sairam++; //contando quantas pessoas acompanhadas foram embora
    if (sairam >= 5){               //se apos todos os lugares estarem ocupados,
        cheio = sairam = 0;         //5 pessoas sairem, quer dizer q n esta mais cheio   
    }
    printf("Pessoa \t%d\t terminou de comer.\t\t(%d lugares vagos) CHEIO = %d\n", thread_id + 1, lugares, cheio);
    pthread_mutex_unlock(&trava);
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