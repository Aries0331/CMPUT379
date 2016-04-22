CFLAGS = -Wall -std=gnu99 -g

Mergesort: lsRalF.o
	gcc $(CFLAGS) -o lsRalF lsRalF.o 

Mergesort.o: lsRalF.c 
	gcc $(CFLAGS) -c lsRalF.c

clean : 
	rm -f *.o lsRalF core
