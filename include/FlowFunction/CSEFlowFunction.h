#ifndef DI_CSEFLOWFUNCTION_H_
#define DI_CSEFLOWFUNCTION_H_

#include "FlowFunction.h"
#include "llvm/InstVisitor.h"

using namespace llvm;
using namespace std;

class CSEFlowFunction:public FlowFunction, public InstVisitor<CSEFlowFunction>{
public:
	CSEFlowFunction():FlowFunction(CSEFLOW){}
//	~CSEFlowFunction(){delete in; delete out;}
	LatticeNode* operator()(Instruction *inst, vector<LatticeNode*> input);	
	static bool classof(const FlowFunction *func){
		return func->type == CSEFLOW;
	}
	virtual void print();
	CSELatticeNode* merge(vector<LatticeNode*> input);

	/*Override InstVisitor Functions*/
	virtual void visitBinaryOperator(BinaryOperator &I);
//class member variable
	CSELatticeNode* in;
	CSELatticeNode* out;
};

#endif

