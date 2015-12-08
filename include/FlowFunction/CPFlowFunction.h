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

/*
  // flow function interface
	LatticeNode* operator()(Instruction *inst, vector<LatticeNode*> input){
		/*
		LatticeNode* a = new CPLatticeNode();
		return a;
		in = merge(input);
		map<Value*, Instruction*> stmt = in->statements;
		out = new CSELatticeNode(false, false, stmt);
		visit(inst);
	//	errs() << "after visiting, statement size:" << out->statements.size() << "\n";
		LatticeNode* cast_out = out;
		return cast_out;
		////////////
		LatticeNode* a = new LatticeNode(CPLATTICE); // to delete
		return a; // to delete
	}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<LatticePoint *> CPFlowFunction::operator()(llvm::Instruction* instr, std::vector<LatticePoint *> info_in){
  // first, ensure that info_in_casted is empty
  info_in_casted = std::vector<CPLatticePoint *>();

  for(std::vector<LatticePoint *>::iterator it = info_in.begin(); it != info_in.end(); ++it) {
    CPLatticePoint *in_lattice_point = dyn_cast<CPLatticePoint>(*it);
    info_in_casted.push_back(in_lattice_point);
  }
  visit(instr);
  if (!ret_value) {
    errs() << "NEED TO IMPL VISIT FOR THIS INSTR: ";
    instr->print(errs());
    return std::vector<LatticePoint*>();
  }
  LatticePoint* lp = dyn_cast<LatticePoint>(ret_value);
  std::vector<LatticePoint*> info_out;
  info_out.push_back(lp);
  return info_out;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
*/

 /*
	virtual void print(){
		/*
		errs() << "---CPFlowFunction Info---\ninput lattice node\n";
		in->print();
		errs()<<"\noutput lattice node\n";
		out->print();
		
	}
	/*
	CPLatticeNode* merge(vector<LatticeNode*> input){
		/*
		errs()<<"CPFlowFunction\n";
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
		////////
		CPLatticeNode * cpNode = new CPLatticeNode();
		return cpNode;
	}
	*/

	//Override InstVisitor Functions ****
	/*
	void visitBinaryOperator(BinaryOperator &I){
	}
*/

//class member variable
	CPLatticeNode* in;
	CPLatticeNode* out;


};

#endif
