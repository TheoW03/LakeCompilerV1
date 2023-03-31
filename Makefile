output:main.o Lexxer.o parse.o generation.o
	g++ Lexxer.o main.o parse.o generation.o -o output

Lexxer.o: Lexxer.cpp parser.h Lexxer.h transpile.h
	g++ -c -g Lexxer.cpp

parse.o: parse.cpp parser.h Lexxer.h transpile.h
	g++ -c -g parse.cpp

generation.o: generation.cpp parser.h Lexxer.h transpile.h
	g++ -c -g generation.cpp

main.o: main.cpp parser.h Lexxer.h transpile.h
	g++ -c -g main.cpp
clean: 
	rm *.o output
	clear