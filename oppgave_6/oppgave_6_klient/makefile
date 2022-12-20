INCLDIR = ./include
CC = gcc
# -lsockets is included so no need to add it.
CFLAGS = -O2
CLFAGS += -I$(INCLDIR)

OBJDIR = obj

# List all the header files here.
_DEPS = main.h
DEPS = $(patsubst %,$(INCLDIR)/%, $(_DEPS))

# List all source files here, but give .o file convention.
# _OBJS = main.o webclient.o
_OBJS = main.o
OBJS = $(patsubst %,$(OBJDIR)/%, $(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Output file name.
oppgave6_klient: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~