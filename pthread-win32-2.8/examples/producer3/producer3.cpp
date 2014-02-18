#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#define sleep(n) Sleep(1000 * (n))
#else
#include <unistd.h>
#endif

#pragma comment(lib, "pthreadVC2.lib")

#define NUM_PRODUCER 2
#define NUM_CONSUMER 1

typedef struct Thread_data_
{
    unsigned int maxgood;
    std::queue<int> q;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
}Thread_data;

Thread_data buffer;

void *producer(void *arg)
{
    bool loop=true;
    long my_id = (long)arg;

    printf("Run producer %d\n",my_id);

    while(loop){
        pthread_mutex_lock(&buffer.mutex);
        if (buffer.q.size() < buffer.maxgood){
            buffer.q.push(my_id);
            printf("%2d goods left, producer=%d\n",buffer.q.size(), my_id);
        }

        if (buffer.q.size() == 10)
            pthread_cond_signal(&buffer.cond);

        pthread_mutex_unlock(&buffer.mutex);
        sleep(1);
    }
    pthread_exit(NULL);
    return NULL;
}

void *consumer(void *arg)
{
    bool loop=true;
    long my_id = (long)arg;

    printf("Run consumer %d\n",my_id);

    while(loop){
        pthread_mutex_lock(&buffer.mutex);
        //unsigned int num = rand() % 10;
        unsigned int num = 10;
        pthread_cond_wait(&buffer.cond, &buffer.mutex);

        for (unsigned int i=0; i<num; i++)
        {
            int k = buffer.q.front();
            buffer.q.pop();
            printf("%d",k);
        }
        printf(" consumer=%d \n", my_id);

        pthread_mutex_unlock(&buffer.mutex);
        sleep(1);
    }
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[])
{
    srand(0);

    buffer.maxgood = 20;

    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init (&buffer.cond, NULL);

    pthread_t thdProducer[NUM_PRODUCER];
    for (int i=0; i<NUM_PRODUCER; i++)
        pthread_create(&thdProducer[i], NULL, producer, (void *)(i+1));

    pthread_t thdConsumer[1];
    for (int i=0; i<NUM_CONSUMER; i++)
        pthread_create(&thdConsumer[i], NULL, consumer, (void *)(i+1));

    void *status=NULL;
    for (int i=0; i<NUM_PRODUCER; i++)
        pthread_join(thdProducer[i], &status);

    for (int i=0; i<NUM_CONSUMER; i++)
        pthread_join(thdConsumer[i], &status);

    printf("There are %d goods.\n", buffer.q.size());

    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.cond);
    pthread_exit(NULL);
}