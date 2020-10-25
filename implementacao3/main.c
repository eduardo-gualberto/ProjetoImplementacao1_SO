#include "SushiBar3/SushiBar3.h"
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

SushiBar *bar;

typedef struct args_struct
{ //struct para a passagem de argumentos nas threads
    int thread_id;
    SushiBar *bar;
} args_struct;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int i;
    int SEATS_N = 5, THREAD_N = 15;
    args_struct **argumentos_vec = (args_struct **)malloc(THREAD_N * sizeof(args_struct *)); /* Estrutra para armazenar os argumentos de cada uma das threads
                                                                                      Feito usando um array de ponteiros pois só trocar os valores de um ponteiro
                                                                                      pode levar condições de corrida
                                                                                      */
    printf("\n\n");

    if (argc > 2)
    {
        THREAD_N = atoi(argv[1]);
        SEATS_N = atoi(argv[2]);
        printf("Executing with %d threads and %d seats.\n\n", THREAD_N, SEATS_N);
    }
    else if (argc == 2)
    {
        printf("Usage: './compileandrun 1' wich defaults to 5 seats and 15 threads,\nor specify the amount of seats and threads with: './main <seats> <thread_num>'.\n");
        return 0;
    }
    else
    {
        printf("Usage: './compileandrun 1 <thread_num> <seats>'.\nDefaults to thread_num = 15 and seats = 5.\n\n");
    }

    bar = SushiBar_init(SEATS_N);
    pthread_t t[THREAD_N];
 
        struct timeval inicio, fim;
    gettimeofday(&inicio, NULL);
    for (i = 0; i < THREAD_N; i++){
        argumentos_vec[i] = (args_struct *)malloc(sizeof(args_struct));
        argumentos_vec[i]->thread_id = i; //coloca o id da thread e SushiBar nos argumentos
        argumentos_vec[i]->bar = bar;
        pthread_create(&t[i], NULL, Sushi, (void *)argumentos_vec[i]);
    }
    for (i = 0; i < THREAD_N; i++)
        pthread_join(t[i], NULL);
    
    gettimeofday(&fim, NULL);
    printf("\n***** Tempo de Execução: %lf segundos ******\n", (double)(fim.tv_sec - inicio.tv_sec) + (double)(fim.tv_usec - inicio.tv_usec)/10000);

    free(argumentos_vec);

    return 0;
}
