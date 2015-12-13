#include "../../include/FlowFunction/RAFlowFunction.h"
typedef vector<LatticeNode*>::iterator laIter;
typedef map<Value*, ConstantRange*>::iterator mapIter;
LatticeNode* RAFlowFunction::operator()(Instruction *inst, vector<LatticeNode*> input){
        //errs() << "start the RAFlowFunction (): input size:" << input.size() << "\n";
	in = merge(input);
	out = new RALatticeNode(false, false, in->RangeMap);
	//errs() << "dump input:\n";
	//out->print();
        RALatticeNode* old_Lattice = new RALatticeNode(false, false, in->RangeMap);
	visit(inst);
	//errs() << "instruction:\n";
        //inst->dump();
	//errs() << "dump output:\n";
	//out->print();
	//errs() << "after visiting, statement size:" << "\n";//out->statements.size() << "\n";
	LatticeNode* temp = out;
  	RALatticeNode* temp1 = dyn_cast<RALatticeNode>(temp);
  	RALatticeNode* new_Lattice = new RALatticeNode(*temp1);
	std::vector<Value*> diffs = new_Lattice->differInRange(old_Lattice);
  
  	for (unsigned i = 0; i < diffs.size(); i++) {
    		Value* val = diffs[i];
    		if (counter_map.count(val) <= 0){
      				counter_map[val] = 1;
    		}
    		else if (counter_map[val] == 100){
      				new_Lattice->RangeMap[val] = new ConstantRange(32, true);
    		}
    		else{
      				counter_map[val] = counter_map[val] + 1;
    		}
  	}
	return new_Lattice;
      
}

void RAFlowFunction::visitBinaryOperator(BinaryOperator &I){
      Use* Operand1;
      Use* Operand2;
      ConstantRange*  Range1;
      ConstantRange*  Range2;
      ConstantRange*  NewRange;
      NewRange = new ConstantRange(32, false);
      int i = 0;
      for (User::op_iterator iter = I.op_begin(), end = I.op_end(); iter != end; ++iter){
            if(i == 0){
                  Operand1 = &*iter;
            }
            else{
                  Operand2 = &*iter;
                }
            i++;
            }

      if (isa<ConstantInt>(Operand1)) {
            ConstantInt* Constant1 = cast<ConstantInt>(Operand1);
            Range1 = new ConstantRange(Constant1->getValue());
      }
      else if (out->RangeMap.count(Operand1->get()) > 0){
            Range1 = out->RangeMap[Operand1->get()];
      }
      else{
            Range1 = new ConstantRange(32, false);
       }
      
      if (isa<ConstantInt>(Operand2)) {
            ConstantInt* Constant2 = cast<ConstantInt>(Operand2);
            Range2 = new ConstantRange(Constant2->getValue());
      }
      else if (out->RangeMap.count(Operand2->get()) > 0){
            Range2 = out->RangeMap[Operand2->get()];
      }
      else{
            Range2 = new ConstantRange(32, false);
      }  
      
      unsigned Op = I.getOpcode();  
      switch (Op) {
    	case Instruction::Add:
      		*NewRange = Range1->add(*Range2);
      	break;
    	case Instruction::Sub:
      		*NewRange = Range1->sub(*Range2);
      	break;
    	case Instruction::Mul:
      		*NewRange = Range1->multiply(*Range2);
      	break;
    	case Instruction::UDiv:
      		*NewRange = Range1->udiv(*Range2);
      	break;
	case Instruction::Or:
      		*NewRange = Range1->binaryOr(*Range2);
      	break;
    	case Instruction::And:
      		*NewRange = Range1->binaryAnd(*Range2);
      	break;
    	case Instruction::LShr:
      		*NewRange = Range1->lshr(*Range2);
      	break;
    	case Instruction::Shl:
      		*NewRange = Range1->shl(*Range2);
      	break;
        default:
                errs() << "default\n";
      }
      out->RangeMap[&I] = NewRange;

      //errs() << "I'm visting BinaryOper!";
      
}


void RAFlowFunction::visitPHINode(PHINode &PHI){
      //errs() << "I'm visiting PHI!\n";
      ConstantRange* NewRange = new ConstantRange(32, false);
      int PhiNum = PHI.getNumIncomingValues(); 
      for (int i = 0; i != PhiNum; i++){
            Value* IncomingValue = PHI.getIncomingValue(i);
	    //errs() << "dump phi incomming value:\n";
	    //IncomingValue -> dump();
	    //errs() << "\n";
            //errs() << "dump outmap:\n";
            //out->print();
            if (out->RangeMap.count(IncomingValue) > 0) {
		   //errs() << "dump phi var candidate: ";
		   //out->RangeMap[IncomingValue]->dump();
                  *NewRange = NewRange->unionWith(*(out->RangeMap[IncomingValue]));
            }
            else if(isa<ConstantInt>(IncomingValue)){
                  ConstantInt* Constant1 = cast<ConstantInt>(IncomingValue);
                  ConstantRange* ConstantRange1 = new ConstantRange(Constant1->getValue());
                  *NewRange = NewRange->unionWith(*ConstantRange1);
            }
    } 
  
    out->RangeMap[&PHI] = NewRange;
}

/*I don't know whether this kind of writing will invoke the LatticeNode join function or RALatticeNode join function*/
RALatticeNode* RAFlowFunction::merge(vector<LatticeNode*> input){
	while(input.size() > 1){
		LatticeNode *node1 = input.back();
		input.pop_back();
		LatticeNode *node2 = input.back();
		input.pop_back();
		LatticeNode *newNode = node1->join(node2);
		input.push_back(newNode);
	}
	RALatticeNode * raNode = dyn_cast<RALatticeNode>(input.front());
	return raNode;
}

void RAFlowFunction::print(){
	errs() << "---RAFlowFunction Info---\ninput lattice node\n";
	in->print();
	errs()<<"\noutput lattice node\n";
	out->print();
}
