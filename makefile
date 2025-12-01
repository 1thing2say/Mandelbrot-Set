CC = g++

CFLAGS = -c -g -Wall -std=c++17 -fpermissive

EXENAME = hi

default: main.o ComplexPlane.o
	$(CC) main.o ComplexPlane.o -o $(EXENAME) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

main.o: main.cpp ComplexPlane.h
	$(CC) $(CFLAGS) main.cpp

ComplexPlane.o: ComplexPlane.cpp ComplexPlane.h
	$(CC) $(CFLAGS) ComplexPlane.cpp

clean:
	rm *.o $(EXENAME)

run:
	./$(EXENAME)
