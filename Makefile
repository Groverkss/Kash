# Take from https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

IDIR =include
SDIR =src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=$(SDIR)/obj
LDIR =lib

LIBS=-lm

_DEPS = util.h libs.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = shell.o color.o prompt.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
