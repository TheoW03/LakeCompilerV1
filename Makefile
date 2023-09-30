output:main.o Lexxer.o parse.o UtilFunctions.o MipsTarget.o ARMTarget.o optimizations.o builtInFunctions.o ExpressionTree.o VaraibleScope.o
		g++ Lexxer.o UtilFunctions.o ARMTarget.o main.o parse.o MipsTarget.o optimizations.o builtInFunctions.o ExpressionTree.o VaraibleScope.o -o output

Lexxer.o: src/CompilerFrontend/Lexxer.cpp src/CompilerFrontend/parser.h src/CompilerFrontend/Lexxer.h  
		g++ -c -g src/CompilerFrontend/Lexxer.cpp

parse.o: src/CompilerFrontend/parse.cpp src/CompilerFrontend/parser.h src/CompilerFrontend/Lexxer.h
		g++ -c -g src/CompilerFrontend/parse.cpp

UtilFunctions.o: src/MipsTarget/UtilFunctions.cpp src/MipsTarget/UtilFunctions.h src/CompilerFrontend/Lexxer.h  src/CompilerFrontend/parser.h
		g++ -c -g src/MipsTarget/UtilFunctions.cpp

MipsTarget.o: src/MipsTarget/MipsTarget.cpp src/MipsTarget/MipsTarget.h src/MipsTarget/UtilFunctions.h src/CompilerFrontend/parser.h src/MipsTarget/builtInFunction.h src/CompilerFrontend/Lexxer.h  src/CompilerFrontend/optimizations.h 
		g++ -c -g src/MipsTarget/MipsTarget.cpp

main.o: main.cpp src/CompilerFrontend/parser.h src/CompilerFrontend/Lexxer.h src/MipsTarget/MipsTarget.h src/ARMTarget/ARMTarget.h
		
		g++ -c -g main.cpp
ARMTarget.o: src/ARMTarget/ARMTarget.cpp src/ARMTarget/ARMTarget.h src/MipsTarget/UtilFunctions.h src/CompilerFrontend/parser.h src/CompilerFrontend/Lexxer.h src/CompilerFrontend/optimizations.h  
		
		g++ -c -g src/ARMTarget/ARMTarget.cpp
optimizations.o: src/CompilerFrontend/optimizations.cpp src/CompilerFrontend/parser.h src/CompilerFrontend/Lexxer.h src/MipsTarget/UtilFunctions.h src/CompilerFrontend/optimizations.h src/MipsTarget/MipsTarget.h  
		
		g++ -c -g src/CompilerFrontend/optimizations.cpp
builtInFunctions.o: src/MipsTarget/builtInFunctions.cpp src/MipsTarget/builtInFunction.h src/CompilerFrontend/parser.h src/MipsTarget/UtilFunctions.h src/CompilerFrontend/Lexxer.h src/MipsTarget/MipsTarget.h src/CompilerFrontend/optimizations.h 
		
		g++ -c -g src/MipsTarget/builtInFunctions.cpp
VaraibleScope.o: src/MipsTarget/VaraibleScope.cpp src/MipsTarget/VaraibleScope.h src/MipsTarget/builtInFunction.h src/CompilerFrontend/parser.h src/MipsTarget/UtilFunctions.h src/CompilerFrontend/Lexxer.h src/MipsTarget/MipsTarget.h src/CompilerFrontend/optimizations.h 
		
		g++ -c -g src/MipsTarget/VaraibleScope.cpp
ExpressionTree.o: src/MipsTarget/ExpressionTree.cpp  src/MipsTarget/ExpressionTree.h src/MipsTarget/VaraibleScope.h src/MipsTarget/builtInFunction.h src/CompilerFrontend/parser.h src/MipsTarget/UtilFunctions.h src/CompilerFrontend/Lexxer.h src/MipsTarget/MipsTarget.h src/CompilerFrontend/optimizations.h 
	
	g++ -c -g src/MipsTarget/ExpressionTree.cpp
line_count:
	./linecount.sh
x86test:

	 nasm -f elf64 MipsTarget/MipsTargetASM/testx86.s && gcc -no-pie  MipsTarget/MipsTargetASM/testx86.o && ./a.out
clean: 
	rm *.o output
	clear
	