#define DEBUG_TYPE "count"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"
#include <map>
using namespace llvm;

namespace {
  struct CountStaticInstructions : public ModulePass {
    static char ID;
    CountStaticInstructions() : ModulePass(ID){}
    std::map<int, int> m;

    virtual bool runOnModule(Module &M){
      for(Module::iterator f = M.begin(); f != M.end(); ++f){
        for(Function::iterator b = f->begin(), be = f->end(); b!=be; ++b) {
          for(BasicBlock::iterator i = b->begin(), ie = b->end(); i != ie; i++){
            if(m.find(i->getOpcode()) != m.end()){
              m[i->getOpcode()] ++;
            }else{
              m[i->getOpcode()] = 1;
            }
          }
        }
      }
      return false;
    }

    void print(raw_ostream &O, const Module *M)const {
//      print(O);
      int totalCount = 0;
      for (std::map<int, int>::const_iterator iter = m.begin(); iter != m.end(); ++iter){
        O << Instruction::getOpcodeName(iter->first) << "\t"<< iter->second << "\n";
        totalCount += iter->second;
      }
      O << "Total\t" <<totalCount << '\n';
    }
  };
}

char CountStaticInstructions::ID = 0;
static RegisterPass<CountStaticInstructions> Y("INCNNT", "InstCount Pass");
