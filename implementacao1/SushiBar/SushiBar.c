#include "SushiBar.h"

struct SushiBar
{
    sem_t seats_sem;
    sem_t mtx;
    pthread_barrier_t barr;
    int n;
    int count;
};

SushiBar *SushiBar_init(int max)
{
    SushiBar *new = (SushiBar *)malloc(sizeof(SushiBar));
    new->count = 0;
    new->n = max;

    sem_init(&new->mtx, 0, 1);

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
    sem_wait(&sb->mtx);
    sb->count++;
    printf("Thread id: %c ENTROU.\t count = %d\n", 'A' + tid, sb->count);
    sem_post(&sb->mtx);
    return;
}

//Função para saída do bar
void SushiBar_leave(SushiBar *sb, int tid)
{
    //Lockar a mutex para acessar 'count' e 'n' com segurança
    sem_wait(&sb->mtx);

    /*
        Caso a quantidade de pessoas no bar seja a maxima, 
        esperar os outros clientes para liberar o assento.

        Caso contrario não precisa esperar os outros
    */

    if (sb->count >= sb->n)
    {
        sem_post(&sb->mtx);

        pthread_barrier_wait(&sb->barr);

        sem_wait(&sb->mtx);
        sb->count--;
        printf("Thread id: %c SAIU.\t count = %d\n", 'A' + tid, sb->count);
        sem_post(&sb->mtx);

        pthread_barrier_wait(&sb->barr);
        sem_post(&sb->seats_sem);
        return;
    }
    else
    {
        sb->count--;
        sem_post(&sb->mtx);
        sem_post(&sb->seats_sem);
        return;
    }
    return;
}