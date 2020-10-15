#include "SushiBar.h"

struct SushiBar
{
    sem_t seats_sem;        //semaforo para controlar a entrada no restaurante
    pthread_mutex_t mtx;    //mutex para garantir exclusividade mutua quando acessar 'count' e 'n'
    pthread_barrier_t barr; //barreira para o caso de bar lotado
    Queue_t q;              //fila para garantir espera justa

    //variaveis de controle
    int n;       //número de lugares no bar
    int count;   //número de lugares ocupados no bar
    int waiting; //número de pessoas aguardando
};

//Inicializa a struct de acordo com o número de mesas disponíveis
SushiBar *SushiBar_init(int n_, int t_)
{
    SushiBar *new = (SushiBar *)malloc(sizeof(SushiBar));
    new->count = new->waiting = 0;
    new->n = n_;

    pthread_mutex_init(&new->mtx, NULL);

    pthread_barrier_init(&(new->barr), NULL, new->n);

    sem_init(&(new->seats_sem), 0, new->n);

    q_init(&new->q, sizeof(int), t_, FIFO, 0);

    return new;
}

void SushiBar_destroy(SushiBar *sb)
{
    q_kill(&sb->q);
    free(sb);
}

//Função para saída do bar
void SushiBar_enter(SushiBar *sb, int tid)
{

    /*
        Toma decisões comuns para ambos os casos: bar lotado ou não
        De toda forma sinalizar ao semaforo interesse em um lugar no bar
    */

    pthread_mutex_lock(&sb->mtx);
    int test_queue = (sb->count >= sb->n);
    if (test_queue)
    {
        sb->waiting++;
        printf("Customer id: %d is waiting in queue. There are %d people waiting.\n", tid, sb->waiting);
    }
    pthread_mutex_unlock(&sb->mtx);

    // sem_wait(&(sb->seats_sem)); //aguardar lugar disponível no bar
    SushiBar_queue(sb, tid);

    //incrementa variavel controladora
    pthread_mutex_lock(&sb->mtx);
    if (test_queue)
        sb->waiting--;
    printf("Customer id: %d took a seat. There are %d seats available and %d unavailable.\n", tid, sb->n - sb->count, sb->count);
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
        printf("Customer id: %d left. There are %d seats available.\n", tid, sb->n - sb->count);
        pthread_mutex_unlock(&sb->mtx);

        pthread_barrier_wait(&sb->barr); // aguardar que todos decrementem a variavel de controle para então sair juntos

        sem_post(&sb->seats_sem);
        return;
    }
    else
    {
        sb->count--;
        printf("Customer id: %d left. There are %d seats available.\n", tid, sb->n - sb->count);
        pthread_mutex_unlock(&sb->mtx);
        sem_post(&sb->seats_sem);
        return;
    }
    return;
}

void SushiBar_queue(SushiBar *sb, int tid)
{
    pthread_mutex_lock(&sb->mtx);
    if (sb->count >= sb->n)
    {
        q_push(&sb->q, (void *)(&tid));
        // printf("Entrou pra fila\n");
        pthread_mutex_unlock(&sb->mtx);
        int *aux = (int *)malloc(sizeof(int));
        int notempty;
    WAIT:
        do
        {
            pthread_mutex_lock(&sb->mtx);
            q_peek(&sb->q, aux);
            // printf("Entrou no loop\n");
            // printf("Checou o primeiro da fila\n");
            notempty = sb->count > 0;
            pthread_mutex_unlock(&sb->mtx);
        } while (notempty && *aux != tid);

        pthread_mutex_lock(&sb->mtx);
        if (sb->count < sb->n && *aux == tid)
        {
            sb->count++;
            pthread_mutex_unlock(&sb->mtx);
            sem_wait(&sb->seats_sem);
            pthread_mutex_lock(&sb->mtx);
            q_pop(&sb->q, aux);
            pthread_mutex_unlock(&sb->mtx);
            return;
        }
        else
        {
            pthread_mutex_unlock(&sb->mtx);
            goto WAIT;
        }
    }
    else
    {
        sb->count++;
        pthread_mutex_unlock(&sb->mtx);
        sem_wait(&sb->seats_sem);
        return;
    }
}