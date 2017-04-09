CC = gcc
CFLAGS = -ansi -pedantic -Wall -g -ggdb `pkg-config --cflags --libs gtk+-2.0`
OBJ = calendar.o gui.o linkedList.o calText.o

calendar : $(OBJ)
	$(CC) $(CFLAGS) -o calendar $(OBJ)

calendar.o : calendar.c calendar.h gui.h linkedList.h calText.h
	$(CC) $(CFLAGS) -c calendar.c

gui.o : gui.c gui.h
	$(CC) $(CFLAGS) -c gui.c

linkedList.o : linkedList.c linkedList.h
	$(CC) $(CFLAGS) -c linkedList.c

calText.o : calText.c calText.h
	$(CC) $(CFLAGS) -c calText.c

clean :
	rm -f calendar $(OBJ)
