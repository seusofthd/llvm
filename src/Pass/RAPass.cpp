#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"
#include "llvm/Support/ConstantRange.h"
#include "../../include/Worklist/WorklistAlg.h"
#include "../../include/Lattice/RALatticeNode.h"
#include "../../include/FlowFunction/RAFlowFunction.h"
using namespace llvm;
using namespace std;

namespace {
	struct RAPass: public ModulePass{
		static char ID;
		RAPass():ModulePass(ID){}
		virtual bool runOnModule(Module &M){
			for(Module::iterator func = M.begin(); func != M.end(); func++){
				//errs() << "line0\n";
                                RALatticeNode* beginNode = new RALatticeNode(true, false);
				//errs() << "line1\n";
				RAFlowFunction* flowFunc = new RAFlowFunction();
				//errs() << "line2\n";
				FlowFunction* flowFunc_cast = dyn_cast<FlowFunction>(flowFunc);
				//errs() << "line3\n";
				WorklistAlg* worklistAlg = new WorklistAlg();
				//errs() << "line4\n";
				map<Instruction*, LatticeNode*> finalMap = worklistAlg->Run_Worklist(*func, flowFunc_cast, beginNode); 	
				//worklistAlg->Run_Worklist(*func, flowFunc_cast, beginNode); 	
//				errs() << "finalMap size is: " << finalMap.size()<<"\n";			
				//errs() << "done\n";	
			}
			return true;
		}
	};

}




















char RAPass::ID = 0;
static RegisterPass<RAPass> X("RAPass", "Range Analysis"); 
