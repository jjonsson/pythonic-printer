CC	=	g++
CFLAGS	=	-std=c++0x -O3 -I src
OBJECTS	=	build/pptest.o


all:	test
test:	$(OBJECTS)
	$(CC) -o build/pptest $(OBJECTS)

build/pptest.o: src/pythonic_print.h src/pythonic_print-private.h test/pptest.cpp
	$(CC) $(CFLAGS) -o build/pptest.o -c test/pptest.cpp

clean:	
	rm -f build/pptest $(OBJECTS)