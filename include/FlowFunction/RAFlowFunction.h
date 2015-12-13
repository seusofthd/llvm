#ifndef DI_RAFLOWFUNCTION_H_
#define DI_RAFLOWFUNCTION_H_

#include "FlowFunction.h"
#include "llvm/InstVisitor.h"
using namespace llvm;
using namespace std;

class RAFlowFunction:public FlowFunction, public InstVisitor<RAFlowFunction>{
public:
	RAFlowFunction():FlowFunction(RAFLOW){}
//	~RAFlowFunction(){delete in; delete out;}
	LatticeNode* operator()(Instruction *inst, vector<LatticeNode*> input);	
	static bool classof(const FlowFunction *func){
		return func->type == RAFLOW;
	}
	virtual void print();
	RALatticeNode* merge(vector<LatticeNode*> input);
	std::map<Value *, int> counter_map;
	/*Override InstVisitor Functions*/
	void visitBinaryOperator(BinaryOperator &I);
        void visitPHINode(PHINode &PHI);
//class member variable
	RALatticeNode* in;
	RALatticeNode* out;
};

#endif


