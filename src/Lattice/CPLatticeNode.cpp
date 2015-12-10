#include "../../include/Lattice/CPLatticeNode.h"

LatticeNode* CPLatticeNode::join(LatticeNode* node){
  // added 1:32

  // in constant propogation, join is intersection
  errs() << "incoming join\n";
  if (node->isBottom || this->isTop){
    errs() << "joined\n";
    return this;
  }
  if (this->isBottom || node->isTop) {
    errs() << "joined\n";
    return node;
  }

	CPLatticeNode* cpNode = dyn_cast<CPLatticeNode>(node);
	map<Value*, ConstantInt*> statements1 = this->statements;
	map<Value*, ConstantInt*> statements2 = cpNode->statements;
	map<Value*, ConstantInt*> new_statements;

	for(map<Value*, ConstantInt*>::iterator iter = statements1.begin(); iter != statements1.end(); iter++){
		Value* var = iter->first;
    ConstantInt* c1 = iter->second;
    if (statements2.count(var) > 0){
      ConstantInt* c2 = statements2[var];
      if (c1 == c2){
        new_statements[var] = c1;
      }
    }
	}
  CPLatticeNode* res = new CPLatticeNode(false, false, new_statements);
  errs() << "joined\n";
  return res;
}


bool CPLatticeNode::equal(LatticeNode* node){
  //errs()<<"incoming equal test\n";

  if (node->isBottom || this->isBottom){
    //errs() << "equal tested\n";
    return node->isBottom == this->isBottom;
  }
  if (node->isTop || this->isTop){
    //errs() << "equal tested\n";
    return node->isTop == this->isTop;
  }

	CPLatticeNode* cpNode = dyn_cast<CPLatticeNode>(node);
//	errs() << "CPLatticeNode equal function\n";
	map<Value*, ConstantInt*> statements1 = this->statements;
	map<Value*, ConstantInt*> statements2 = cpNode->statements;

	for(map<Value*, ConstantInt*>::iterator iter = statements1.begin(); iter != statements1.end(); iter++){
	   Value* var = iter->first;
     ConstantInt* c1 = iter->second;
     if(statements2.count(var) <= 0){
       //errs()<<"equal tested\n";
       return false;
     }
     else{
       ConstantInt* c2 = statements2[var];
       // if (c1!=c2)
       if (c1->getValue() != c2->getValue()){
         //errs()<<"equal tested\n";
         return false;
       }
     }
	}
	//errs() << "CPLatticeNode equal function\n";
	return true;
}

void CPLatticeNode::print(){

	errs() << "---CPLatticeNode Info---\n";
	errs() << "Bottom:" << this->isBottom << "  Top:" << this->isTop << "\n";
	for(map<Value*, ConstantInt*>::iterator iter = statements.begin(); iter != statements.end(); iter++){
     Value* var = iter->first;
     ConstantInt* c = iter->second;
     /*if (var->getName().size() < 10){
       errs() << var->getName().str();
     }
     else{
       errs() << var->getValueName();
     }*/
     if(var->hasName()){
       errs() << var->getName() <<  " --> " << c->getValue() << ", ";
     }
   }
    errs()<<"\n";

    //errs() << "CPLattice print\n";
}
