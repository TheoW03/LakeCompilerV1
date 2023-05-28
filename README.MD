# MIPSTrans32

a MIPS 32 target Transpiler. from a pythonic, JS, C like syntax

it also kind of supports x86 if i can figure that out 

coded by TheoW03, in C++ 

# lang definition


```C++

//% -> mod
//* -> multiply
// / -> divison
// +,- -> addition and subtraction
#define tau 6.28318531 //it can lex this but no support for parsing this yet  but this is a macro

function main(string args){ //this is how you call the main method (this is how you do comments)
    float pi = 3.1415; //floats
    int random_constant = 100; //integer
    random_constant = pi + random_constant * ((pi + random_constant) % 2)/10; 
    print(random_constant); //print to console. support for strings are not added yet 
    exit(1); //exit
    
}

```

 # run transpiled code

<b>MIPS target</b>

1. install QTSpim
https://spimsimulator.sourceforge.net/

---

2. the if you didnt specify a file name. its out.s the location is MipsTarget/MipsTargetASM
---

3. locate file in QTSPim by going to file->Reinitialize and load file

---

4. Before running, I recomend pressing clear registers, and then run

----
<br>

<b> ARM Target</b>

coming soon.