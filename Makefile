CC		 = gcc
EXEC	 = main
CFLAGS = -g -Wall 
OBJ 	 = main.o linkedlist.o 


$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

main.o : main.c linkedlist.h
	$(CC) $(CFLAGS) main.c -c

linkedlist.o : linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) linkedlist.c -c

clean:
	rm -f $(EXEC) $(OBJ)


