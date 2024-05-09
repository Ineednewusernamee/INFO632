#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10 
#define MAX_MESSAGES 10 

typedef struct {
    int id;
    char *message;
} MESS;

MESS T[N]; // Tampon
int id = 0; 
int ir = 0; 

sem_t sprod, scons; 

void Produire(MESS *message) {
    message->id = id++;
    char buffer[20];
    sprintf(buffer, "Message %d", message->id);
    message->message = strdup(buffer);
    printf("Producteur : %s\n", message->message);
}

void Consommer(MESS *message) {
    char *ptr = message->message;
    while (*ptr) {
        *ptr = toupper(*ptr);
        ptr++;
    }
    printf("Consommateur : %s\n", message->message);
    free(message->message);
}

void Deposer(MESS *message) {
    T[id % N] = *message;
}

void Retirer(MESS *message) {
    *message = T[ir % N];
}

void *Production(void *arg) {
    int count = 0; 
    while (count < MAX_MESSAGES) { 
        MESS message;
        Produire(&message);
        sem_wait(&sprod);
        Deposer(&message);
        sem_post(&scons);
        count++; 
    }
    return NULL;
}

void *Consommation(void *arg) {
	int count = 0;
    while (count < MAX_MESSAGES) {
        MESS message;
        sem_wait(&scons);
        Retirer(&message);
        sem_post(&sprod);
        Consommer(&message);
        count++;
    }
    return NULL;
}

int main() {
    sem_init(&sprod, 0, N); 
    sem_init(&scons, 0, 0); 

    pthread_t prod, cons;
    pthread_create(&prod, NULL, Production, NULL);
    pthread_create(&cons, NULL, Consommation, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
