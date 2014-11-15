
#   Unix Makefile for xd

#   For production
CFLAGS = -O
STRIP = strip

#   For debugging
#CFLAGS = -g
#STRIP = @true

all:	xd

xd:	xd.o
	cc $(CFLAGS) xd.o -o xd
	$(STRIP) xd

clean:
	rm -f xd xd.o
