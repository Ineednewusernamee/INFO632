#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/* Variables globales */
sem_t sem1, sem2;

/* Fonctions des threads */
void a1() {
    printf("a1 est en cours d'exécution\n");
}

void a2() {
    printf("a2 est en cours d'exécution\n");
}

void b1() {
    printf("b1 est en cours d'exécution\n");
}

void b2() {
    printf("b2 est en cours d'exécution\n");
}

void *processus_un(void *arg) {
    a1();
    sem_post(&sem1);  // Signale que a1 est terminée
    sem_wait(&sem2);  // Attend que b1 soit terminée
    a2();
    return NULL;
}

void *processus_deux(void *arg) {
    b1();
    sem_post(&sem2);  // Signale que b1 est terminée
    sem_wait(&sem1);  // Attend que a1 soit terminée
    b2();
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Initialisation des sémaphores
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    // Création des threads
    if (pthread_create(&thread1, NULL, processus_un, NULL) < 0) {
        perror("Erreur : impossible de créer le thread");
        exit(1);
    }
    if (pthread_create(&thread2, NULL, processus_deux, NULL) < 0) {
        perror("Erreur : impossible de créer le thread");
        exit(1);
    }

    // Attendre que les deux threads se terminent
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destruction des sémaphores
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    exit(0);
}
