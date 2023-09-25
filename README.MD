# MTLake32

a MIPS 32 target Transpiler. from a pythonic, JS, C like syntax

coded by TheoW03, in C++ 

# lang definition


```C++

//% -> mod
//* -> multiply
// / -> divison
// +,- -> addition and subtraction

fn main(string args) returns int { //this is how you call the main method (this is how you do comments), returns for return
    const float pi = 3.1415; // floats
    int random_var = 100; //integer
    bool turringComplete = true; //i think lol 
    random_var = pi + random_var * ((pi + random_var) % 2)/10; 
    //if statemnets
    if(random_var == 100){
        print("%d", random_var);
    }
    //loops 
    while(random_var < 1000){
        random_var = random_var + 1;
    }
    print("%f",random_var); // %f -> float %d -> integer. this is a temp system until I add something better :3
    exit(1); //exit
    return 0;
    
}
fn f(int x) : int -> x + 1; //new feature i been thinking of. for a while >:3
```

# CLI commands

1st arg is the file name

-d : runs a demo of this dir, the file is test.txt and the out file is MIPSTarget/MipsTargetASM/out.s

-token : outputs the token list

-o ``file``:  output file if none specified its going to be MipsTarget/MipsTargetASM/out.s

-v : version


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


