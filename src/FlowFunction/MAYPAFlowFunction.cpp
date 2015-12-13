#include "../../include/FlowFunction/MAYPFlowFunction.h"
typedef map<Value*, set<Value*> >::iterator mapIter;
typedef set<Value*>::iterator setIter;

MAYPLatticeNode* MAYPFlowFunction::merge(vector<LatticeNode*> input){

	while(input.size() > 1){
		LatticeNode *node1 = input.back();
		input.pop_back();
		LatticeNode *node2 = input.back();
		input.pop_back();
		LatticeNode *newNode = node1->join(node2);
		input.push_back(newNode);
	}
	MAYPLatticeNode *mayNode = dyn_cast<MAYPLatticeNode>(input.front());
	return mayNode;
}


LatticeNode* MAYPFlowFunction::operator()(Instruction* inst, vector<LatticeNode*> input){
	in = merge(input);
	//copy in to out has some benefit. For some instructions we do not need to override visitor functions
	set<Value*> ptAll = in->point_to_all;
	map<Value*, set<Value*> > ptInfo = in->point_to_info;
	out = new MAYPLatticeNode(false, false, ptAll, ptInfo);
	visit(inst);
	LatticeNode* cast_out = out;
	return cast_out;
}


void MAYPFlowFunction::visitLoadInst(LoadInst &I){
/*	Value *val = &I;
	Instruction *inst = &I;	
	map<Value*, set<Value*> > ptInfo = out->point_to_info;

	Value* v = inst->getOperand(0);
	if(ptInfo.count(val) == 0){
		set<Value*> ptSet;
		ptSet.insert(v);
		ptInfo[val] = ptSet;
	}else{
		ptInfo[val].clear();
		ptInfo[val].insert(v);
	}
	out->point_to_info = ptInfo;

	errs() << "Load Instruction Debug\n";
	for(mapIter iter = out->point_to_info.begin(); iter != out->point_to_info.end(); iter++){
		errs() << "key:"<<*iter->first << "\n";
		set<Value*> s = iter->second;
		errs()<< "Values:\n"; 
		for(setIter it = s.begin(); it != s.end(); it++){
			errs() << **it << "\n";
		}
		errs() << "\n";
	}

	errs() << "load info:\n";
	Instruction * inst = &I;
	Value* val = &I;
	string name = "haha";
   // name = val->getName().data();
	
	errs() <<inst->getOperand(0)->getName() << "......\n";
//	errs() << *inst->getOperand(0)<< "\n";
	errs()<< I <<"\n";	
*/

}

void MAYPFlowFunction::visitStoreInst(StoreInst &I){
	Instruction* inst = &I;
	Value* x = inst->getOperand(0);
	Value* y = inst->getOperand(1);
//	errs() << I << "--" <<x << "--"<< y << "\n";
	map<Value*, set<Value*> > ptInfo = out->point_to_info;
	// y = &x case
	if(dyn_cast<AllocaInst>(x) && dyn_cast<AllocaInst>(y)){
//		errs() << "interpret y = &x:" << I << "\n";
//		errs() << *y << "->" << *x << "\n";
		ptInfo[y].clear();
		ptInfo[y].insert(x);
	}

	if(dyn_cast<LoadInst>(x)){
//		errs() << "loading-----" << "\n";
		Value* op_x = dyn_cast<Instruction>(x)->getOperand(0);
		Value* op_y = dyn_cast<Instruction>(y)->getOperand(0);
//		errs() << "y:" << *op_y << " x:" << *op_x << "\n";
		if(dyn_cast<AllocaInst>(op_x) && dyn_cast<AllocaInst>(op_y)){
			
			// *y = x
			set<Value*> s1 = ptInfo[y];
			set<Value*> s2 = ptInfo[x];
			if(s1.size() == 1){
				ptInfo[*s1.begin()].clear();
			}

//			errs() << "interpret *y = x" << I << "\n";
			for(setIter iter = s1.begin(); iter != s1.end(); iter++){
				for(setIter it = s2.begin(); it != s2.end(); it++){
					ptInfo[*iter].insert(*it);
//					errs() << **iter << "->"<<**it;
				}
			}

		}else if (dyn_cast<AllocaInst>(op_x) && !dyn_cast<AllocaInst>(op_y)){	
			//y = x case
//			errs() <<"interpret y = x:" << I << "\n";
//			errs()  << *op_x << "--" << *op_y << "\n";
			ptInfo[y] = ptInfo[op_x]; 
		}else if(dyn_cast<LoadInst>(op_x)){
			//y = *x case
			Value* temp = dyn_cast<Instruction>(op_x)->getOperand(0);
//			errs() << *temp<< "\n";
//			errs() <<"interpret y = *x" << I << "\n";
			set<Value*> first_level = ptInfo[temp];
//			errs() << "first level size:" << first_level.size() << "\n";
			for(setIter iter = first_level.begin(); iter != first_level.end(); iter++){
//				errs() << *temp << "->" << **iter<< "\n";
				set<Value*> second_level = ptInfo[*iter];
//				errs() << "\n\n";
//				errs() << "second level size:" << second_level.size() << "\n";
				for(setIter it = second_level.begin(); it != second_level.end();it++){
					ptInfo[y].insert(*it);
//					errs() << *y << "->" << **it << "\n";
				}
//				errs() << "\n\n";
			}
		}

	}
	out->point_to_info = ptInfo;
	/*	
	errs() << "left value"<<*y << "\n";
	errs() << "points to:" << "\n";
	for(setIter iter = ptInfo[y].begin(); iter != ptInfo[y].end(); iter++){
		errs() << **iter << "\n";
	}
	errs()<< "\n";
	out->point_to_info = ptInfo;*/
//	errs() << "Store info:\n";
//	Instruction *inst =&I;
//	errs() << inst->getOperand(0)->getName() << "\n";
//	errs() << inst->getOperand(1)->getName() << "\n";
//	errs() << *inst	<< "\n";

}

void MAYPFlowFunction::print(){
	errs()<< "---MAYPFlowFunction Info---\ninput lattice node\n";
	in->print();
	errs() << "\noutput lattice node\n";
	out->print();
}

