#include <pthread.h>
#include <stdio.h>

#include "process_generator.h"
#include "timer.h"
#include "estructuras.h"
#include "defines.h"


void* timer_function(void *indice_frec) {
    int cuenta = 0;
    int index = *(int*)indice_frec;

    while (1) {
        if (pthread_mutex_lock(&mutex) != 0) {
            perror("Error al bloquear el mutex");
            return NULL;
        }

        contador++;
        cuenta++;
        if (cuenta == frecuencias[index]) {
            cuenta = 0;

            if (index == 0) {
                if (pthread_mutex_lock(&mutexProcGenerator) != 0) {
                    perror("Error al bloquear el mutexProcGenerator");
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                while (done_countProcGenerator < 1) {
                    if (pthread_cond_wait(&condProcGenerator, &mutexProcGenerator) != 0) {
                        perror("Error al esperar condProcGenerator");
                        pthread_mutex_unlock(&mutexProcGenerator);
                        pthread_mutex_unlock(&mutex);
                        return NULL;
                    }
                }
                done_countProcGenerator = 0;
                if (pthread_cond_broadcast(&cnd_brProcGenerator) != 0) {
                    perror("Error al enviar broadcast cnd_brProcGenerator");
                    pthread_mutex_unlock(&mutexProcGenerator);
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
                if (pthread_mutex_unlock(&mutexProcGenerator) != 0) {
                    perror("Error al desbloquear mutexProcGenerator");
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
            }

            if (pthread_mutex_lock(&mutexScheduler) != 0) {
                perror("Error al bloquear el mutexScheduler");
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            while (done_countScheduler < 1) {
                if (pthread_cond_wait(&condScheduler, &mutexScheduler) != 0) {
                    perror("Error al esperar condScheduler");
                    pthread_mutex_unlock(&mutexScheduler);
                    pthread_mutex_unlock(&mutex);
                    return NULL;
                }
            }
            done_countScheduler = 0;
            if (pthread_cond_broadcast(&br_Scheduler) != 0) {
                perror("Error al enviar broadcast br_Scheduler");
                pthread_mutex_unlock(&mutexScheduler);
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            if (pthread_mutex_unlock(&mutexScheduler) != 0) {
                perror("Error al desbloquear mutexScheduler");
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
        }

        if (pthread_cond_signal(&cond) != 0) {
            perror("Error al enviar signal cond");
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        if (pthread_cond_wait(&broadcast, &mutex) != 0) {
            perror("Error al esperar broadcast");
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        if (pthread_mutex_unlock(&mutex) != 0) {
            perror("Error al desbloquear mutex");
            return NULL;
        }
    }

    // Nunca llega aquí, pero es buena práctica asegurarse de que el mutex se libera
    if (pthread_mutex_unlock(&mutex) != 0) {
        perror("Error al desbloquear mutex");
    }
    return NULL;
}