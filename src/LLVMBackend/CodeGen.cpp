#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>

using namespace std;
using namespace llvm;
llvm::Value *Node::Codegen(IRBuilder<> &builder)
{
}

llvm::Value *IntegerNode::Codegen(IRBuilder<> &builder)
{
    return builder.getInt32(stoi(num));
}
llvm::Value *OperatorNode::Codegen(IRBuilder<> &builder)
{
}