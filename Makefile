CC		 = gcc
EXEC	 = main
CFLAGS = -g -Wall 
OBJ 	 = main.o linkedlist.o functions.o


$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -pthread 

main.o : main.c linkedlist.h
	$(CC) $(CFLAGS) main.c -c

linkedlist.o : linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) linkedlist.c -c

functions.o : functions.c functions.h
	$(CC) $(CFLAGS) functions.c -c



clean:
	rm -f $(EXEC) $(OBJ)


