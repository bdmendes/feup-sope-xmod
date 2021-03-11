CC = cc 
CFLAGS = -Wall -Werror
PROG_NAME = xmod

OBJECTS = main.o parsers.o retrievers.o transverse.o utils.o

xmod: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROG_NAME) $(OBJECTS)

%.o: %.c %.h

.PHONY: clean
clean:
	rm -f $(PROG_NAME) $(OBJECTS)