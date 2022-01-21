CFLAGS := -m64 -O2 -g -D_GNU_SOURCE -D_REENTRANT -W -I/usr/local/include
LDFLAGS := -m64 -lm

all: blockmma.c
	$(CC) $(CFLAGS) -Wall -fPIC -c blockmma.c
	$(CC) $(CFLAGS) -shared -Wl,-soname,libblockmma.so.1 -o libblockmma.so.1.0 blockmma.o

install: libblockmma.so.1.0
	cp libblockmma.so.1.0 /usr/lib/libblockmma.so.1
	ln -fs /usr/lib/libblockmma.so.1 /usr/lib/libblockmma.so
	cp blockmma.h  /usr/local/include


clean:
	rm -f *.so *.o *.1.0


.PHONY: all clean
