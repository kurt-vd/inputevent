PROGS	= inputevent timeofday
default	: $(PROGS)

VERSION:= $(shell git describe --always --tags --dirty)
PREFIX	= /usr/local
CFLAGS	= -Wall
LDLIBS	=

-include config.mk

CPPFLAGS+= -DVERSION=\"$(VERSION)\"

.PHONY: clean install

inputevent: LDLIBS+=-lm
inputevent: inputtables.o modtables.o lib/libt.o

clean:
	rm -rf $(PROGS) $(wildcard *.o lib/*.o)

install: $(PROGS)
	@[ -d $(DESTDIR)$(PREFIX)/bin ] || install -v -d $(DESTDIR)$(PREFIX)/bin
	@install -v $(PROGS) $(DESTDIR)$(PREFIX)/bin

