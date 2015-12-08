#include "../../include/Worklist/WorklistAlg.h"
void WorklistAlg::test(){
	errs() << "testing\n";
}
void WorklistAlg::init(Function &F, FlowFunction* flowFunc, LatticeNode* beginNode){
	output_map.clear();
	predecessor.clear();
	finalMap.clear();
	while(!worklist.empty())
		worklist.pop();
	for(Function::iterator B = F.begin(); B != F.end(); B++){
		for(BasicBlock::iterator inst = B->begin(); inst != B->end(); inst++){
			predecessor[inst] = vector<Instruction*>();
			successor[inst] = vector<Instruction*>();
		}
	}




	for(Function::iterator B = F.begin(); B != F.end(); B++){
		for(BasicBlock::iterator inst = B->begin(); inst != B->end(); inst++){
//			errs() << *inst << "\n";
			worklist.push(inst);
			LatticeNode* node = new LatticeNode(beginNode->type, false, true);
//			find it's next node
			output_map[inst] = node;
			if(!inst->isTerminator()){
				BasicBlock::iterator next_inst = inst;
				next_inst++;
				predecessor[next_inst].push_back(inst);
				successor[inst].push_back(next_inst);
			}else{
				const TerminatorInst* terminator = B->getTerminator();
				int cnt = terminator->getNumSuccessors();
				for(int i = 0; i < cnt; i++){
					Instruction* next_inst = terminator->getSuccessor(i)->begin();
					predecessor[next_inst].push_back(inst);
					successor[inst].push_back(next_inst);
				}
			}
		}
	}
	// check the correctness;
//	errs() << "worklist algorithm initialize end\n";
//	for(map<Instruction*, vector<Instruction*> >::iterator iter = successor.begin(); iter != successor.end(); iter++){
//		errs() << *(iter->first) << "\n";
//		for(vector<Instruction*>::iterator it = iter->second.begin(); it != iter->second.end(); it++){
//			errs() << *(iter->first) << ":"<<*(*it) << "\n";
//		}
//	}
//	errs() << "predecessor check done\n";
}


map<Instruction*, LatticeNode*> WorklistAlg::Run_Worklist(Function &F, FlowFunction* flowFunc, LatticeNode* beginNode){
	init(F, flowFunc, beginNode);

	//errs() << "begin running worklist\n";

	while(!worklist.empty()){
		//errs() << "exec Instruction *inst = worklist.front(); \n";
		Instruction *inst = worklist.front();
		//errs() << "exec worklist.pop(); \n";
		worklist.pop();
		if(!matchFlowFunc(inst, flowFunc, beginNode)){
			//errs() << "add in worklist\n";
			for(vector<Instruction*>::iterator iter = successor[inst].begin(); iter != successor[inst].end(); iter++){
				worklist.push(*iter);
			}
		}
	}



	errs() << "---worklist algorithm output_map dump started---\n\n";
	//errs() << "out iteration\n";
	for(map<Instruction*, LatticeNode*>::iterator iter = output_map.begin(); iter != output_map.end(); iter++){
		//errs() << "in iteration\n";
		errs()<< *iter->first << "\n";
		//errs() << "in iteration 2222\n";

		// modified
		//iter->second->print()

		if (iter->second!=NULL){
			iter->second->print();
		}
	}
	errs() << "---worklist algorithm output_map dump finished---\n\n";
	// construct finalMap
	for(map<Instruction*, LatticeNode*>::iterator iter = output_map.begin(); iter != output_map.end(); iter++){
		Instruction* inst = iter->first;
		vector<Instruction*> preNodeList = predecessor[inst];
		vector<LatticeNode*> input;
		for(vector<Instruction*>::iterator iter = preNodeList.begin(); iter != preNodeList.end(); iter++){
			input.push_back(output_map[*iter]);
		}
		// if it is the entrance of the function
		if(input.size() == 0){
			finalMap[inst] = beginNode;
		}else{
			input.push_back(beginNode);
			LatticeNode* output = merge(input);
			finalMap[inst] = output;
		}
	}
	errs() << "---worklist algorithm finalmap dump started---\n\n";
	for(map<Instruction*, LatticeNode*>::iterator iter = finalMap.begin(); iter != finalMap.end(); iter++){
		errs() << *iter->first << "\n";

		// modified: iter->second->print();
		if (iter->second != NULL){
			iter->second->print();
		}

	}
	errs() << "---worklist algorithm finalmap dump finished---\n\n";
	return finalMap;
}

bool WorklistAlg::matchFlowFunc(Instruction* inst, FlowFunction* flowFunc, LatticeNode* beginNode){
	vector<Instruction*> predNodeList = predecessor[inst];
	vector<LatticeNode*> input;
	for(vector<Instruction*>::iterator iter = predNodeList.begin(); iter != predNodeList.end(); iter++){
		input.push_back(output_map[*iter]);
	}
	if(input.size() == 0){
		input.push_back(beginNode);
	}
	LatticeNode *new_output;

/*add more cases when needed*/
	if(isa<CSEFlowFunction>(flowFunc)){

//		errs() << "cast function to cse\n";
//		errs() << flowFunc->type;
		CSEFlowFunction *func = cast<CSEFlowFunction>(flowFunc);
		new_output = (*func)(inst, input);
	}
	if(isa<CPFlowFunction>(flowFunc)){

		  //errs() << "cast function to cp\n";
//		errs() << flowFunc->type;
		CPFlowFunction *func = cast<CPFlowFunction>(flowFunc);
		new_output = (*func)(inst, input);
		  //errs() << "casted\n";
	}


/*make judgement on whether there are difference between new output lattice and old one*/
	LatticeNode *old_output = output_map[inst];

	//errs() << "eq test\n";
	//errs() << (new_output == NULL) << '\n';
	//errs() << (old_output == NULL) << '\n' << '\n';
	// original version:
	//bool label = old_output->equal(new_output);

	// ZW modified
	bool label = false;
	if (old_output != NULL && new_output != NULL){
		label = old_output->equal(new_output);
	}

	//errs() << "eq tested \n";
	//errs() << "\n";

	if(!label){
		output_map[inst] = new_output;
	}
	return label;
}

LatticeNode* WorklistAlg::merge(vector<LatticeNode*> input){
	while(input.size() > 1){
		LatticeNode *node1 = input.back();
		input.pop_back();
		LatticeNode *node2 = input.back();
		input.pop_back();


		//LatticeNode *newNode = node1->join(node2);
		LatticeNode *newNode;
		if (node1 != NULL){
			newNode = node1->join(node2);
		}
		else{
			newNode = node2;
		}


		input.push_back(newNode);
	}
	return input.front();
}
