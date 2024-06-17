#include "estructuras.h"

extern int num_cpus; 
extern int num_cores; 
extern int num_procesos; 
extern int num_timers; 
int* frecuencias;
extern int contador;

extern pthread_mutex_t mutex;
extern pthread_cond_t cond; 
extern pthread_cond_t broadcast; 

extern int done_countProcGenerator;
extern pthread_mutex_t mutexProcGenerator;
extern pthread_cond_t condProcGenerator; 
extern pthread_cond_t cnd_brProcGenerator;

extern int done_countScheduler;
extern pthread_mutex_t mutexScheduler;
extern pthread_cond_t condScheduler;
extern pthread_cond_t br_Scheduler;

extern machine_t machine;
extern process_queue_t lista_procesos;
extern pthread_t* id;



