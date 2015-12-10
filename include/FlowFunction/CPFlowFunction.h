#ifndef DI_CPFLOWFUNCTION_H_
#define DI_CPFLOWFUNCTION_H_

#include "FlowFunction.h"
#include "llvm/IR/InstrTypes.h"
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
		//errs()<<"CPFlow function\n";
	}

  // flow function interface
	LatticeNode* operator()(Instruction *inst, vector<LatticeNode*> input){
		in = merge(input);
		map<Value*, int> info = in->data_info;
		out = new CPLatticeNode(false, false, info);
		visit(inst);
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
		//errs()<<"CPFlowFunction merge\n";
		while(input.size() > 1){
			LatticeNode *node1 = input.back();
			input.pop_back();
			LatticeNode *node2 = input.back();
			input.pop_back();
		//	errs() << "before join\n";
			LatticeNode *newNode = node1->join(node2);
		//	errs() << "after join\n";
			input.push_back(newNode);
		}
		CPLatticeNode * cpNode = dyn_cast<CPLatticeNode>(input.front());
		return cpNode;
	}

	void visitStoreInst(StoreInst &I){

		if (out->data_info.find(I.getPointerOperand()) != out->data_info.end()){
				out->data_info.erase(I.getPointerOperand());
		}
		if (isa<ConstantInt>(I.getValueOperand())){
			out->data_info[I.getPointerOperand()] = dyn_cast<ConstantInt>(I.getValueOperand())->getLimitedValue();
		}
		else if(out->data_info.find(I.getValueOperand()) != out->data_info.end()){
			out->data_info[I.getPointerOperand()] = out->data_info[I.getValueOperand()];
		}
	}

	void visitLoadInst(LoadInst &I){
		Value* tmp = &I;
		errs() << (out->data_info.find(I.getPointerOperand()) != out->data_info.end()) <<'\n';
		if (out->data_info.find(I.getPointerOperand()) != out->data_info.end()){
			out->data_info[tmp] = out->data_info[I.getPointerOperand()];
		}
		else{
			if (out->data_info.find(tmp) != out->data_info.end()){
				out->data_info.erase(tmp);
		//		errs() << "erase existing info\n";
			}
		}
	}

	void visitBinaryOperator(BinaryOperator &I){
		Instruction* inst = &I;
		Value* left = inst->getOperand(0);
		Value* right = inst->getOperand(1);
    int opcode = I.getOpcode();
		int result;
		int a;
		int b;
		if (out->data_info.find(&I) != out->data_info.end()){
			out->data_info.erase(&I);
		}
		if (!isa<ConstantInt>(left) && out->data_info.find(left) == out->data_info.end()){
			return;
		}
		if (!isa<ConstantInt>(right) && out->data_info.find(right) == out->data_info.end()){
			return;
		}
		if(isa<ConstantInt>(left) && isa<ConstantInt>(right)){
			a = dyn_cast<ConstantInt>(left)->getLimitedValue();
			b = dyn_cast<ConstantInt>(right)->getLimitedValue();
		}
		else if (isa<ConstantInt>(left) && out->data_info.find(right) != out->data_info.end()){
			a = dyn_cast<ConstantInt>(left)->getLimitedValue();
			b = out->data_info[right];
		}
		else if (isa<ConstantInt>(right) && out->data_info.find(left) != out->data_info.end()){
			a = out->data_info[left];
			b = dyn_cast<ConstantInt>(right)->getLimitedValue();
		}
		else if (out->data_info.find(right) != out->data_info.end() && out->data_info.find(left) != out->data_info.end()) {
			a = out->data_info[left];
			b = out->data_info[right];
		}
		else{
		//	errs() << "unexpected error!!!!!\n";
		}
		if (opcode == 8){
			//ADD
		//	errs() << "ADD\n";
			result = a + b;
		}
		else if (opcode == 10){
			//SUB
		//	errs() << "SUB\n";
			result = a - b;
		}
		else if(opcode == 12){
			//MUL
		//	errs() << "MUL\n";
			result = a * b;
		}
		out->data_info[&I] = result;
	}

//class member variable
	CPLatticeNode* in;
	CPLatticeNode* out;


};

#endif
