# Makefile example. Change all "file" to your files.

# Make ./include and ./obj folder.
# Put all header files in ./include. 

INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CLFAGS += -I$(INCLDIR)

OBJDIR = obj

# List all the header files here.
_DEPS = formatConvert.h
DEPS = $(patsubst %,$(INCLDIR)/%, $(_DEPS))

# List all source files here, but give .o file convention.
_OBJS = main.o formatConvert.o
OBJS = $(patsubst %,$(OBJDIR)/%, $(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Output file name.
oppgave7: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
