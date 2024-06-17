#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "process_generator.h"
#include "estructuras.h"
#include "defines.h"
#include "clock.h"
#include "timer.h"
#include "scheduler.h"

//Variables globales
int num_cpus;
int num_cores;
int num_procesos;
int num_timers;
pthread_t* id;
int contador;
pthread_mutex_t mutex; 
pthread_cond_t cond; 
pthread_cond_t broadcast;

int done_countProcGenerator;
pthread_mutex_t mutexProcGenerator; 
pthread_cond_t condProcGenerator; 
pthread_cond_t cnd_brProcGenerator;

int done_countScheduler;
pthread_mutex_t mutexScheduler; 
pthread_cond_t condScheduler; 
pthread_cond_t br_Scheduler;

machine_t machine;
process_queue_t lista_procesos;

// Función para imprimir un banner del kernel
void print_kernel_banner() {
    printf("============================================================\n");
    printf("=                 Simulated Kernel                         =\n");
    printf("=                Initializing system...                    =\n");
    printf("============================================================\n\n");
}

// Función para imprimir el estado del sistema
void print_system_status(const char* message) {
    printf("[KERNEL] %s\n", message);
}


int inicializando_kernel() {

    hilo_t hilo_vacio;
    hilo_vacio.proceso.pid = -1;
    hilo_vacio.proceso.status = -1;

    machine.cpu_array = (cpu_t*)malloc(num_cpus * sizeof(cpu_t)); // Reservar espacio para las CPUs
    if (machine.cpu_array == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_cpus; i++) { // Recorrer el array de CPUs
        machine.cpu_array[i].cpu_id = i;
        machine.cpu_array[i].core_array = (core_t*)malloc(num_cores * sizeof(core_t)); // Reservar espacio para los cores
        if (machine.cpu_array[i].core_array == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < num_cores; j++) { // Recorrer los cores dentro de cada CPU
            machine.cpu_array[i].core_array[j].hilos = (hilo_t*)malloc(num_procesos * sizeof(hilo_t));
            if (machine.cpu_array[i].core_array[j].hilos == NULL) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            for (int k = 0; k < num_procesos; k++) {
                machine.cpu_array[i].core_array[j].hilos[k] = hilo_vacio;
            }
        }
    }
    return 1;
}

int eliminar_hilos(){
    for (int i = 0; i < 3+num_timers; i++){
        pthread_join(id[i],NULL);
    }
}

int main(int argc, char *argv[]) {
    // Imprimir banner del kernel
    print_kernel_banner();

    if (argc!=6) {
        printf("Error en argumentos, formato es: \n num_CPU, num_cores, num_hilos, frequency_processGenerator, frequency_scheduler\n");
        exit(-1);
    }
    num_cpus = atoi(argv[1]);
    num_cores = atoi(argv[2]);
    num_procesos = atoi(argv[3]);
    num_timers = 2;

    frecuencias = (int*)malloc((num_timers)*sizeof(int));
    for (int j = 0; j < num_timers; j++)
    {
        frecuencias[j] = atoi(argv[4+j]);
    }
    print_system_status("Inicializando timers...");
    if (frecuencias == NULL) {
        perror("malloc frecuencias");
        exit(EXIT_FAILURE);
    }

    print_system_status("Clock creado con éxito.");

    if (!inicializando_kernel()) {
        fprintf(stderr, "Error inicializando hardware\n");
        free(frecuencias);
        exit(EXIT_FAILURE);
    }
    print_system_status("Inicializando control entre los procesos...");

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_cond_init(&broadcast,NULL);
    pthread_mutex_init(&mutexProcGenerator,NULL);
    pthread_cond_init(&condProcGenerator,NULL);
    pthread_cond_init(&cnd_brProcGenerator,NULL);
    pthread_mutex_init(&mutexScheduler,NULL);
    pthread_cond_init(&condScheduler,NULL);
    pthread_cond_init(&br_Scheduler,NULL);

    id = (pthread_t*)malloc((4+num_timers)*sizeof(pthread_t));

    if (id == NULL) {
        perror("malloc id");
        exit(EXIT_FAILURE);
    }

    int aux=0;
    pthread_create(&id[0], NULL, clock_function, &aux);
    pthread_create(&id[1],NULL,scheduler,&aux);
    pthread_create(&id[2],NULL, process_generator,&aux);
    pthread_create(&id[3], NULL, monitor_function, &aux);

    for (int i = 0; i < num_timers; i++){
        int *arg = malloc(sizeof(*arg));  //Aquí se guarda espacio para el indice
        if (arg == NULL) {
            perror("malloc arg");
            free(id);
            exit(EXIT_FAILURE);
        }

        *arg = i;
        if (pthread_create(&id[3 + i], NULL, timer_function, arg) != 0) {
            perror("pthread_create timer");
            free(arg);
            free(id);
            exit(EXIT_FAILURE);
        }
    }


    eliminar_hilos();

    free(frecuencias); // Liberar frecuencias
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condProcGenerator);
    pthread_cond_destroy(&cnd_brProcGenerator);

    printf("La simulación ha acabado\n");

    return 0;
}
