# Lacus

a strongly typed, procedual, compiled programming language. 
right now MIPS 32 Assembly and LLVM

syntax was strongly inspired by rust, Ada and C

why i choose the name,
lacus is latin for lake, and I chose lake to honor my favorite enby-coded character in Animation. I cant name it lake
because lean4 has a build system with the name of lake. 


# Langauge definition


```C++
//% -> mod
//* -> multiply
// / -> divison
// +,- -> addition and subtraction

// <, > greater than less than
// >= <=, GTE, LTE
//  /= not equal
fn main(args: string) returns int { //this is how you call the main method (this is how you do comments), returns for return
    const float pi = 3.1415; // floats
    int random_var = 100; //integer
    bool turringComplete = true; //i think lol 
    random_var = (int)(pi + random_var * ((pi + random_var) % 2)/10); 
    //if statemnets
    if(random_var == 100){
        print("%d", random_var);
    }else if(random_var == 200){
        random_var = random_var + 1;
        print("%d", random_var);
    }else{
        random_var = 0;
        print("%d", random_var);
    }
    //loops 
    while random_var < 1000 do random_var = random_var + 1;
    for(int i = 0; i < 10; step i, 1) ->  print("%d",i); 
    for(int i = 10; i > 0; step i, -1) ->  print("%d",i); //step is "i++" in C. (step VAR, NUM)
    print("%f",random_var); // %f -> float %d -> integer. this is a temp system until I add something better :3
    exit(1); //exit
    return 0;
    
}
fn f(x: int) : int -> x + 1; //new feature i been thinking of. for a while >:3
```


# CLI commands

1st arg is the file name

-d : runs a demo of this dir, the file is test.txt and the out file is MIPSTarget/MipsTargetASM/out.s

-token : outputs the token list

-o ``file``:  output file if none specified its going to be MipsTarget/MipsTargetASM/out.s

-m: specify MIPS 32 target

-l: specify LLVM target (coming soon)

-v : version (coming soon)

# Build Instructions

if you have linux as your primary OS

use the ``./setUpCMake.sh`` script to setup the CMake code. 

and from then on 

``./build.sh`` to build your code. the exe name should be called "output" 

``./clean.sh`` to clean all the objs and exe.   


else just create a build dir and do 

``cmake ..``


# Run Compilation

<b>MIPS</b>

if you dont have a MIPS CPU on hand just do this

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


<b>LLVM</b>

WARNING: this is in the very early stages of development so there is a huge version discrepancy between MIPS32 target and the LLVM target

LLVM IR version is 14.0.0 (the best OS to run this on is Ubuntu linux)

and the generated LLVM IR files are located in out/out.ll

how to run LLVM IR code

```Sh
llc out/out.ll
clang out/out.s
./a.out

```


# Test Programs

Try some test programs. using the MIPS32 target

<a href=https://github.com/TheoW03/Lake/tree/master/TestPrograms>Test programs</a>

