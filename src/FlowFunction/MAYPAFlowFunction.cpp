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
	Value *val = &I;
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
/*
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
	errs() << I << "--" <<x << "--"<< y << "\n";
/*
	map<Value*, set<Value*> > ptInfo = out->point_to_info;
	// y = &x case
	if(dyn_cast<AllocaInst>(x) && dyn_cast<AllocaInst>(y)){
		ptInfo[y].clear();
		ptInfo[y].insert(x);
	}

	if(dyn_cast<LoadInst>(x)){
		errs() << "loading-----" << "\n";
		Value* op_x = dyn_cast<Instruction>(x)->getOperand(0);
		Value* op_y = dyn_cast<Instruction>(y)->getOperand(0);
		if(dyn_cast<AllocaInst>(op_x)){
			//y = x case
			errs() <<"interpret y = x:" << I << "\n";
			ptInfo[y] = ptInfo[op_x]; 
		}else if(dyn_cast<LoadInst>(op_x)){
			//y = *x case
			Value* temp = dyn_cast<LoadInst>(op_x);
			errs() <<"interpret y = *x" << I << "\n";
			set<Value*> first_level = ptInfo[temp];
			for(setIter iter = first_level.begin(); iter != first_level.end(); iter++){
				set<Value*> second_level = ptInfo[*iter];
				for(setIter it = second_level.begin(); it != second_level.end();iter++){
					ptInfo[y] = ptInfo[*it];
				}
			}
		}else if(dyn_cast<LoadInst>(y)){
			// *y = x
			Value *t = dyn_cast<LoadInst>(y);
			errs() << "interpret *y = x" << I << "\n";
			
		}

	}
	errs() << "left value"<<*y << "\n";
	errs() << "points to:" << "\n";
	for(setIter iter = ptInfo[y].begin(); iter != ptInfo[y].end(); iter++){
		errs() << **iter << "\n";
	}
	errs()<< "\n";
	out->point_to_info = ptInfo;
//	errs() << "Store info:\n";
//	Instruction *inst =&I;
//	errs() << inst->getOperand(0)->getName() << "\n";
//	errs() << inst->getOperand(1)->getName() << "\n";
//	errs() << *inst	<< "\n";
*/
}

void MAYPFlowFunction::print(){
	errs()<< "print function\n";
}

