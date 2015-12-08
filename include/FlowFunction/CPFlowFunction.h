#ifndef DI_CPFLOWFUNCTION_H_
#define DI_CPFLOWFUNCTION_H_

#include "FlowFunction.h"
#include "llvm/IR/InstrTypes.h"
// #include "helper/helper.h"
#include "../Lattice/CPLatticeNode.h"
#include "llvm/InstVisitor.h"
#include <map>
#include <vector>

using namespace llvm;
using namespace std;

class CPFlowFunction:public FlowFunction, public InstVisitor<CPFlowFunction>{



public:

	static bool classof(const FlowFunction *func){
		return func->type == CPFLOW;
	}

	CPFlowFunction():FlowFunction(CPFLOW){
		errs()<<"CPFlow function\n";
	}

  // flow function interface
	LatticeNode* operator()(Instruction *inst, vector<LatticeNode*> input){
		in = merge(input);
		map<Value*, ConstantInt*> stmt = in->statements;
		out = new CPLatticeNode(false, false, stmt);
		visit(inst);
	//	errs() << "after visiting, statement size:" << out->statements.size() << "\n";
		LatticeNode* cast_out = out;
		return cast_out;
	}

	virtual void print(){
		errs() << "---CPFlowFunction Info---\ninput lattice node\n";
		in->print();
		errs()<<"\noutput lattice node\n";
		out->print();
	}

	CPLatticeNode* merge(vector<LatticeNode*> input){
		errs()<<"CPFlowFunction merge\n";
		while(input.size() > 1){
			LatticeNode *node1 = input.back();
			input.pop_back();
			LatticeNode *node2 = input.back();
			input.pop_back();
			LatticeNode *newNode = node1->join(node2);
			input.push_back(newNode);
		}
		CPLatticeNode * cpNode = dyn_cast<CPLatticeNode>(input.front());
		return cpNode;
	}

	void visitBinaryOperator(BinaryOperator &I){
		errs() << "BinaryOperator!!!!!\n";


	}

//class member variable
	CPLatticeNode* in;
	CPLatticeNode* out;


};

#endif
