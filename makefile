CC      = gcc
PROGRAM = escalona
LIB     = lib

all:
	$(CC) -c main.c
	$(CC) -c conflito.c
	$(CC) -c visao.c
	$(CC) *.o -o $(PROGRAM)

run:
	./$(PROGRAM)

clean:
	rm *.o 
	rm escalona