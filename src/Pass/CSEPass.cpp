#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"

#include "../../include/Worklist/WorklistAlg.h"
#include "../../include/Lattice/CSELatticeNode.h"
#include "../../include/FlowFunction/CSEFlowFunction.h"
using namespace llvm;
using namespace std;

namespace {
	struct CSEPass: public ModulePass{
		static char ID;
		CSEPass():ModulePass(ID){}
		virtual bool runOnModule(Module &M){
			for(Module::iterator func = M.begin(); func != M.end(); func++){
				CSELatticeNode* beginNode = new CSELatticeNode(true, false);
				CSEFlowFunction* flowFunc = new CSEFlowFunction();
				FlowFunction* flowFunc_cast = dyn_cast<FlowFunction>(flowFunc);
				WorklistAlg* worklistAlg = new WorklistAlg();
				map<Instruction*, LatticeNode*> finalMap = worklistAlg->Run_Worklist(*func, flowFunc_cast, beginNode); 	
//				errs() << "finalMap size is: " << finalMap.size()<<"\n";			
				errs() << "done\n";	
			}
			return true;
		}
	};

}
















char CSEPass::ID = 0;
static RegisterPass<CSEPass> X("CSEPass", "Common subexpression elimination"); 
