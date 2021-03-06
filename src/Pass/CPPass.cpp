#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"

#include "../../include/Worklist/WorklistAlg.h"
#include "../../include/Lattice/CPLatticeNode.h"
#include "../../include/FlowFunction/CPFlowFunction.h"
using namespace llvm;
using namespace std;

namespace {
	struct CPPass: public FunctionPass{
		static char ID;
		CPPass():FunctionPass(ID){}
		virtual bool runOnFunction(Function &func){
//			for(Module::iterator func = M.begin(); func != M.end(); func++){
				CPLatticeNode* beginNode = new CPLatticeNode(true, false);
				CPFlowFunction* flowFunc = new CPFlowFunction();
				FlowFunction* flowFunc_cast = dyn_cast<FlowFunction>(flowFunc);
				WorklistAlg* worklistAlg = new WorklistAlg();
//				errs()<<"begin to run worklistAlg\n";
				errs()<<"\n";
				map<Instruction*, LatticeNode*> finalMap = worklistAlg->Run_Worklist(func, flowFunc_cast, beginNode);
				errs() << "done\n";

//			}
			return true;
		}
	};

}





















char CPPass::ID = 0;
static RegisterPass<CPPass> X("CPPass", "Constant Propogation");
