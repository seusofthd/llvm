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

		//map<Value*, int> info = in->data_info;
		map<Value*, int> info = in->data_info;
		//map<Value*, int> invalid_tmp = in->invalid_tmp;
		out = new CPLatticeNode(false, false, info);
		//out -> tmp_info = tmp_info;
		//out -> invalid_tmp = invalid_tmp;
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
		errs() << (out->data_info.find(I.getPointerOperand()) != out->data_info.end()) <<'\n';
		if (out->data_info.find(I.getPointerOperand()) != out->data_info.end()){
			//errs() << "loading\n";
			// exist in the incoming information
			//errs() << (*out->statements[I.getPointerOperand()]) << "\n";
			out->data_info[tmp] = out->data_info[I.getPointerOperand()];
		}
		/////////////
		else{
			if (out->data_info.find(tmp) != out->data_info.end()){
				out->data_info.erase(tmp);
				errs() << "erase existing info\n";
			}
		}
		//errs() << "load out\n";
	}

	void visitBinaryOperator(BinaryOperator &I){
		Instruction* inst = &I;

		Value* left = inst->getOperand(0);
		Value* right = inst->getOperand(1);
		errs() << "YUYUYU:  " << *left << ',' << *right << '\n';


		errs() << "BinaryOperator12345n";
		//User::op_iterator it = I.op_begin();
    int opcode = I.getOpcode();

    //bool flag;
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
			//a = dyn_cast<ConstantInt>(left)->getLimitedValue();
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
			errs() << "unexpected error!!!!!\n";
		}
		/*
		if (out->statements.find(*it) != out->statements.end()
		    && out->statements.find(*(it+1)) != out->statements.end()){
		}*/

		if (opcode == 8){
			//ADD
			errs() << "ADD\n";
			result = a + b;
		}
		else if (opcode == 10){
			//SUB
			errs() << "SUB\n";
			result = a - b;
		}
		else if(opcode == 12){
			//MUL
			errs() << "MUL\n";
			result = a * b;
		}

		out->data_info[&I] = result;

		//if(out->statements.find())

		//errs() << I[it] << "YEYEYE" <<I[it] <<'\n';
		//errs() << "YEYEYE" << "***"<< (I.getOpcode()) <<"***" << *(*it) <<  ": " << *(out->statements[*it])  << " and " << *(*(it+1)) <<'\n';
		//errs() << "YEYEYE" << I << "***"<< (I.getOpcode()) <<"***" <<  ": " << *(out->data_info[*it])  << " and " << *(*(it+1)) <<'\n';

	}


/*
	static std::pair<Use*, Use *> getOperands(BinaryOperator &BO){
	    Use* S1;
	    Use* S2;
	    int i = 0;
	    for (User::op_iterator OP = BO.op_begin(), OPE = BO.op_end(); OP != OPE; ++OP){
	      if(i == 0){
	        S1 = &*OP;
	      }
	      else{
	        S2 = &*OP;
	      }
	      i++;
	    }
	    return std::make_pair(S1, S2);
	  }
*/
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
