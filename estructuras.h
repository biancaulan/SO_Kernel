#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H


typedef struct process_info 
{
    int pid; 
    int status;
    int tiempo_restante; //tiempo restante en segundos
    int tiempo_ejecutando;

} process_info_t;

typedef struct process_node 
{
    struct process_node * next;
    process_info_t me;

} process_node_t;

typedef struct process_queue  
{
    process_node_t* first;
    process_node_t* last;

} process_queue_t;

typedef struct hilo_s
{
    process_info_t proceso;// tiene el proceso que el hilo tiene

} hilo_t;

typedef struct core
{
    int core_id;
    hilo_t* hilos;

} core_t;

typedef struct cpu
{
    int cpu_id;
    core_t* core_array;
} cpu_t;

typedef struct machine
{
    cpu_t* cpu_array;

} machine_t;
#endif //ESTRUCTURAS_H