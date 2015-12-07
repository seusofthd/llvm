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
	map<Value*, ConstantInt*> statements1 = this->statements;
	map<Value*, ConstantInt*> statements2 = cpNode->statements;
	map<Value*, ConstantInt*> new_statements;

	for(map<Value*, ConstantInt*>::iterator iter = statements1.begin(); iter != statements1.end(); iter++){
		Value* var = iter->first;
    ConstInt* c1 = iter->second;
    if (statements2.count(var) > 0){
      ConstInt* c2 = statements2[var];
      if (c1 == c2){
        new_statements[var] = c1;
      }
    }
	}
  CPLatticeNode* res = new CPLatticeNode(false, false, new_statements);
  return new_node;
}


bool CPLatticeNode::equal(LatticeNode* node){
  if (node->isBottom || this->isBottom){
    return node->isBottom == this->isBottom;
  }
  if (node->isTop || this->isTop){
    return node->isTop == this->isTop;
  }

	CPLatticeNode* cpNode = dyn_cast<CPLatticeNode>(node);
//	errs() << "CPLatticeNode equal function\n";
	map<Value*, ConstantInt*> statements1 = this->statements;
	map<Value*, ConstantInt*> statements2 = cpNode->statements;

	for(map<Value*, ConstantInt*>::iterator iter = statements1.begin(); iter != statements1.end(); iter++){
	   Value* var = iter->first;
     ConstInt* c1 = iter->second;
     if(statements2.count(var) <= 0){
       return false;
     }
     else{
       ConstInt* c2 = statements2[var];
       if (c1 != c2){
         return false;
       }
     }
	}
//	errs() << "CPLatticeNode equal function\n";
	return true;
}

void CPLatticeNode::print(){
	errs() << "---CPLatticeNode Info---\n";
	errs() << "Bottom:" <<this->isBottom << "  Top:" << this->isTop << "\n";
	for(map<Value*, ConstantInt*>::iterator iter = statements.begin(); iter != statements.end(); iter++){
		errs() << iter->first << "->"<<*(iter->second)<< "(" <<iter->second<<")" << "\n";
	}
	errs() << "\n";
}
