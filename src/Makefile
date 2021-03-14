CC = cc 
CFLAGS = -Wall -Werror
PROG_NAME = xmod

OBJECTS = main.o parse/parsers.o retrieve/retrievers.o util/utils.o log/log.o signal/sig.o

xmod: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROG_NAME) $(OBJECTS)

%.o: %.c %.h

.PHONY: clean
clean:
	rm -f $(PROG_NAME) $(OBJECTS)