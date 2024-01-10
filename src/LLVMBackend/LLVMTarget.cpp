#include <iostream>
#include <string>
#include <vector>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <memory>
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/MipsTarget/UtilFunctions.h"
#include "llvm/Support/raw_ostream.h"

#include "../../src/CompilerFrontend/Lexxer.h"
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;
using namespace llvm;
Value *traverse_tree(unique_ptr<Node> op, IRBuilder<> &builder)
{
  if (op == nullptr)
  {
    return builder.getInt32(0);
  }
  if (instanceof <IntegerNode *>(op.get()))
  {
    IntegerNode *pd = dynamic_cast<IntegerNode *>(op.get());
    return pd->Codegen(builder);
  }
  if (instanceof <OperatorNode *>(op.get()))
  {
    OperatorNode *pd = dynamic_cast<OperatorNode *>(op.get());
    if (pd->token.id == type::ADDITION)
    {
      return builder.CreateAdd(traverse_tree(move(op->right), builder), traverse_tree(move(op->left), builder), "addtmp");
    }
  }
}
void gen_LLVM(vector<unique_ptr<FunctionNode>> op, string filename)
{

  llvm::LLVMContext context;
  llvm::Module module("example", context);
  FunctionType *funcType = FunctionType::get(Type::getInt32Ty(context), false);
  Function *mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module);
  BasicBlock *entryBlock = BasicBlock::Create(context, "entry", mainFunc);
  IRBuilder<> builder(entryBlock);

  Value *constantValue = builder.getInt32(42);

  // Return the constant value from the function
  builder.CreateRet(constantValue);

  string irString;
  llvm::raw_string_ostream llvmStringStream(irString);
  if (filename == "")
  {
    filename = "out.ll";
  }
  ofstream outfile("out/out.ll");
  module.print(llvmStringStream, nullptr);
  outfile << irString << endl;
  cout << "compiled successfully: output located at out/out.ll" << endl;
}
