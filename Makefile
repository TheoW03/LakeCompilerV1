output:main.o tools.o parse.o generation.o
	g++ tools.o main.o parse.o generation.o -o output

tools.o: tools.cpp parser.h tools.h transpile.h
	g++ -c -g tools.cpp

parse.o: parse.cpp parser.h tools.h transpile.h
	g++ -c -g parse.cpp

generation.o: generation.cpp parser.h tools.h transpile.h
	g++ -c -g generation.cpp

main.o: main.cpp parser.h tools.h transpile.h
	g++ -c -g main.cpp
clean: 
	rm *.o output
	clear