#include "SushiBar.h"

struct SushiBar
{
    sem_t seats_sem;        //semaforo para controlar a entrada no restaurante
    pthread_mutex_t mtx;    //mutex para garantir exclusividade mutua quando acessar as variaveis de controle
    pthread_barrier_t barr; //barreira para o caso de bar lotado

    //variaveis de controle
    Queue_t q;   //fila para garantir espera justa
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
    pthread_mutex_destroy(&sb->mtx);
    pthread_barrier_destroy(&sb->barr);
    sem_destroy(&sb->seats_sem);
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

        pthread_barrier_wait(&sb->barr); //aguardar todos chegarem a este ponto para decrementar variavel de controle 'count'

        pthread_mutex_lock(&sb->mtx);
        sb->count--;
        printf("Customer id: %d left. There are %d seats available.\n", tid, sb->n - sb->count);
        pthread_mutex_unlock(&sb->mtx);

        pthread_barrier_wait(&sb->barr); // aguardar que todos decrementem a variavel de controle para então sair juntos

        sem_post(&sb->seats_sem);
        return;
    }
    else //caso não esteja lotado apenas sair do bar
    {
        sb->count--;
        printf("Customer id: %d left. There are %d seats available.\n", tid, sb->n - sb->count);
        pthread_mutex_unlock(&sb->mtx);
        sem_post(&sb->seats_sem);
        return;
    }
    return;
}

//Gerencia o fluxo de threads por meio de uma fila
void SushiBar_queue(SushiBar *sb, int tid)
{
    /*
        Aqui existem dois casos: bar lotado ou bar não-lotado.
        Bar lotado: entrar numa fila até que você seja o primeiro da fila e o bar se esvazie
        Bar não-lotado: entrar direto para o bar, sem necessidade de fila
    */

    pthread_mutex_lock(&sb->mtx);
    if (sb->count >= sb->n) //verifica se está lotado
    {
        q_push(&sb->q, (void *)(&tid)); //colocar na fila a id da thread que deseja entrar

        pthread_mutex_unlock(&sb->mtx);
        int *aux = (int *)malloc(sizeof(int)); //variavel para realizar a interação com a fila
        int notempty;                          //condição do loop
    //busy wait
    WAIT:
        do
        {
            pthread_mutex_lock(&sb->mtx);
            q_peek(&sb->q, aux); //varifica o primeiro da fila (sem retirá-lo da fila) e coloca em 'aux'

            notempty = sb->count > 0; //testa para ver se o bar já se esvaziou

            pthread_mutex_unlock(&sb->mtx);
        } while (notempty && *aux != tid); //se for o primeiro da fila ou o bar estiver se esvaziado, saia

        pthread_mutex_lock(&sb->mtx);
        if (sb->count < sb->n && *aux == tid) //controle para garantir que o bar ainda tem espaço para abrigar o primeiro da fila
        {
            //entra para o bar
            sb->count++;
            pthread_mutex_unlock(&sb->mtx);
            sem_wait(&sb->seats_sem);
            pthread_mutex_lock(&sb->mtx);

            //libera lugar na fila
            q_pop(&sb->q, aux);
            pthread_mutex_unlock(&sb->mtx);
            return;
        }
        else //aqui faz com que o excedente de threads retornem para a espera
        {
            pthread_mutex_unlock(&sb->mtx);
            goto WAIT;
        }
    }
    else // caso não esteja lotado, apenas entrar para o bar
    {
        sb->count++;
        pthread_mutex_unlock(&sb->mtx);
        sem_wait(&sb->seats_sem);
        return;
    }
}