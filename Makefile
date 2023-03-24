output:main.o tools.o
	g++ tools.o main.o -o output
tools.o: tools.cpp tools.h
	g++ -c tools.cpp
main.o: main.cpp tools.h
	g++ -c main.cpp
clean: 
	rm *.o output