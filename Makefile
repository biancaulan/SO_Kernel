# Makefile

# Compilador y opciones de compilación
CC = gcc
CFLAGS = -pthread -Wall -Wextra

# Nombre del ejecutable
TARGET = kernel_program

# Lista de archivos objeto
OBJS = clock.o kernel.o process_generator.o timer.o scheduler.o

# Reglas

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clock.o: clock.c clock.h defines.h estructuras.h scheduler.h
	$(CC) $(CFLAGS) -c clock.c

kernel.o: kernel.c defines.h estructuras.h clock.h process_generator.h timer.h scheduler.h
	$(CC) $(CFLAGS) -c kernel.c

process_Generator.o: process_generator.c process_generator.h defines.h estructuras.h
	$(CC) $(CFLAGS) -c process_generator.c

timer.o: timer.c timer.h defines.h estructuras.h
	$(CC) $(CFLAGS) -c timer.c

schedule.o: scheduler.c scheduler.h defines.h

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
