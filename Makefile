CC		 = gcc
EXEC	 = main
CFLAGS = -g -Wall 
OBJ 	 = main.o linkedlist.o functions.o teller.o customer.o


$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -pthread 

main.o : main.c linkedlist.h
	$(CC) $(CFLAGS) main.c -c

linkedlist.o : linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) linkedlist.c -c

functions.o : functions.c functions.h
	$(CC) $(CFLAGS) functions.c -c


teller.o : teller.c teller.h
	$(CC) $(CFLAGS) teller.c -c

customer.o : customer.c customer.h
	$(CC) $(CFLAGS) customer.c -c

clean:
	rm -f $(EXEC) $(OBJ)


