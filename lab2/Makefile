CFLAGES = -Wall -std=c99 -pthread -g

Factory: assemblers.o packers.o factory.o
	gcc $(CFLAGES) -o Factory assemblers.o packers.o factory.o

factory.o: factory.c factory.h
	gcc $(CFLAGES) -c factory.c

assemblers.o: assemblers.c factory.h
	gcc $(CFLAGES) -c assemblers.c

packers.o: packers.c factory.h
	gcc $(CFLAGES) -c packers.c

clean:
	-rm -f *.o Factory core