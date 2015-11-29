#include "../../include/Lattice/CSELatticeNode.h"

LatticeNode* CSELatticeNode::join(LatticeNode* node){
	if (node->isBottom){
		return this;
	}

	if (this->isBottom){
		return node;
	}

	CSELatticeNode* cseNode = dyn_cast<CSELatticeNode>(node);
	map<Value*, Instruction*> statements1 = this->statements;
	map<Value*, Instruction*> statements2 = cseNode->statements;
	map<Value*, Instruction*> new_statements;

	for(map<Value*, Instruction*>::iterator iter = statements1.begin(); iter != statements1.end(); iter++){
		if(statements2.find(iter->first) != statements2.end()){
			if(statements2[iter->first] == statements1[iter->first])
				new_statements[iter->first] = iter->second;
		}
	}

	CSELatticeNode* new_node = new CSELatticeNode(false, false, new_statements);
	return new_node; 
}

bool CSELatticeNode::equal(LatticeNode* node){
	CSELatticeNode* cseNode = dyn_cast<CSELatticeNode>(node);
	errs() << "CSELatticeNode equal function\n";
	map<Value*, Instruction*> statements1 = this->statements;
	map<Value*, Instruction*> statements2 = cseNode->statements;

	if(this->isBottom == true || cseNode->isBottom == true)
		return this->isBottom == cseNode->isBottom;
	if(this->isTop == true || cseNode->isTop == true)
		return this->isTop == cseNode->isTop;
	
	for(map<Value*, Instruction*>::iterator iter = statements1.begin(); iter != statements1.end(); iter++){
		if(statements2.find(iter->first) != statements2.end()){
			if(statements2[iter->first] != statements1[iter->first])
				return false;
		}else{
			return false;
		}
	}
	errs() << "CSELatticeNode equal function\n";
	return true;
}

//void CSELatticeNode::print(){
//	if(this->isBottom == true)
//		errs() << "Bottom Lattice Node\n";
//	if(this->isTop == true)
//		errs() << "Top Lattice Node\n";
//	for(map<Value*, Instruction*>::iterator iter = statements.begin(); iter != statements.end(); iter++){
//		errs() << *(iter->first) << " " << *(iter->second) << "\n";
//	}
//}
