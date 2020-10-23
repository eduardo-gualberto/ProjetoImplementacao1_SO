#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

pthread_mutex_t mutex;          // mutex para garantir o controle das variáveis
pthread_cond_t condicao;        // condição para controlar a entrada dos clientes
pthread_barrier_t barreira;     // barreira controla a saída quando o bar está lotado
int lugares_ocupados = 0;       // lugares OCUPADOS no bar
int numero_lugares;             // Número total de lugares (assentos) no bar

// Protótipos de Funções:
void EntraCliente(int thread_id);
void EsperaNaFila(int thread_id);
void SaiCliente(int thread_id);


/* 
   Esta função recebe o id da thread como parâmetro e 
   realiza a entrada dos clientes no bar 
*/
void EntraCliente(int thread_id) {
    pthread_mutex_lock(&mutex);
    
    // Caso o bar esteja lotado:
    while (lugares_ocupados == numero_lugares) {
        EsperaNaFila(thread_id);
    }

    // Entrada do Cliente
    lugares_ocupados++;

    //printf("Lugares Ocupados: %d\n", lugares_ocupados);
    printf("Cliente %d ENTROU\n", thread_id);    

    pthread_mutex_unlock(&mutex);
}


/*
   Esta função recebe o id da thread como parâmetro e 
   coloca as threads em espera
*/
void EsperaNaFila(int thread_id) {
    printf("Cliente %d esperando...\n", thread_id);        

    pthread_cond_wait(&condicao, &mutex);       // Bloqueia as threads, esperando a liberação dos lugares no bar (lotado)   
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
}


/* 
   Esta função recebe o id da thread como parâmetro e 
   realiza a saída do cliente do bar.
   Se está lotado: Saem todos os clientes dentro do bar
   Caso contrário: Cliente sai do bar normalmente 
*/
void SaiCliente(int thread_id) {
    pthread_mutex_lock(&mutex);

    if (lugares_ocupados == numero_lugares) {
        pthread_mutex_unlock(&mutex);
        pthread_barrier_wait(&barreira);        
        
        pthread_mutex_lock(&mutex);
        lugares_ocupados--;
        
        printf("Cliente %d terminou de comer\n", thread_id);
        printf("Lotado: Cliente %d SAIU\n", thread_id);
        
        pthread_mutex_unlock(&mutex);

        pthread_barrier_wait(&barreira);        // Barreira espera todos saírem do bar para sinalizar para novos clientes entrarem

        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&condicao);      // Sinaliza para as threads que o bar está vazio
    } else {
        lugares_ocupados--;
        printf("Cliente %d terminou de comer\n", thread_id);
        printf("Não lotado: Cliente %d SAIU\n", thread_id);

    }
    pthread_mutex_unlock(&mutex);
}


// Função da Thread
void *SushiBar(void* thread) {
    int thread_id = *(int*) thread;
    int wait_sec = rand() % 5 + 1;

    EntraCliente(thread_id);
    sleep(wait_sec);
    SaiCliente(thread_id);
}


// Função Principal
int main(int argc, char *argv[]) {

    int numero_clientes = 12;                   // Número Padrão de Clientes 
    numero_lugares = 5;                         // Número Padrão de Lugares no Bar

    // Recebendo Parâmetros por Linha de Comando
    if (argv[1] != NULL && atoi(argv[1]) > 0) { 
        numero_clientes = atoi(argv[1]);
    }
    
    if (argv[2] != NULL && atoi(argv[2]) > 0) {
        numero_lugares = atoi(argv[2]);
    }

    pthread_t threads[numero_clientes];
    unsigned int thread_ids[numero_clientes];
    int status;                                 // Checa o status de criação de cada pthread (sucesso / falha)
    srand(time(NULL));

    // Inicializações:
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condicao, NULL);
    pthread_barrier_init(&barreira, NULL, numero_lugares);

    // Determinando o Tempo de Execução usando a struct timeval:
    struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);

    for(int i = 0; i < numero_clientes; i++) {
        thread_ids[i] = i;
        status = pthread_create(&threads[i], NULL, SushiBar, &thread_ids[i]);

        if (status != 0) {
            printf("Erro ao criar thread\n");
            return -1;
        }

        //printf("\nThread %d criada com sucesso!\n\n", thread_ids[i]);
    }

    for (int j = 0; j < numero_clientes; j++) {
        pthread_join(threads[j], NULL);
    }

    gettimeofday(&fim, NULL);
    printf("Tempo de Execução: %lf\n", (double)(fim.tv_sec - inicio.tv_sec) + (double)(fim.tv_usec - inicio.tv_usec)/10000);       // Tempo de Execução
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condicao);
    pthread_barrier_destroy(&barreira);

    return 0;

}

