CC = cc 
CFLAGS = -Wall -Werror
PROG_NAME = xmod

SRCDIR = src

OBJECTS = $(SRCDIR)/main.o $(SRCDIR)/parse/parsers.o $(SRCDIR)/retrieve/retrievers.o $(SRCDIR)/util/utils.o $(SRCDIR)/log/log.o $(SRCDIR)/signal/sig.o

xmod: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROG_NAME) $(OBJECTS)

%.o: %.c %.h

.PHONY: clean
clean:
	rm -f $(PROG_NAME) $(OBJECTS)