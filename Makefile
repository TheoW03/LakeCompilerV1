output:main.o Lexxer.o parse.o UtilFunctions.o MipsTarget.o ARMTarget.o optimizations.o builtInFunctions.o 
	g++ Lexxer.o UtilFunctions.o ARMTarget.o main.o parse.o MipsTarget.o optimizations.o builtInFunctions.o -o output
Lexxer.o: compilerFrontend/Lexxer.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h  
	g++ -c -g compilerFrontend/Lexxer.cpp
parse.o: compilerFrontend/parse.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h
	g++ -c -g compilerFrontend/parse.cpp
UtilFunctions.o: MipsTarget/UtilFunctions.cpp MipsTarget/UtilFunctions.h compilerFrontend/Lexxer.h  compilerFrontend/parser.h
	g++ -c -g MipsTarget/UtilFunctions.cpp
MipsTarget.o: MipsTarget/MipsTarget.cpp MipsTarget/MipsTarget.h MipsTarget/UtilFunctions.h compilerFrontend/parser.h MipsTarget/builtInFunction.h compilerFrontend/Lexxer.h  compilerFrontend/optimizations.h 
	g++ -c -g MipsTarget/MipsTarget.cpp

main.o: main.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h ARMTarget/ARMTarget.h
	g++ -c -g main.cpp
ARMTarget.o: ARMTarget/ARMTarget.cpp ARMTarget/ARMTarget.h MipsTarget/UtilFunctions.h compilerFrontend/parser.h compilerFrontend/Lexxer.h compilerFrontend/optimizations.h  
		g++ -c -g ARMTarget/ARMTarget.cpp
optimizations.o: compilerFrontend/optimizations.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/UtilFunctions.h compilerFrontend/optimizations.h MipsTarget/MipsTarget.h  
		g++ -c -g compilerFrontend/optimizations.cpp
builtInFunctions.o: MipsTarget/builtInFunctions.cpp MipsTarget/builtInFunction.h compilerFrontend/parser.h MipsTarget/UtilFunctions.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h compilerFrontend/optimizations.h 
		g++ -c -g MipsTarget/builtInFunctions.cpp
line_count:
	./linecount.sh
x86test:

	 nasm -f elf64 MipsTarget/MipsTargetASM/testx86.s && gcc -no-pie  MipsTarget/MipsTargetASM/testx86.o && ./a.out
clean: 
	rm *.o output
	clear
	