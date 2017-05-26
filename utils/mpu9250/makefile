# The c compiler
CC=gcc

# Compiler flags
CFLAGS=-Wall -std=c11

LIBS=-lwiringPi -lrt

mpu9250.o: mpu9250.c
	$(CC) -c -o mpu9250.o mpu9250.c $(CFLAGS) $(LIBS)

build: mpu9250.o
	$(CC) -o mpu9250 mpu9250.o $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f mpu9250.o mpu9250