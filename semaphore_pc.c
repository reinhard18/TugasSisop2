#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define Num_thread 3
#define Buffer_size 10

void producer(void *ptr);
void consumer(void *ptr);

int buffer[Buffer_size];
int in = 0;
int out = 0;

sem_t mutex;
int main(int argc, char *argv[])
{
    pthread_t prod[Num_thread], cons[Num_thread];
    int i;
    char *msg1 = "Thread 1";
    char *msg2 = "Thread 2";
    sem_init(&mutex, 0, 1);
    for (i = 0; i < Num_thread; i++)
    {
        pthread_create(&prod[i], NULL, (void *)&producer, (void *)msg1);
    }
    for (i = 0; i < Num_thread; i++)
    {
        pthread_create(&cons[i], NULL, (void *)&consumer,(void *) msg2);
    }
    for (i = 0; i < Num_thread; i++)
    {
        pthread_join(prod[i], NULL);
    }
    for (i = 0; i < Num_thread; i++)
    {
        pthread_join(cons[i], NULL);
    }
    sem_destroy(&mutex);
    return 0;
}

void producer (void *ptr){
    char *msg = (char *) ptr;
    sem_wait(&mutex);
    int prod = (rand()) %1000;
    buffer[in] = prod;
    printf("\n Producer %d consume %d", in, buffer[in]);
    sem_post(&mutex);
    pthread_exit(0);
}

void consumer (void *ptr){
    char *msg = (char *) ptr;
    sem_wait(&mutex);
    int consumed;
    consumed = buffer[out];
    printf("\n Consumer %d consume %d", in, consumed);
    out = (out +1) & Buffer_size;
    sem_post(&mutex);
    pthread_exit(0);
}