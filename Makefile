CC	=	g++
CFLAGS	=	-std=c++0x -O3 -I src
OBJECTS	=	build/pptest.o build/pythonic_print.o
TARGET	=	pptest


all:	test
test:	$(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

build/pythonic_print.o: src/pythonic_print.h src/pythonic_print-private.h src/pythonic_print.cpp
	mkdir -p build
	$(CC) $(CFLAGS) -o build/pythonic_print.o -c src/pythonic_print.cpp

build/pptest.o: src/pythonic_print.h src/pythonic_print-private.h test/pptest.cpp
	mkdir -p build
	$(CC) $(CFLAGS) -o build/pptest.o -c test/pptest.cpp

clean:	
	rm -f $(TARGET) $(OBJECTS)