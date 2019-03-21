all: example1 example2

example1: example1.cpp parengine
	g++ example1.cpp parengine.o -lpthread -o example1

example2:
	g++ example2.cpp parengine.o -lpthread -o example2

parengine: parengine.hpp parengine.cpp
	g++ -c parengine.cpp -o parengine.o

clean:
	rm -rf example1 example2 parengine.o
