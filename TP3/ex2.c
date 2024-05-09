#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* Global variables */
int x = 0;
sem_t m;

/* Thread function */
void *thread(void *arg)
{
    int i = (int)arg;

    /* critical section */
    sem_wait(&m);  
    x = x + 1;
    sem_post(&m);  

    printf("x: %d, i: %d\n", x, i);
    return NULL;
}

int main ()
{
    pthread_t tid[10];
    int i;

    /* init sem at "1" */
    sem_init(&m, 0, 1);  

    /* create TEN threads */
    for (i = 0; i < 10; i++)
    {
        if (pthread_create(&tid[i], NULL, thread, (void*)i) < 0) {
            perror("Error: thread cannot be created");
            exit(1);
        }
    }

    /* wait for all created thread to terminate */
    for (i = 0; i < 10; i++) pthread_join(tid[i], NULL);

    printf("Final value of x is %d\n", x);

    /* Clean up semaphore */
    sem_destroy(&m);

    exit(0);
}
