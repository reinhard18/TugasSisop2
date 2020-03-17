#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int max, loops, consumers;
int buffer[4];
int fillIndex, useIndex, count = 0;
pthread_mutex_t mutex;
pthread_cond_t empty, full;

void *producer(void *arg);
void *consumer(void *arg);

int main(int argc, char *argv[])
{
    max = 4;
    loops = 4;
    pthread_t prods, consumers;
    pthread_create(&prods, NULL, producer, NULL);
    pthread_create(&consumers, NULL, consumers, NULL);
    pthread_join(prods, NULL);
    pthread_join(consumers, NULL);
    return 0;
}

void put(int value)
{
    buffer[fillIndex] = value;
    fillIndex = (fillIndex + 1) % max;
    count++;
}
int get()
{
    int tmp = buffer[useIndex];
    useIndex = (useIndex + 1) % max;
    count--;
    return tmp;
}

void *producer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&mutex);
        while (count == max)
            pthread_cond_wait(&empty, &mutex);
        put(i);
        //count = 0;
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
        //printf(%d, count);
        printf("%s", "Producer mengisi buffer ke= ");
        printf("%d\n", i);
    }
}

void *consumer(void *arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
        pthread_mutex_lock(&mutex);
        while (count == 0)
            pthread_cond_wait(&full, &mutex);
        int tmp = get();
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        printf("%s", "Consumer mengkonsumsi buffer ke =");
        printf("%d\n", tmp);
    }
}