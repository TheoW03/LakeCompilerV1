#include <iostream>
#include <string>
#include <vector>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/MipsTarget/UtilFunctions.h"

void gen_LLVM(vector<FunctionNode *> op, string filename)
{
    // basic hello world in LLVM
    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>("MyModule", context);

    llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
    llvm::Function *mainFunction = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module.get());

    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context, "entry", mainFunction);
    llvm::IRBuilder<> builder(entryBlock);

    llvm::Value *constant = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    builder.CreateRet(constant);

    // module->print(llvm::outs(), nullptr);

    std::string irString;
    llvm::raw_string_ostream stringStream(irString);

    // Print the LLVM module to the output stream.
    module->print(stringStream, nullptr);

    // Get the LLVM IR as a string.
    std::string irCode = stringStream.str();

    // Output the LLVM IR string.
    std::cout << irCode << endl;
}
