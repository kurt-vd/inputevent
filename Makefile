PROGS	= inputevent
default	: $(PROGS)

VERSION:= $(shell git describe --always --tags --dirty)
PREFIX	= /usr/local
CFLAGS	= -g3 -O0 -Wall

-include config.mk

CPPFLAGS+= -DVERSION=\"$(VERSION)\"

.PHONY: clean install

inputevent: inputtables.o

clean:
	rm -rf $(PROGS) *.o

install: $(PROGS)
	@[ -d $(DESTDIR)$(PREFIX)/bin ] || install -v -d $(DESTDIR)$(PREFIX)/bin
	@install -v $(PROGS) $(DESTDIR)$(PREFIX)/bin

