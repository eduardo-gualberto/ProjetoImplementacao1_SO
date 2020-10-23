#include "SushiBar3.h"
#include <time.h>


struct SushiBar
{
    pthread_mutex_t mtx; //mutex para garantir exclusividade mutua quando acessar 'count'
    int n;               //número de lugares no bar
    int count;           //número de lugares ocupados no bar
};

typedef struct args_struct
{ //struct para a passagem de argumentos nas threads
    int thread_id;
    SushiBar *bar;
} args_struct;

SushiBar *SushiBar_init(int n_)
{
    //inicialização da estrutra do SushiBar
    SushiBar *new = (SushiBar *)malloc(sizeof(SushiBar));
    new->count = 0;
    new->n = n_;
    pthread_mutex_init(&new->mtx, NULL);

    return new;
}


void SushiBar_enter(SushiBar *sb, int tid)
{
    /*
        Verifica se todos os assentos estão sendo usados
        Se sim, entra num loop de espera até todos os assentos serem liberados (count == 0)
        Se não, entra no bar e utiliza um assento
    */
    pthread_mutex_lock(&sb->mtx);
VER:    if (sb->count == sb->n)
    {
        printf("Pessoa %c esperando a liberação de todos os assentos\n", 'A' + tid);
        pthread_mutex_unlock(&sb->mtx); //Mutex desbloqueia aqui para que não haja deadlock
        while (sb->count > 0)
        {
        }
        pthread_mutex_lock(&sb->mtx); //mutex bloqueia para incrementar count
        goto VER;
    }
    sb->count++; //incremente count -> ocupa um assento
    printf("Clinte %c ENTROU.\t assentos ocupados = %d\n", 'A' + tid, sb->count);
    pthread_mutex_unlock(&sb->mtx);
    return;
}
void SushiBar_leave(SushiBar *sb, int tid)
{

    //Sai do bar e libera um asssento
    pthread_mutex_lock(&sb->mtx);
    sb->count--;
    printf("Cliente %c SAIU.\t         assentos ocupados = %d\n\n", 'A' + tid, sb->count);
    pthread_mutex_unlock(&sb->mtx);
    return;
}
void *Sushi(void *argumentos)
{
    //recebe id e o sushi bar que foram passados por meio de um ponteiro struct_args
    args_struct *arguments = argumentos;
    int tid = arguments->thread_id;
    SushiBar *sb = arguments->bar;
    int wait_sec = rand() % 5 + 1;
    printf("Cliente %c chegou no sushiBar\n", 'A' + tid);
    SushiBar_enter(sb, tid); //tenta entrar no SushiBar
    sleep(wait_sec);         //Comendo
    SushiBar_leave(sb, tid); //Deixa o restaurante
    sleep(5);
    return NULL;
}

