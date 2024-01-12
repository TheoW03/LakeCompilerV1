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
    switch (pd->token.id)
    {
    case type::ADDITION:
      return builder.CreateAdd(traverse_tree(move(op->left), builder), traverse_tree(move(op->right), builder), "addtmp");
    case type::SUBTRACT:
      return builder.CreateSub(traverse_tree(move(op->left), builder), traverse_tree(move(op->right), builder), "subtmp");
    case type::MULTIPLY:
      return builder.CreateMul(traverse_tree(move(op->left), builder), traverse_tree(move(op->right), builder), "multmp");
    case type::MOD:
      return builder.CreateSRem(traverse_tree(move(op->left), builder), traverse_tree(move(op->right), builder), "modtmp");
    case type::DIVISION:
      return builder.CreateSDiv(traverse_tree(move(op->left), builder), traverse_tree(move(op->right), builder), "divtmp");
    }
  }
}
void statement_gen(shared_ptr<Node> statement, IRBuilder<> &builder)
{
  if (instanceof <VaraibleDeclaration *>(statement.get()))
  {
    VaraibleDeclaration *pd = dynamic_cast<VaraibleDeclaration *>(statement.get());
  }
  else if (instanceof <ReturnStatment *>(statement.get()))
  {
    ReturnStatment *pd = dynamic_cast<ReturnStatment *>(statement.get());
    builder.CreateRet(traverse_tree(move(pd->expression), builder));
  }
}

void gen_LLVM(vector<unique_ptr<FunctionNode>> op, string filename)
{

  llvm::LLVMContext context;
  llvm::Module module("example", context);
  IRBuilder<> builder(context);

  for (size_t i = 0; i < op.size(); i++)
  {
    FunctionType *funcType;
    shared_ptr<FunctionNode> function = move(op[i]);
    if (function->returnType.has_value())
    {
      switch (function->returnType.value().id)
      {
      case type::INT:
      {
        funcType = FunctionType::get(builder.getInt32Ty(), false);
      }
      }
    }
    else
    {
      funcType = FunctionType::get(builder.getVoidTy(), false);
    }
    Function *funcRef = Function::Create(funcType, Function::ExternalLinkage, function->nameOfFunction.buffer, module);
    BasicBlock *entryBlock = BasicBlock::Create(context, "entry", funcRef);
    builder.SetInsertPoint(entryBlock);
    for (int i = 0; i < function->statements.size(); i++)
    {
      statement_gen(function->statements[i], builder);
    }
  }
  string irString;
  llvm::raw_string_ostream llvmStringStream(irString);
  if (filename == "")
    filename = "out.ll";
  ofstream outfile("out/out.ll");
  module.print(llvmStringStream, nullptr);
  outfile << irString << endl;
  cout << "compiled successfully: output located at out/out.ll" << endl;
}
