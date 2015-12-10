#ifndef MAYPAFLOWFUNCTION_H_
#define MAYPAFLOWFUNCTION_H_

#include "FlowFunction.h"
#include "../Lattice/MAYPLatticeNode.h"

#include "llvm/InstVisitor.h"
using namespace std;
using namespace llvm;

class MAYPFlowFunction:public FlowFunction, public InstVisitor<MAYPFlowFunction>{
public:
	MAYPFlowFunction():FlowFunction(MAYPFLOW){}
	LatticeNode* operator()(Instruction* inst, vector<LatticeNode*>input);
	static bool classof(const FlowFunction* func){
		return func->type == MAYPFLOW;
	}
	// Function override from FlowFunction
	virtual void print();
	MAYPLatticeNode* merge(vector<LatticeNode*> input);
	//Function override from InstVisitor
	virtual void visitLoadInst(LoadInst &I);
	virtual void visitStoreInst(StoreInst &I);
	MAYPLatticeNode* in;
	MAYPLatticeNode* out;
};

#endif
