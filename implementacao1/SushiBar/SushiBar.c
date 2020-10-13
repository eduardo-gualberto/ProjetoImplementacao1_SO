#include "SushiBar.h"

struct SushiBar
{
    sem_t seats_sem;
    pthread_mutex_t mtx;
    pthread_barrier_t barr;
    int n;
    int count;
};

SushiBar *SushiBar_init(int max)
{
    SushiBar *new = (SushiBar *)malloc(sizeof(SushiBar));
    new->count = 0;
    new->n = max;

    pthread_mutex_t aux = PTHREAD_MUTEX_INITIALIZER;

    new->mtx = aux;

    pthread_barrier_init(&(new->barr), NULL, new->n);

    sem_init(&(new->seats_sem), 0, new->n);

    return new;
}

void SushiBar_destroy(SushiBar *sb)
{
    free(sb);
}

//Função para saída do bar
void SushiBar_enter(SushiBar *sb, int tid)
{

    /*
        Toma decisões comuns para ambos os casos: bar lotado ou não
        De toda forma sinalizar ao semaforo interesse em um lugar no bar
    */

    sem_wait(&(sb->seats_sem));
    pthread_mutex_lock(&sb->mtx);
    sb->count++;
    printf("Thread id: %d ENTROU.\t count = %d\n", tid, sb->count);
    pthread_mutex_unlock(&sb->mtx);
    return;
}

//Função para saída do bar
void SushiBar_leave(SushiBar *sb, int tid)
{
    //Lockar a mutex para acessar 'count' e 'n' com segurança
    pthread_mutex_lock(&sb->mtx);

    /*
        Caso a quantidade de pessoas no bar seja a maxima, 
        esperar os outros clientes para liberar o assento.

        Caso contrario não precisa esperar os outros
    */

    if (sb->count >= sb->n)
    {
        pthread_mutex_unlock(&sb->mtx);

        pthread_barrier_wait(&sb->barr);

        pthread_mutex_lock(&sb->mtx);
        sb->count--;
        printf("Thread id: %d SAIU.\t count = %d\n", tid, sb->count);
        pthread_mutex_unlock(&sb->mtx);

        pthread_barrier_wait(&sb->barr);
        sem_post(&sb->seats_sem);
        return;
    }
    else
    {
        pthread_mutex_unlock(&sb->mtx);
        pthread_mutex_lock(&sb->mtx);
        sb->count--;
        pthread_mutex_unlock(&sb->mtx);
        sem_post(&sb->seats_sem);
        return;
    }
    return;
}