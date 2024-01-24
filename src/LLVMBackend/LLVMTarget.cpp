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

Value *traverse_tree(unique_ptr<Node> op, map<string, Value *> &varaible, IRBuilder<> &builder)
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
  if (instanceof <VaraibleReference *>(op.get()))
  {
    VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op.get());
    if (varaible.find(pd->varaible.buffer) == varaible.end())
    {
      cout << "varaible mis match error" << endl;
      exit(EXIT_FAILURE);
    }
    return builder.CreateLoad(builder.getInt32Ty(), varaible[pd->varaible.buffer], "var_ref");
    // return varaible[pd->varaible.buffer];
  }
  if (instanceof <OperatorNode *>(op.get()))
  {
    OperatorNode *pd = dynamic_cast<OperatorNode *>(op.get());
    switch (pd->token.id)
    {
    case type::ADDITION:
      return builder.CreateAdd(traverse_tree(move(op->left), varaible, builder), traverse_tree(move(op->right), varaible, builder), "addtmp");
    case type::SUBTRACT:
      return builder.CreateSub(traverse_tree(move(op->left), varaible, builder), traverse_tree(move(op->right), varaible, builder), "subtmp");
    case type::MULTIPLY:
      return builder.CreateMul(traverse_tree(move(op->left), varaible, builder), traverse_tree(move(op->right), varaible, builder), "multmp");
    case type::MOD:
      return builder.CreateSRem(traverse_tree(move(op->left), varaible, builder), traverse_tree(move(op->right), varaible, builder), "modtmp");
    case type::DIVISION:
      return builder.CreateSDiv(traverse_tree(move(op->left), varaible, builder), traverse_tree(move(op->right), varaible, builder), "divtmp");
    }
  }
  return builder.getInt32(0);
}
void statement_gen(shared_ptr<Node> statement, map<string, Value *> &varaible, IRBuilder<> &builder)
{
  map<type, llvm::Type *> type_map;
  type_map[type::INT] = builder.getInt32Ty();
  type_map[type::STRING] = PointerType::getUnqual(builder.getInt8Ty());
  if (instanceof <VaraibleDeclaration *>(statement.get()))
  {
    VaraibleDeclaration *pd = dynamic_cast<VaraibleDeclaration *>(statement.get());
    Value *v = builder.CreateAlloca(type_map[pd->typeOfVar.id], nullptr, pd->varaible.buffer);
    builder.CreateStore(traverse_tree(move(pd->expression), varaible, builder), v);
    varaible[pd->varaible.buffer] = v;
  }
  else if (instanceof <VaraibleReference *>(statement.get()))
  {
    VaraibleReference *pd = dynamic_cast<VaraibleReference *>(statement.get());
    if (varaible.find(pd->varaible.buffer) == varaible.end())
    {
      cout << "varaible mis match error" << endl;
      exit(EXIT_FAILURE);
    }
    else
    {
      builder.CreateStore(traverse_tree(move(pd->expression), varaible, builder), varaible[pd->varaible.buffer]);
    }
  }
  else if (instanceof <ReturnStatment *>(statement.get()))
  {
    ReturnStatment *pd = dynamic_cast<ReturnStatment *>(statement.get());
    builder.CreateRet(traverse_tree(move(pd->expression), varaible, builder));
  }
}
void llvmTestFunction()
{
}
void gen_LLVM(vector<unique_ptr<FunctionNode>> op, string filename)
{
  // llvmTestFunction();
  // exit(EXIT_SUCCESS);
  llvm::LLVMContext context;
  llvm::Module module("example", context);

  IRBuilder<> builder(context);
  map<type, llvm::Type *> type_map;
  type_map[type::INT] = builder.getInt32Ty();
  type_map[type::STRING] = PointerType::getUnqual(builder.getInt8Ty());
  // PointerType::getUnqual(PointerType::getUnqual(builder.getInt32()));

  for (size_t i = 0; i < op.size(); i++)
  {
    FunctionType *funcType;
    shared_ptr<FunctionNode> function = move(op[i]);

    map<string, Value *> varaible;
    // map[type::FLOAT] = builder.getFloat
    vector<shared_ptr<VaraibleDeclaration>> params = function->params;

    // llvm::Type *arr_of_paramtypes[params.size()];
    vector<llvm::Type *> arr_of_paramtypes;
    if (function->params.size() != 0)
    {
      for (size_t i2 = 0; i2 < params.size(); i2++)
      {
        // arr_of_paramtypes[i2] = type_map[params[i2]->typeOfVar.id];
        arr_of_paramtypes.push_back(type_map[params[i2]->typeOfVar.id]);
      }
    }

    if (function->returnType.has_value())
    {

      funcType = FunctionType::get(type_map[function->returnType.value().id],
                                   arr_of_paramtypes, false);
    }
    else
    {
      funcType = FunctionType::get(builder.getVoidTy(), false);
    }
    Function *funcRef = Function::Create(funcType, Function::ExternalLinkage, function->nameOfFunction.buffer, module);
    BasicBlock *entryBlock = BasicBlock::Create(context, "entry", funcRef);
    builder.SetInsertPoint(entryBlock);
    llvm::Function::arg_iterator args = funcRef->arg_begin();
    for (size_t i2 = 0; i2 < params.size(); i2++)
    {
      (args)->setName(params[i2]->varaible.buffer);
      varaible[params[i2]->varaible.buffer] = args;
      args++;
    }

    // llvm::LLVMGetParam(funcRef, 1)->setName("b");
    // funcRef.GetParam(0);
    for (int i = 0; i < function->statements.size(); i++)
    {
      statement_gen(function->statements[i], varaible, builder);
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
