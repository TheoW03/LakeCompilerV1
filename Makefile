output:main.o Lexxer.o parse.o MipsTarget.o x86Target.o optimizations.o
	g++ Lexxer.o x86Target.o main.o parse.o MipsTarget.o optimizations.o -o output
Lexxer.o: compilerFrontend/Lexxer.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h 
	g++ -c -g compilerFrontend/Lexxer.cpp

parse.o: compilerFrontend/parse.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h 
	g++ -c -g compilerFrontend/parse.cpp

MipsTarget.o: MipsTarget/MipsTarget.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h compilerFrontend/optimizations.h 
	g++ -c -g MipsTarget/MipsTarget.cpp

main.o: main.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h x86TargetFiles/x86Target.h 
	g++ -c -g main.cpp
x86Target.o: x86TargetFiles/x86Target.cpp  x86TargetFiles/x86Target.h compilerFrontend/parser.h compilerFrontend/Lexxer.h compilerFrontend/optimizations.h  
		g++ -c -g x86TargetFiles/x86Target.cpp

optimizations.o: compilerFrontend/optimizations.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h compilerFrontend/optimizations.h
		g++ -c -g compilerFrontend/optimizations.cpp
x86test:
	 nasm -f elf64 x86TargetFiles/x86TargetASM/out.s && gcc -no-pie  x86TargetFiles/x86TargetASM/out.o && ./a.out
line_count:
	./linecount.sh
clean: 
	rm *.o output
	clear
	