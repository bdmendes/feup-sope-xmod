CC = gcc 
CFLAGS = -Wall
PROG_NAME = xmod

OBJECTS = main.o parsers.o

xmod: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROG_NAME) $(OBJECTS)

#Object files and their needed sources/headers
#Compiler command is deduced
main.o: main.c
parsers.o: parsers.c

#Clean command
clean:
	rm -f $(PROG_NAME) $(OBJECTS)