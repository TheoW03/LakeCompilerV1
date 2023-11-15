#include <iostream>
#include <string>
#include <vector>
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/Module.h"
// #include "llvm/IR/IRBuilder.h"

// #include "llvm/IR/Function.h"
// #include "llvm/IR/Type.h"
// #include "llvm/Support/TargetSelect.h"
// #include "llvm/IR/Constants.h"
// #include "llvm/Support/raw_ostream.h"
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/MipsTarget/UtilFunctions.h"
#include "../../src/CompilerFrontend/Lexxer.h"

using namespace std;

void gen_LLVM(vector<FunctionNode *> op, string filename)
{
    // // basic hello world in LLVM
    // llvm::LLVMContext context;
    // std::unique_ptr<llvm::Module> module = make_unique<llvm::Module>("MyModule", context);

    // for (int i = 0; i < op.size(); i++)
    // {
    //     // llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
    //     llvm::FunctionType *funcType;
    //     if (op[i]->returnType != nullptr)
    //     {
    //         switch (op[i]->returnType->id)
    //         {
    //         case type::INT:
    //             funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
    //             break;
    //         case type::FLOAT:
    //             funcType = llvm::FunctionType::get(llvm::Type::getFloatTy(context), false);
    //             break;
    //         case type::CHAR:
    //             funcType = llvm::FunctionType::get(llvm::Type::getInt8Ty(context), false);
    //             break;
    //         case type::BOOL:
    //             funcType = llvm::FunctionType::get(llvm::Type::getInt1Ty(context), false);
    //             break;
    //         }
    //     }
    //     else
    //     {
    //         funcType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
    //     }
    //     llvm::Function *mainFunction = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, op[i]->nameOfFunction->buffer, module.get());
    //     llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context, "entry", mainFunction);
    //     llvm::IRBuilder<> builder(entryBlock);
    // }

    // // llvm::Value *constant = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
    // // builder.CreateRet(constant);

    // // module->print(llvm::outs(), nullptr);

    // string irString;
    // llvm::raw_string_ostream stringStream(irString);

    // // Print the LLVM module to the output stream.
    // module->print(stringStream, nullptr);

    // // Get the LLVM IR as a string.
    // string irCode = stringStream.str();

    // // Output the LLVM IR string.

    // std::cout << irCode << endl;
}
