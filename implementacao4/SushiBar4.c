#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t condicao;
pthread_barrier_t barreira;
int lugares_ocupados = 0;
int numero_lugares;

void *SushiBar(void* thread) {
    int thread_id = *(int*) thread;
    int wait_sec = rand() % 5 + 1;
    
    // Entrada dos Clientes:

    pthread_mutex_lock(&mutex);
    
    while (lugares_ocupados == numero_lugares) {

        //sleep(wait_sec);
        printf("Cliente %d esperando...\n", thread_id);        

        pthread_cond_wait(&condicao, &mutex);

        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
    }

    lugares_ocupados++;

    if (lugares_ocupados > numero_lugares) {
        lugares_ocupados = numero_lugares;
    }

    printf("Cliente %d ENTROU\n", thread_id);    

    pthread_mutex_unlock(&mutex);

    sleep(wait_sec);

    // Saída dos Clientes:

    pthread_mutex_lock(&mutex);

    if (lugares_ocupados == numero_lugares) {
        pthread_mutex_unlock(&mutex);
        pthread_barrier_wait(&barreira);
        

        pthread_mutex_lock(&mutex);
        lugares_ocupados--;
        printf("Cliente %d terminou de comer\n", thread_id);
        sleep(1);
        printf("Dentro de esta_lotado: Cliente %d SAIU\n", thread_id);
        pthread_mutex_unlock(&mutex);

        pthread_barrier_wait(&barreira);

        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&condicao);
    } else {
        lugares_ocupados--;
        printf("Cliente %d terminou de comer\n", thread_id);
        sleep(1);
        printf("Não lotado: Cliente %d SAIU\n", thread_id);
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&condicao);

    }
    
    pthread_mutex_unlock(&mutex);

}



int main(int argc, char *argv[]) {

    int numero_clientes = 12;                   // Número de Clientes Padrão
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
    int status;                                 // Checa o status de cada pthread (sucesso / falha)
    srand(time(NULL));

    // Inicializações:
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condicao, NULL);
    pthread_barrier_init(&barreira, NULL, numero_lugares);

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

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condicao);
    pthread_barrier_destroy(&barreira);

    return 0;

}

