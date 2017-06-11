// LVIExample.cpp - Sample usage of LazyValueInfo
//
// This software is in the public domain.

#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/LazyValueInfo.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

struct LVIExample : public llvm::ModulePass {
  static char ID;

  static LVIExample *instance;

  LVIExample() : llvm::ModulePass(ID) {}

  virtual bool runOnModule(llvm::Module &m);

  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};

bool LVIExample::runOnModule(llvm::Module &m) {
  for (llvm::Module::iterator func = m.begin(), fe = m.end(); func != fe;
       ++func) {

    if (func->isDeclaration())
      continue;

    llvm::LazyValueInfo &LVI = getAnalysis<llvm::LazyValueInfo>(*func);
    for (llvm::Function::iterator bit = func->begin(), bie = func->end();
         bit != bie; ++bit) {
      for (llvm::BasicBlock::iterator it = bit->begin(), ie = bit->end();
           it != ie; ++it) {
        llvm::Instruction *inst = &(*it);
        if (!inst->getType()->isIntegerTy())
          continue;

        if (inst->getOpcode() == llvm::Instruction::PHI)
          continue;

        if (llvm::Constant *c = LVI.getConstant(inst, inst->getParent())) {
          inst->dump();
          llvm::errs() << "is constant with value: ";
          c->dump();
        }
      }
    }
  }
  return false;
}

void LVIExample::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  AU.setPreservesAll();
  AU.addRequired<llvm::LazyValueInfo>();
}

int main(int argc, char **argv) {
  llvm::LLVMContext &context = llvm::getGlobalContext();
  llvm::SMDiagnostic err;
  llvm::Module *M = llvm::ParseIRFile(argv[1], err, context);
  if (!M) {
    llvm::errs() << "failed to read IR file" << argv[1] << "\n";
    return 1;
  }

  llvm::PassManager PM;
  PM.add(llvm::createLazyValueInfoPass());
  PM.add(new LVIExample());
  PM.run(*M);

  return 0;
}

char LVIExample::ID = 0;

static llvm::RegisterPass<LVIExample> X("range-analysis", "Range analysis");
