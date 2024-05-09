#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_MESSAGES 10

struct Queue {
    int front, rear, size;
    unsigned capacity;
    char** array; 
    sem_t mutex; 
};

struct Queue* createQueue(unsigned capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (char**)malloc(queue->capacity * sizeof(char));
    sem_init(&(queue->mutex), 0, 1); 
    return queue;
}

int isFull(struct Queue* queue) {  
    return (queue->size == queue->capacity);  
}

int isEmpty(struct Queue* queue) {  
    return (queue->size == 0); 
}

void Deposer(struct Queue* queue, char *message) {
    sem_wait(&(queue->mutex)); 
    if (isFull(queue)) {
        sem_post(&(queue->mutex)); 
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = message;
    queue->size = queue->size + 1;
    sem_post(&(queue->mutex)); 
}

char* Retirer(struct Queue* queue) {
    sem_wait(&(queue->mutex)); 
    if (isEmpty(queue)) {
        sem_post(&(queue->mutex)); 
        return NULL;
    }
    char *message = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    sem_post(&(queue->mutex)); 
    return message;
}

char* Produire() {
    static int k = 0;
    char* message = (char*)malloc(20 * sizeof(char));
    sprintf(message, "Message %d", k++);
    return message;
}

void Consommer(char *message) {
    printf("Message consommé : %s\n", message);
    for(int i = 0; message[i] != '\0'; i++) {
        if(message[i] >= 'a' && message[i] <= 'z') {
            message[i] = message[i] - 32;
        }
    }
    printf("Message en majuscule : %s\n", message);
}

void* Producteur(void* args) {
    struct Queue* queue = (struct Queue*)args;
    int messages_produits = 0;
    while (messages_produits < MAX_MESSAGES) {
        char* message = Produire(); 
        Deposer(queue, message);
        messages_produits++;
    }
    return NULL;
}

void* Consommateur(void* args) {
    struct Queue* queue = (struct Queue*)args;
    int messages_consommes = 0;
    while (messages_consommes < MAX_MESSAGES) {
        char* message = Retirer(queue);
        if (message != NULL) {
            Consommer(message); 
            free(message);
            messages_consommes++;
        }
    }
    return NULL;
}

int main() {
    struct Queue* queue = createQueue(MAX_MESSAGES);

    pthread_t producerThread, consumerThread;

    pthread_create(&producerThread, NULL, Producteur, (void*)queue);
    pthread_create(&consumerThread, NULL, Consommateur, (void*)queue);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    sem_destroy(&(queue->mutex));
    free(queue->array);
    free(queue);

    return 0;
}
