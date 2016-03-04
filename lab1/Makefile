CFLAGS = -Wall -std=c99 -g

Mergesort: Mergesort.o
	gcc $(CFLAGS) -o Mergesort Mergesort.o 

Mergesort.o: Mergesort.c Mergesort.h
	gcc $(CFLAGS) -c Mergesort.c

clean : 
	rm -f *.o Mergesort core
