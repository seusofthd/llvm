#ifndef DI_CPFLOWFUNCTION_H_
#define DI_CPFLOWFUNCTION_H_

#include "FlowFunction.h"
#include "llvm/IR/InstrTypes.h"
#include "helper.h"
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


		//errs() << "interface!!!\n";
		//errs() << *inst <<'\n';
		in = merge(input);

		/*
		//errs() << "interface!!!123123\n";
		for(map<Value*, ConstantInt*>::iterator iter = in->statements.begin(); iter != in->statements.end(); iter++){
			 //errs() << (*iter) << '\n';
			 Value* var = iter->first;
			 ConstantInt* c = iter->second;
			 errs() << var->getName() <<  " --> " << c->getValue() << ", ";
		}
		errs()<<"\n";
		//errs() << "interface!!!22222222\n";
		*/

		map<Value*, ConstantInt*> stmt = in->statements;
		map<Value*, ConstantInt*> tmp_info = in->tmp_info;
		map<Value*, int> invalid_tmp = in->invalid_tmp;
		out = new CPLatticeNode(false, false, stmt);
		out -> tmp_info = tmp_info;
		out -> invalid_tmp = invalid_tmp;
		visit(inst);
	  //errs() << "after visiting, statement size:" << out->statements.size() << "\n";
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
			errs() << "before join\n";
			LatticeNode *newNode = node1->join(node2);
			errs() << "after join\n";
			input.push_back(newNode);
		}
		CPLatticeNode * cpNode = dyn_cast<CPLatticeNode>(input.front());
		return cpNode;
	}

	void visitStoreInst(StoreInst &I){
		/*
		errs() << I << '\n';
		errs() << *I.getValueOperand() << '\n';
		errs() << (out->tmp_info.find(I.getValueOperand( )) != out->tmp_info.end()) << '\n';
		if (out->tmp_info.find(I.getValueOperand()) != out->tmp_info.end()){
			if (out->invalid_tmp.find(I.getValueOperand()) == out->invalid_tmp.end()){
				out->statements[I.getPointerOperand()] = dyn_cast<ConstantInt>(out->tmp_info[I.getValueOperand()]);
			}
			else{
				out->statements.erase(I.getPointerOperand());
			}
		}
		else{
			out->statements[I.getPointerOperand()] = dyn_cast<ConstantInt>(I.getValueOperand());
		}*/
		//errs() << "storing!!\n";
		//errs() << *I.getValueOperand() << '\n';
		if (out->statements.find(I.getValueOperand()) != out->statements.end()){
			//errs() << "storing1112312\n";
			//errs() << *out->statements[I.getValueOperand()] << '\n';
			out->statements[I.getPointerOperand()] = dyn_cast<ConstantInt>(out->statements[I.getValueOperand()]);
		}
		else{
			if (I.getValueOperand()->getValueID() == 11 || I.getValueOperand()->getValueID() == 12){
				out->statements[I.getPointerOperand()] = dyn_cast<ConstantInt>(I.getValueOperand());
			}
		}
	}

	void visitLoadInst(LoadInst &I){
		/*
		errs() << "load in\n";
		Value* tmp = &I;
		if (out->statements.find(I.getPointerOperand()) != out->statements.end()){
			//errs() << "valid\n";
			out->tmp_info[tmp] = out->statements[I.getPointerOperand()];
		}
		else{
			//errs() << "invalid\n";
			out->invalid_tmp[tmp] = 0;
		}
		//errs() << "load out\n";
		*/
		//errs() << "load in\n";
		Value* tmp = &I;
		errs() << (out->statements.find(I.getPointerOperand()) != out->statements.end()) <<'\n';
		if (out->statements.find(I.getPointerOperand()) != out->statements.end()){
			//errs() << "loading\n";
			// exist in the incoming information
			//errs() << (*out->statements[I.getPointerOperand()]) << "\n";
			out->statements[tmp] = out->statements[I.getPointerOperand()];
		}
		//errs() << "load out\n";
	}

	void visitBinaryOperator(BinaryOperator &I){
		errs() << "BinaryOperator12345n";
		errs() << I <<'\n';
	}

/*

void CPFlowFunction::visitBinaryOperator(BinaryOperator &I) {
  // join ?
  CPLatticePoint* result = new CPLatticePoint(*(info_in_casted.back()));
  info_in_casted.pop_back();
  BinaryOperator* current = &I;
  std::pair<Use*, Use *> operands = helper::getOperands(I);
  Use* S1 = operands.first;
  Use* S2 = operands.second;
  ConstantInt* C1 = NULL;
  ConstantInt* C2 = NULL;

  if (isa<ConstantInt>(S1)) {
    C1 = dyn_cast<ConstantInt>(S1);
  } else if (result->representation.count(S1->get()) > 0) {
    C1 = result->representation[S1->get()];
  } else {
    C1 = ConstantInt::get(context, llvm::APInt(32, 0, true));
  }
  if (isa<ConstantInt>(S2)) {
    C2 = dyn_cast<ConstantInt>(S2->get());
  } else if (result->representation.count(S2->get()) > 0) {
    C2 = result->representation[S2->get()];
  } else {
    C2 = ConstantInt::get(context, llvm::APInt(32, 0, true));
  }
  // representation never initialized
  ret_value = new CPLatticePoint(false, false, std::map<Value*, ConstantInt*>(result->representation));
  ret_value->representation[current] = helper::foldBinaryOperator(I.getOpcode(), C1, C2);
}


*/



//class member variable
	CPLatticeNode* in;
	CPLatticeNode* out;


};

#endif
