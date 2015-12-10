#include "../../include/Lattice/CPLatticeNode.h"

LatticeNode* CPLatticeNode::join(LatticeNode* node){

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
	//map<Value*, ConstantInt*> statements1 = this->statements;
  map<Value*, int> data_info1 = this->data_info;

  map<Value*, int> data_info2 = cpNode->data_info;
	map<Value*, int> new_data_info;

	for(map<Value*, int>::iterator iter = data_info1.begin(); iter != data_info1.end(); iter++){
		Value* var = iter->first;
    int c1 = iter->second;
    if (data_info2.count(var) > 0){
      int c2 = data_info2[var];
      if (c1 == c2){
        new_data_info[var] = c1;
      }
    }
	}
  CPLatticeNode* res = new CPLatticeNode(false, false, new_data_info);
  errs() << "joined\n";
  return res;
}


bool CPLatticeNode::equal(LatticeNode* node){


  if (node->isBottom || this->isBottom){
    return node->isBottom == this->isBottom;
  }
  if (node->isTop || this->isTop){
    return node->isTop == this->isTop;
  }

	CPLatticeNode* cpNode = dyn_cast<CPLatticeNode>(node);
	map<Value*, int> data_info1 = this->data_info;
	map<Value*, int> data_info2 = cpNode->data_info;

	for(map<Value*, int>::iterator iter = data_info1.begin(); iter != data_info1.end(); iter++){
	   Value* var = iter->first;
     int c1 = iter->second;
     if(data_info2.count(var) <= 0){
       return false;
     }
     else{
       int c2 = data_info2[var];
       if (c1 != c2){
         return false;
       }
     }
	}
	return true;
}

void CPLatticeNode::print(){

	errs() << "---CPLatticeNode Info---\n";
	errs() << "Bottom:" << this->isBottom << "  Top:" << this->isTop << "\n";
	for(map<Value*, int>::iterator iter = data_info.begin(); iter != data_info.end(); iter++){
     Value* var = iter->first;
     int c = iter->second;
     if(var->hasName()){
       errs() << var->getName() <<  " --> " << c << ", ";
     }
   }
    errs()<<"\n";
}
