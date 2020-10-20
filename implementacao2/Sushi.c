#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

int lugares;           //numero de lugares atualmente ocupados
int n_assentos;        //numero de assentos maximos do restaurante
bool cheio = false;    //indica se restaurante esta cheio
bool ultimo;           //marca a ultima pessoa a entrar no bar antes de lotar
int sairam = 0;        //indica o numero de pessoas acompanhadas q sairam do SushiBar
pthread_mutex_t trava; //mutex para controle

//funçao q verifica se o restaurante ainda esta cheio
bool verifica_Cheio()
{
    bool esta_cheio;
    pthread_mutex_lock(&trava);
    esta_cheio = cheio;
    pthread_mutex_unlock(&trava);
    return esta_cheio;
}

//funçao q manda pessoa para a fila
void espera_fila(int wait_sec, int pessoa_N)
{
    pthread_mutex_unlock(&trava);
    printf("Pessoa \t%d\t entrou na fila.\n", pessoa_N);
    while (verifica_Cheio()) //verificando se eh possivel entrar no bar
        sleep(wait_sec);
    pthread_mutex_lock(&trava);
}

//funçao q faz pessoa entrar no restaurante
bool entrar_SushiBar(int pessoa_N)
{
    lugares--; //pessoa entra no restaurante
    if (lugares <= 0 && ultimo)
    {
        printf("\nPessoa \t%d\t esta voltando para a fila.\n\n", pessoa_N);
        lugares++;
        return false; //se duas pessoaa tentarem entrar ao msm tempo uma eh mandada de novo pra fila
    }
    if (lugares == 0)
        cheio = ultimo = true; //se n houver mais lugares quer dizer q esta cheio e pessoa foi a ultima a entrar
    printf("Pessoa \t%d\t esta comendo.\t\t\t(%d lugares vagos) CHEIO = %d\n", pessoa_N, lugares, cheio);
    pthread_mutex_unlock(&trava);
    return true;    //pessoa conseguiu entrar
}

//funçao q faz pessoa sair do restaurante
void sair_SushiBar(int pessoa_N)
{
    pthread_mutex_lock(&trava);
    lugares++; //pessoa esta indo embora
    if (cheio)
        sairam++; //contando quantas pessoas acompanhadas foram embora
    if (sairam >= n_assentos)
    {                                //se apos todos os lugares estarem ocupados,
        sairam = 0; //5 pessoas sairem, quer dizer q n esta mais cheio
        ultimo = cheio = false;
    }
    printf("Pessoa \t%d\t terminou de comer.\t\t(%d lugares vagos) CHEIO = %d\n", pessoa_N, lugares, cheio);
    pthread_mutex_unlock(&trava);
}

//Solução q n eh justa, pois ordem de chegada n eh mantida.
void *Sushi(void *thread)
{
    int pessoa_N = (*(int *)thread) + 1;
    int wait_sec = rand() % 5 + 1; //tempo q a pessoa ficara no bar
    pthread_mutex_lock(&trava);
    if (cheio) //se o bar estiver cheio a pessoa vai para fila
FILA:   espera_fila(wait_sec, pessoa_N);
    if (!entrar_SushiBar(pessoa_N))
        goto FILA;
    sleep(wait_sec); //pessoa esta dentro do restaurante comendo
    sair_SushiBar(pessoa_N);
    return NULL;
}

int main(int argc, char *argv[])
{
    int n_Pessoas = atoi(argv[2]);
    if (n_Pessoas <= 0)
        n_Pessoas = 10;
    n_assentos = atoi(argv[1]);
    if (n_assentos <= 0)
        n_assentos = 5;
    lugares = n_assentos;

    pthread_t thread[n_Pessoas];
    int ids[n_Pessoas];
    srand(time(NULL));

    if (pthread_mutex_init(&trava, NULL) != 0)
        return 1;

    for (int i = 0; i < n_Pessoas; i++)
    {
        ids[i] = i;
        pthread_create(&thread[i], NULL, Sushi, &ids[i]);
    }

    for (int i = 0; i < n_Pessoas; i++)
        pthread_join(thread[i], NULL);

    pthread_mutex_destroy(&trava);
    return 0;
}