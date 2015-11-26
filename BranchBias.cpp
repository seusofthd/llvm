#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"
#include <iomanip>
#include <map>
#include <vector>
#include <utility>
using namespace llvm;
using namespace std;

namespace {
	struct BranchBias: public ModulePass{
		static char ID;
		BranchBias(): ModulePass(ID){}
		virtual bool runOnModule(Module &M){
			IRBuilder<> irBuilder(M.getContext());
			Constant* constant1 = M.getOrInsertFunction("_Z16insertStatisticsPKci", 
				Type::getVoidTy(M.getContext()), 
				Type::getInt8PtrTy(M.getContext()), 
				Type::getInt1Ty(M.getContext()), NULL);
			
			Function* function1 = cast<Function>(constant1);

			Constant* constant2 = M.getOrInsertFunction("_Z15printStatisticsv", Type::getVoidTy(M.getContext()), NULL);
			Function* Function2 = cast<Function>(constant2);
			for(Module::iterator func = M.begin(); func != M.end(); ++func){
				for(Function::iterator block = func->begin(); block != func->end(); ++block){
					// enter into the block;
					TerminatorInst *terminator = block->getTerminator();
					irBuilder.SetInsertPoint(terminator);
					BranchInst *branchInst = (BranchInst*) terminator;
					if(branchInst->isConditional()){
						// int val = branchInst->getCondition() == 0?0:1;
						
						// vector<Value*> args;
						// Value *funName = irBuilder.CreateGlobalStringPtr(func->getName());
						// Value *taken = ConstantInt::get(Type::getInt32Ty(M.getContext()), val);
						// args.push_back(funName);
						// args.push_back(taken);
						// irBuilder.CreateCall(function1, args);
						Value* operand = branchInst->getCondition();
						vector<Value*> args;
						Value *funName = irBuilder.CreateGlobalStringPtr(func->getName());
						args.push_back(funName);
						args.push_back(operand);
						irBuilder.CreateCall(function1, args);
					}
					if(func->getName() == "main"){
						for(BasicBlock::iterator inst = block->begin(); inst != block->end(); ++inst){
							if((string)inst->getOpcodeName() == "ret"){
								irBuilder.SetInsertPoint(func->back().getInstList().back().getPrevNode()->getNextNode());
								irBuilder.CreateCall(Function2);
							}
						}
					}
				}
					
			}


			return true;
		}
	};


}

char BranchBias::ID = 0;
static RegisterPass<BranchBias> X("branch", "BranchBias ananlysis");


