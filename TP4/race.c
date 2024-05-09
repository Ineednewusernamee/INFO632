#include "race.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define NB_TOURS 10

Voiture* voitures;
pthread_barrier_t startBarrier;
pthread_mutex_t mutex;

static int current_position = 1;

void* fonctionVoiture(void* arg) {
    Voiture* voiture = (Voiture*) arg;
    pthread_barrier_wait(&startBarrier);

    for (int i = 0; i < NB_TOURS; i++) {
        sleep(rand() % 3 + 1);  
        pthread_mutex_lock(&mutex);
        voiture->nbTours++;
        printf("Voiture %d a complété le tour %d\n", voiture->id, voiture->nbTours);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    voiture->position = current_position++;
    printf("Voiture %d a terminé la course en rang %d\n", voiture->id, voiture->position);
    pthread_mutex_unlock(&mutex);

    return NULL;
}


void setup_race(int nbVoitures) {
    voitures = malloc(nbVoitures * sizeof(Voiture));
    pthread_barrier_init(&startBarrier, NULL, nbVoitures + 1);
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < nbVoitures; i++) {
        voitures[i].id = i + 1;
        voitures[i].nbTours = 0;
    }
}

void start_race(int nbVoitures) {
    pthread_t threads[nbVoitures];
    for (int i = 0; i < nbVoitures; i++) {
        pthread_create(&threads[i], NULL, fonctionVoiture, &voitures[i]);
    }
    pthread_barrier_wait(&startBarrier);
    printf("La course commence !\n");
    for (int i = 0; i < nbVoitures; i++) {
        pthread_join(threads[i], NULL);
    }
}

void cleanup_race() {
    pthread_barrier_destroy(&startBarrier);
    pthread_mutex_destroy(&mutex);
    free(voitures);
}

int compare(const void* a, const void* b) {
    Voiture* voitureA = (Voiture*)a;
    Voiture* voitureB = (Voiture*)b;
    return voitureA->position - voitureB->position;
}


