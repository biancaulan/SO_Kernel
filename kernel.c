#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

//crear threads
int pthread_c

int main(int argc, char *argv[]){
    
    int N = 100; //POR AHORA 100  
    clock_t start = clock();

    pthread_mutex_t mutex; //puntero a una estructura tipo pthread_mutex_t
    //crear mutex
    pthread_mutex_init(*mutex, NULL); 
    
    sem_t semaforo; //puntero a un objeto tipo sem_t
    //crear semaforo
    sem_init(*semaforo,0, 1);
    
    //Contiene el conjunto de los procesos
    struct PCB process_queue[N];
}