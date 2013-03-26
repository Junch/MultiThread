//
// https://computing.llnl.gov/tutorials/pthreads/
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS     5

#ifdef WIN32
#pragma comment(lib, "pthreadVC2.lib")
#endif

void *PrintHello(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    pthread_exit(NULL);
    return NULL;
}

int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for(t=0; t<NUM_THREADS; t++){
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /*There is a definite problem if main() finishes before the threads it spawned if you don't
    call pthread_exit() explicitly. All of the threads it created will terminate because main()
    is done and no longer exists to support the threads.
    By having main() explicitly call pthread_exit() as the last thing it does, main() will block
    and be kept alive to support the threads it created until they are done.
    */

    /* Last thing that main() should do */
    pthread_exit(NULL);
}