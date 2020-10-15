#include "SushiBar.h"
#define N 5 //Número de assentos disponíveis
#define C 11 //Número de clientes


struct SushiBar
{
    pthread_mutex_t mtx;    //mutex para garantir exclusividade mutua quando acessar 'count'
    int n;     //número de lugares no bar
    int count; //número de lugares ocupados no bar
};

typedef struct args_struct { //struct para a passagem de argumentos nas threads
    int thread_id;  
    SushiBar *bar; 
}args_struct;

SushiBar *SushiBar_init(int n_)
{
    //inicialização da estrutra do SushiBar
    SushiBar *new = (SushiBar *)malloc(sizeof(SushiBar));
    new->count = 0;
    new->n = n_;

    pthread_mutex_init(&new->mtx, NULL);

    return new;
}

void SushiBar_destroy(SushiBar *sb)
{
    free(sb);
}

void SushiBar_enter(SushiBar *sb, int tid)
{
    /*
        Verifica se todos os assentos estão sendo usados
        Se sim, entra num loop de espera até todos os assentos serem liberados (count == 0)
        Se não, entra no bar e utiliza um assento
    */  
    pthread_mutex_lock(&sb->mtx); 
    if(sb->count == sb->n){
        printf("Pessoa %c esperando a liberação de todos os assentos\n", 'A' + tid);
        pthread_mutex_unlock(&sb->mtx); //Mutex desbloqueia aqui para que não haja deadlock
        while(sb->count > 0){
        }
        pthread_mutex_lock(&sb->mtx); //mutex bloqueia para incrementar count
    }
    sb->count++; //incremente count -> ocupa um assento
    printf("Pessoa %c ENTROU.\t count = %d\n", 'A' + tid, sb->count);
    pthread_mutex_unlock(&sb->mtx);
    return;

}
void SushiBar_leave(SushiBar *sb, int tid)
{

    //Sai do bar e libera um asssento
    pthread_mutex_lock(&sb->mtx);
    sb->count--;
    printf("Pessoa %c SAIU.\t count = %d\n",'A'+tid, sb->count);
    pthread_mutex_unlock(&sb->mtx);
    return;
    
}
void *Sushi( void * argumentos)
{
    //recebe id e o sushi bar que foram passados por meio de um ponteiro struct_args
    args_struct *arguments = argumentos;
    int tid = arguments->thread_id;
    SushiBar *sb = arguments->bar; 
    
    int wait_sec = rand() % 5 + 1;
    printf("Pessoa %c chegou no sushiBar\n", 'A'+tid);
    SushiBar_enter(sb,tid); //tenta entrar no SushiBar
    sleep(wait_sec); //Comendo
    SushiBar_leave(sb, tid); //Deixa o restaurante
    sleep(5);
    return NULL;
}
int main() {
    SushiBar *bar  = SushiBar_init(N);
    args_struct **argumentos_vec = (args_struct **)malloc(C*sizeof(args_struct*)); /* Estrutra para armazenar os argumentos de cada uma das threads
                                                                                      Feito usando um array de ponteiros pois só trocar os valores de um ponteiro
                                                                                      pode levar condições de corrida
                                                                                      */
    pthread_t thread[C];
    int ids[C];
    int i;
    srand(time(NULL));
    for (i=0;i<C;i++){
        ids[i] = i;
        argumentos_vec[i] = (args_struct *)malloc(sizeof(args_struct));  
        argumentos_vec[i]->thread_id = i; //coloca o id da thread e SushiBar nos argumentos
        argumentos_vec[i]->bar = bar;
        pthread_create(&thread[i], NULL, Sushi,(void *)argumentos_vec[i]); //Passa para a thread a execução da função Sushi com os argumentos relativos a iésiama thread

    }
     for (int i = 0; i < C; i++)
        pthread_join(thread[i], NULL);

    free(argumentos_vec);
    return 0;
}
