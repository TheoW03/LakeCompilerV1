output:main.o Lexxer.o parse.o MipsTarget.o x86Target.o
	g++ Lexxer.o x86Target.o main.o parse.o MipsTarget.o -o output
Lexxer.o: compilerFrontend/Lexxer.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h 
	g++ -c -g compilerFrontend/Lexxer.cpp

parse.o: compilerFrontend/parse.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h 
	g++ -c -g compilerFrontend/parse.cpp

MipsTarget.o: MipsTarget/MipsTarget.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h 
	g++ -c -g MipsTarget/MipsTarget.cpp

main.o: main.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h 
	g++ -c -g main.cpp
x86Target.o: x86TargetFiles/x86Target.cpp  x86TargetFiles/x86Target.h compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h 
		g++ -c -g x86TargetFiles/x86Target.cpp
x86test:
	 nasm -f elf64 x86TargetFiles/x86TargetASM/test.s && gcc -no-pie  x86TargetFiles/x86TargetASM/test.o && ./a.out
clean: 
	rm *.o output a.out
	clear