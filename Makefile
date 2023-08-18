output:main.o Lexxer.o parse.o MipsTarget.o ARMTarget.o optimizations.o builtInFunctions.o
	g++ Lexxer.o ARMTarget.o main.o parse.o MipsTarget.o optimizations.o builtInFunctions.o -o output
Lexxer.o: compilerFrontend/Lexxer.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h  
	g++ -c -g compilerFrontend/Lexxer.cpp

parse.o: compilerFrontend/parse.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h
	g++ -c -g compilerFrontend/parse.cpp

MipsTarget.o: MipsTarget/MipsTarget.cpp compilerFrontend/parser.h MipsTarget/builtInFunction.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h compilerFrontend/optimizations.h 
	g++ -c -g MipsTarget/MipsTarget.cpp

main.o: main.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/MipsTarget.h ARMTarget/ARMTarget.h 
	g++ -c -g main.cpp
ARMTarget.o: ARMTarget/ARMTarget.cpp  ARMTarget/ARMTarget.h compilerFrontend/parser.h compilerFrontend/Lexxer.h compilerFrontend/optimizations.h  
		g++ -c -g ARMTarget/ARMTarget.cpp
optimizations.o: compilerFrontend/optimizations.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h compilerFrontend/optimizations.h MipsTarget/MipsTarget.h
		g++ -c -g compilerFrontend/optimizations.cpp
builtInFunctions.o: MipsTarget/builtInFunctions.cpp compilerFrontend/parser.h compilerFrontend/Lexxer.h MipsTarget/builtInFunction.h compilerFrontend/optimizations.h
		g++ -c -g MipsTarget/builtInFunctions.cpp
line_count:
	./linecount.sh
clean: 
	rm *.o output
	clear
	