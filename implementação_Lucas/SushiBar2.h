
#ifndef SUSHIBAR_H_
#define SUSHIBAR_H_

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

typedef struct SushiBar SushiBar;

SushiBar *SushiBar_init(int);
void SushiBar_destroy(SushiBar *);
void SushiBar_enter(SushiBar *, int);
void SushiBar_leave(SushiBar *, int);
void *Sushi(void * argumentos);

#endif