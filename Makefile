# Take from https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

IDIR=include
SDIR=src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=$(SDIR)/obj
LDIR =lib

LIBS=-lm

_DEPS = util.h libs.h builtin.h char_vector.h \
		ls.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = shell.o prompt.o parse.o utils.o \
	   builtin.o char_vector.o ls.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
