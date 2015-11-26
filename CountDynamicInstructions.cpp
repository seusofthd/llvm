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
using namespace llvm;
using namespace std;

namespace {
	struct CountDynamicinstructions: public ModulePass {
		static char ID;
		CountDynamicinstructions(): ModulePass(ID){}

		virtual bool runOnModule(Module& M){
			IRBuilder<> irbuilder(M.getContext());
			Constant* constant1 = M.getOrInsertFunction("_Z11insertCountPci", Type::getVoidTy(M.getContext()), Type::getInt8PtrTy(M.getContext()), Type::getInt32Ty(M.getContext()), NULL);
			Function* Function1 = cast<Function>(constant1);
			Constant* constant2 = M.getOrInsertFunction("_Z8printMapv", Type::getVoidTy(M.getContext()), NULL);
			Function* Function2 = cast<Function>(constant2);
			for(Module::iterator func = M.begin(); func != M.end(); ++func){
				for(Function::iterator block = func->begin(); block != func->end(); ++block){
					// inside is the block
					map<const char*, int> counterMap;

					for(BasicBlock::iterator inst = block->begin(); inst != block->end(); ++inst){
						counterMap[inst->getOpcodeName()]++;
					}
					
					
					TerminatorInst *terminator = block->getTerminator();

					irbuilder.SetInsertPoint(terminator);
					vector<Value*> argVec;
					for(map<const char*, int>::iterator iter = counterMap.begin(); iter != counterMap.end(); iter++){
						Value *OpcodeName = irbuilder.CreateGlobalStringPtr(iter->first);
						Value *OpcodeValue = ConstantInt::get(Type::getInt32Ty(M.getContext()), iter->second);						
						argVec.clear();
						argVec.push_back(OpcodeName);
						argVec.push_back(OpcodeValue);
						irbuilder.CreateCall(Function1, argVec);
					}

					if(func->getName() == "main"){
						for(BasicBlock::iterator inst = block->begin(); inst != block->end(); ++inst){
							if((string)inst->getOpcodeName() == "ret"){
								irbuilder.SetInsertPoint(func->back().getInstList().back().getPrevNode()->getNextNode());
								irbuilder.CreateCall(Function2);
							}
						}
					}
				}
			}
			return true;
		}

	}; 
}

char CountDynamicinstructions::ID = 0;
static RegisterPass<CountDynamicinstructions> X("dynamic", "Dynamic instruction pass");