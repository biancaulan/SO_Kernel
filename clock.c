#include <pthread.h>;
#include <stdio.h>;

pthread_mutex_t mutex;
pthread_cond_t conidicion1;

void clock_kernel(){

    while (1){
        pthread_mutex_lock(&mutex); //cerramos el cerrojo
        while (){
            
        }
    }

}