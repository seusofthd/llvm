#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"

#include "../../include/Worklist/WorklistAlg.h"
#include "../../include/Lattice/MAYPLatticeNode.h"
#include "../../include/FlowFunction/MAYPFlowFunction.h"

using namespace llvm;
using namespace std;

namespace{
	struct MAYPPass: public FunctionPass{
		static char ID;
		MAYPPass():FunctionPass(ID){}

		virtual bool runOnFunction(Function &F){
			MAYPLatticeNode* beginNode = new MAYPLatticeNode(true, false);
//			beginNode->print();
			MAYPFlowFunction* flowFunc = new MAYPFlowFunction();
			FlowFunction* flowFunc_cast = dyn_cast<FlowFunction>(flowFunc);
			WorklistAlg* worklistAlg = new WorklistAlg();
			map<Instruction*, LatticeNode*> finalMap = worklistAlg->Run_Worklist(F, flowFunc_cast, beginNode);
			return true;
		}
	};
}


char MAYPPass::ID = 0;
static RegisterPass<MAYPPass> X("MAYPPass", "May point to analysis");
