#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "clock.h"
#include "defines.h"
#include "estructuras.h"

//Función que da pulso al process_generator y al timer
void* clock_function(void * argument){
    
    contador = 0;
    while (1){
        if(pthread_mutex_lock(&mutex)!=0){
            perror("Error al bloquear el mutex");
            break;
        }
        while (contador<num_timers){
            if (pthread_cond_wait(&cond, &mutex) != 0) {
                perror("Error al esperar la condición");
                pthread_mutex_unlock(&mutex); // Desbloquear el mutex antes de salir
                return NULL;
            }
        }
        contador=0;
        // Despertar a todos los hilos que esperan en cnd_br
        if (pthread_cond_broadcast(&broadcast) != 0) {
            perror("Error al enviar la señal de broadcast");
            pthread_mutex_unlock(&mutex); // Desbloquear el mutex antes de salir
            return NULL;
        }
        // Desbloquear el mutex
        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("Error al desbloquear el mutex");
            break;
        }
    }
    // Asegurarse de que el mutex se libera al salir del ciclo
    pthread_mutex_unlock(&mutex);
    return NULL;
}