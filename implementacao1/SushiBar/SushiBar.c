#include "SushiBar.h"

struct SushiBar
{
    sem_t seats_sem;        //semaforo para controlar a entrada no restaurante
    pthread_mutex_t mtx;    //mutex para garantir exclusividade mutua quando acessar 'count' e 'n'
    pthread_barrier_t barr; //barreira para o caso de bar lotado

    //variaveis de controle
    int n;     //número de lugares no bar
    int count; //número de lugares ocupados no bar
};

//Inicializa a struct de acordo com o número de mesas disponíveis
SushiBar *SushiBar_init(int n_)
{
    SushiBar *new = (SushiBar *)malloc(sizeof(SushiBar));
    new->count = 0;
    new->n = n_;

    pthread_mutex_init(&new->mtx, NULL);

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

    sem_wait(&(sb->seats_sem)); //aguardar lugar disponível no bar

    //incrementa variavel controladora
    pthread_mutex_lock(&sb->mtx);
    sb->count++;
    printf("Thread id: %c ENTROU.\t count = %d\n", 'A' + tid, sb->count);
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

        pthread_barrier_wait(&sb->barr); //aguardar todos chegarem a este ponto para decrementar variavel de controle

        pthread_mutex_lock(&sb->mtx);
        sb->count--;
        printf("Thread id: %c SAIU.\t count = %d\n", 'A' + tid, sb->count);
        pthread_mutex_unlock(&sb->mtx);

        pthread_barrier_wait(&sb->barr); // aguardar que todos decrementem a variavel de controle para então sair juntos
        sem_post(&sb->seats_sem);
        return;
    }
    else
    {
        sb->count--;

        pthread_mutex_unlock(&sb->mtx);
        sem_post(&sb->seats_sem);
        return;
    }
    return;
}