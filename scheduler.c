#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "estructuras.h"
#include "defines.h"
#include "scheduler.h"

#define quantum 5

void* monitor_function(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutexScheduler);
        for (int i = 0; i < num_cpus; i++) {
            for (int j = 0; j < num_cores; j++) {
                for (int k = 0; k < num_procesos; k++) {
                    if (machine.cpu_array[i].core_array[j].hilos[k].proceso.status != -1) {
                        process_info_t* proc = &machine.cpu_array[i].core_array[j].hilos[k].proceso;

                        proc->tiempo_restante--;
                        proc->tiempo_ejecutando++;

                        if (proc->tiempo_restante <= 0) {
                            printf("Proceso %d terminado en hilo %i del core %i de la cpu %i, tenía %d segundos de ejecución\n",
                                   proc->pid, k, j, i, proc->tiempo_ejecutando);
                            proc->status = -1; 
                        } else if (proc->tiempo_ejecutando >= quantum) {
                            printf("Proceso %d TIMEOUT en hilo %i del core %i de la cpu %i. Le quedan %d segundos\n",
                                   proc->pid, k, j, i, proc->tiempo_restante);
                            proc->status = -1; // Liberar el procesador
                            proc->tiempo_ejecutando = 0; // Resetear quantum
                                                      
                            // Crear nodo para volver a meterlo en la cola
                            process_node_t* nuevo_process_node = (process_node_t*)malloc(sizeof(process_node_t));  
                            nuevo_process_node->me = *proc; 
                            nuevo_process_node->next = NULL;

                            // Verificar si la lista está vacía
                            if (lista_procesos.first == NULL) {
                                lista_procesos.first = nuevo_process_node;
                                lista_procesos.last = nuevo_process_node;
                            } else { // si no está vacía
                                // Se actualiza el último
                                lista_procesos.last->next = nuevo_process_node;
                                lista_procesos.last = nuevo_process_node;
}                     
                        }
                    }
                }
            }
        }

        pthread_mutex_unlock(&mutexScheduler);
    }
    return NULL;
}


 void* scheduler(void * argument){
    bool asignado;
    pthread_mutex_lock(&mutexScheduler);
     printf("+----------------------------------------------------------+\n");
    printf("|\033[1;37m                         SCHEDULER                       \033[0m |\n");
    printf("+----------------------------------------------------------+\n");
    while (1)
    {
        done_countScheduler++;
        // CONTROL : printf("Llamado al dispatcher desde el timer\n");
        //Si hay procesos en la cola
        if (lista_procesos.first!=NULL){
            asignado=false;
            //Recorrer los cores e hilos asignando el primer proceso en la cola al primer hueco libre
            for (int i = 0; i < num_cpus && asignado==false; i++){
                for (int j = 0; j < num_cores && asignado==false; j++){
                    for (int k = 0; k < num_procesos && asignado==false; k++){
                        if (machine.cpu_array[i].core_array[j].hilos[k].proceso.status == -1) // Significa que un procesador está libre
                        {
                            hilo_t hilo_nuevo;

                            hilo_nuevo.proceso = lista_procesos.first->me;
                            hilo_nuevo.proceso.status = 1;
                            hilo_nuevo.proceso.tiempo_ejecutando = 0;
                            machine.cpu_array[i].core_array[j].hilos[k] = hilo_nuevo;
                            asignado=true;

                            // Si solo hay un proceso en la lista, el último va a ser null
                            if (lista_procesos.last == lista_procesos.first){
                                lista_procesos.last = NULL;
                            }
                            // En cualquier caso, el rimero debe apuntar al siguiente (si es el único se queda en null, como debería)
                            lista_procesos.first = lista_procesos.first->next;
                            
                            printf("\x1b[34mAsignado proceso %d al hilo %i del core %i de la cpu %i\n \x1b[0m",
                                machine.cpu_array[i].core_array[j].hilos[k].proceso.pid, k, i, j);
                        }    
                    }    
                }    
            }    
        } else {
            printf("\x1b[33mNo hay procesos en espera \n \x1b[0m");
        }
        pthread_cond_signal(&condScheduler);
        pthread_cond_wait(&br_Scheduler,&mutexScheduler);
    }
    pthread_mutex_unlock(&mutexScheduler);
    return NULL;
}
