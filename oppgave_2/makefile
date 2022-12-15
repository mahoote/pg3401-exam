INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CLFAGS += -I$(INCLDIR)

OBJDIR = obj

# List all the header files here.
_DEPS = main.h
DEPS = $(patsubst %,$(INCLDIR)/%, $(_DEPS))

# List all source files here, but give .o file convention.
_OBJS = main.o
OBJS = $(patsubst %,$(OBJDIR)/%, $(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Output file name.
oppgave2: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
