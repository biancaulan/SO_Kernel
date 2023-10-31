#include <pthread.h>

 //Estructuras de los unidades necesarias
struct PCB {
    pthread_t pid; //identificador de un hilo
    
};

//Estructura que contiene el conjunto de los procesos
struct Process_queue{
    struct PCB conjunto_PCB[100];
};


//Estructura que representa las CPUS, cores e hilos
struct Machine{
   int hilos;
   int cores;
   int CPUS;
};

enum Estados {
    READY,
    EXECUTING,
    BLOCKED,
    FINISHED
};