#include "../../include/Lattice/CPLatticeNode.h"

LatticeNode* CPLatticeNode::join(LatticeNode* node){
  // in constant propogation, join is intersection
  if (in->isBottom || this->isTop){
    return this;
  }
  if (this->isBottom || in->isTop) {
    return in;
  }

	CPLatticeNode* cpNode = dyn_cast<CPLatticeNode>(node);
	map<Value*, Instruction*> statements1 = this->statements;
	map<Value*, Instruction*> statements2 = cpNode->statements;
	map<Value*, Instruction*> new_statements;

	for(map<Value*, Instruction*>::iterator iter = statements1.begin(); iter != statements1.end(); iter++){
		if(statements2.find(iter->first) != statements2.end()){
			if(statements2[iter->first] == statements1[iter->first])
				new_statements[iter->first] = iter->second;
		}
	}

	CPLatticeNode* new_node = new CPLatticeNode(false, false, new_statements);
	return new_node;
}

bool CPLatticeNode::equal(LatticeNode* node){
	CPLatticeNode* cpNode = dyn_cast<CPLatticeNode>(node);
//	errs() << "CPLatticeNode equal function\n";
	map<Value*, Instruction*> statements1 = this->statements;
	map<Value*, Instruction*> statements2 = cpNode->statements;

	if(this->isBottom == true || cpNode->isBottom == true)
		return this->isBottom == cpNode->isBottom;
	if(this->isTop == true || cpNode->isTop == true)
		return this->isTop == cpNode->isTop;

	for(map<Value*, Instruction*>::iterator iter = statements1.begin(); iter != statements1.end(); iter++){
		if(statements2.find(iter->first) != statements2.end()){
			if(statements2[iter->first] != statements1[iter->first])
				return false;
		}else{
			return false;
		}
	}
//	errs() << "CPLatticeNode equal function\n";
	return true;
}

void CPLatticeNode::print(){
	errs() << "---CPLatticeNode Info---\n";
	errs() << "Bottom:" <<this->isBottom << "  Top:" << this->isTop << "\n";
	for(map<Value*, Instruction*>::iterator iter = statements.begin(); iter != statements.end(); iter++){
		errs() << iter->first << "->"<<*(iter->second)<< "(" <<iter->second<<")" << "\n";
	}
	errs() << "\n";
}
