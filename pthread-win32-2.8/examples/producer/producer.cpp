#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#define sleep(n) Sleep(1000 * (n))
#else
#include <unistd.h>
#endif

#pragma comment(lib, "pthreadVC2.lib")

#define NUM_PRODUCER 3

typedef struct Thread_data_
{
    int goods;
    int maxgood;
}Thread_data;

pthread_mutex_t mutexgoods;
Thread_data buffer={0};

void *producer(void *arg)
{
    bool loop=true;
    long my_id = (long)arg;

    printf("Run thread %d\n",my_id);

    while(loop){
        pthread_mutex_lock(&mutexgoods);
        if (buffer.goods < buffer.maxgood){
            buffer.goods += 1;
            printf("The current goods is %2d, thread=%d\n",buffer.goods, my_id);
        }
        else
            loop = false;
        pthread_mutex_unlock(&mutexgoods);
        sleep(1);
    }
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[])
{
    buffer.maxgood = 20;

    pthread_mutex_init(&mutexgoods, NULL);

    pthread_t thdProducer[NUM_PRODUCER];
    for (int i=0; i<NUM_PRODUCER; i++)
        pthread_create(&thdProducer[i], NULL, producer, (void *)(i+1));

    void *status=NULL;
    for (int i=0; i<NUM_PRODUCER; i++)
        pthread_join(thdProducer[i], &status);

    printf("There are %d goods.\n", buffer.goods);

    pthread_mutex_destroy(&mutexgoods);
    pthread_exit(NULL);
}