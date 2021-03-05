CC = gcc 
CFLAGS = -Wall
PROG_NAME = xmod

OBJECTS = main.o parsers.o

xmod: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROG_NAME) $(OBJECTS)

#Object files and their needed sources/headers
#Compiler command is deduced
main.o: main.c xmod.h
parsers.o: parsers.c xmod.h

#Clean command
clean:
	rm -f $(PROG_NAME) $(OBJECTS)