#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "defines.h"
#include "estructuras.h"
#include "process_generator.h"

void* process_generator(void* args){
   
    pthread_mutex_lock(&mutexProcGenerator);
    int ultimoid = 0;
    while (1) {
        done_countProcGenerator++;
        
        // Crear nuevo proceso
        process_node_t* nuevo_process_node = (process_node_t*)malloc(sizeof(process_node_t));  
        process_info_t nuevo_process_info;
        nuevo_process_info.pid = ultimoid;
        nuevo_process_info.status = 0; // Estatus inicial del proceso
        //inicializo el tiempo de ejecución aleatorio entre 2 y 8 segundos
        nuevo_process_info.tiempo_ejecutando = 0;
        // Inicializa el tiempo restante
        nuevo_process_info.tiempo_restante = (rand() % 7) + 2;
        // nuevo_process_info.tiempo_restante = 10; // Comprobación
        
        ultimoid++;
        // Otras inicializaciones del proceso

        nuevo_process_node->me = nuevo_process_info;
        nuevo_process_node->next = NULL;

        // Meterlo en la cola
        if (lista_procesos.first == NULL) {  // La lista está vacía
            lista_procesos.first = nuevo_process_node;
            lista_procesos.last = nuevo_process_node;
        } else {                             // Se actualiza el último
            lista_procesos.last->next = nuevo_process_node;
            lista_procesos.last = nuevo_process_node;
        }

        // Mostrar el PID del proceso principal junto con el PID del nuevo proceso
        printf("\x1b[32mCreado el proceso %d\n\x1b[0m", lista_procesos.last->me.pid);

        pthread_cond_signal(&condProcGenerator);
        pthread_cond_wait(&cnd_brProcGenerator, &mutexProcGenerator);
    }
}
