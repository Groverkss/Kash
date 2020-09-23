IDIR=include
SDIR=src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=$(SDIR)/obj
LDIR =lib

LIBS=-lm -lncurses

_DEPS = util.h libs.h builtin.h char_vector.h \
		ls.h signal_handlers.h nightswatch.h \
		history.h execute.h proclist.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = shell.o prompt.o parse.o utils.o \
	   execute.o char_vector.o ls.o \
	   signal_handlers.o nightswatch.o \
	   history.o builtin.o proclist.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
