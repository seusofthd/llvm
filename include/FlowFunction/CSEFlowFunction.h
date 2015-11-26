#ifndef CSEFLOWFUNCTION_H_
#define CSEFLOWFUNCTION_H_

#include "FlowFunction.h"
#include "llvm/InstVisitor.h"

using namespace llvm;
using namespace std;

class CSEFlowFunction:public FlowFunction, public InstVisitor<CSEFlowFunction>{
public:
	CSEFlowFunction():FlowFunction(CSEFLOW){}
	LatticeNode* operator()(Instruction *inst, vector<LatticeNode*> input);	
	static bool classof(const FlowFunction *func){
		return func->type == CSEFLOW;
	}
	CSELatticeNode* merge(vector<LatticeNode*> input);
	/*Override InstVisitor Functions*/
	void visitBinaryOperator(BinaryOperator &I);
//class member variable
	CSELatticeNode* in;
	CSELatticeNode* out;
};

#endif
