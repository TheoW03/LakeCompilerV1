# compiler

how it works

# usage 

for a MIPS target to compile

and use the return value from 
``parse(Vector<Tokens> tokens)``

to gen ASM

``void gen_mips_target(Node *op, string filename = "")``

----

for x86 


use the return value from the  

``testExpressionParse(Vector<Tokens> tokens) ``

and then to generate run

``void gen_x86_target(Node *op, string filename = "")``

(if no file is give for MIPS the dir)

is MipsTarget/MipsTargetASM/out.s

x86 its 

(x86 can only handle expressions)

x86TargetFiles/x86TargetASM/out.s

---

once files generated, 

for x86 you can run 

``make x86test``


for mips

just pop the generated file into qtSpim

