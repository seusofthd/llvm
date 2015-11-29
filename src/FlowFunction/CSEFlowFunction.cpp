#include "../../include/FlowFunction/CSEFlowFunction.h"
typedef vector<LatticeNode*>::iterator laIter;
typedef map<Value*, Instruction*>::iterator mapIter;
LatticeNode* CSEFlowFunction::operator()(Instruction *inst, vector<LatticeNode*> input){
	errs() << "start the CSEFlowFunction (): input size:" << input.size() << "\n";
	in = merge(input);
	map<Value*, Instruction*> stmt = in->statements;
	out = new CSELatticeNode(false, false, stmt);
	visit(inst);
	errs() << "after visiting, statement size:" << out->statements.size() << "\n";
	LatticeNode* cast_out = out;
	return cast_out;
}

void CSEFlowFunction::visitBinaryOperator(BinaryOperator &I){
	errs() << "visit binary operation\n";
	Value *leftVal = &I;
	if(out->statements.find(leftVal) != out->statements.end()){
		return;
	}
	errs() << "not find the statement\n";	
	Instruction* newInst = &I;
	for(mapIter iter = out->statements.begin(); iter != out->statements.end(); iter++){
		Instruction* expr = iter->second;
		errs() << "iterate map:" << *expr << *newInst << "\n";
		if(expr->isIdenticalTo(newInst)){
			errs() << "identical statements:" << *expr <<":" << *newInst << "\n";
			out->statements[leftVal] = expr; 
		}
	}	
	
	if(out->statements.find(leftVal) == out->statements.end()){
		errs() << "add new statement\n";
		out->statements[leftVal] = newInst;
		errs() << *leftVal << ":" << *newInst << "\n";
	}
}

/*I don't know whether this kind of writing will invoke the LatticeNode join function or CSELatticeNode join function*/
CSELatticeNode* CSEFlowFunction::merge(vector<LatticeNode*> input){
	while(input.size() > 1){
		LatticeNode *node1 = input.back();
		input.pop_back();
		LatticeNode *node2 = input.back();
		input.pop_back();
		LatticeNode *newNode = node1->join(node2);
		input.push_back(newNode);
	}
	CSELatticeNode * cseNode = dyn_cast<CSELatticeNode>(input.front());
	return cseNode;
}
