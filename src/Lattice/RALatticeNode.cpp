#include "../../include/Lattice/RALatticeNode.h"

LatticeNode* RALatticeNode::join(LatticeNode* node){
	if (node->isBottom){
		return this;
	}

	if (this->isBottom){
		return node;
	}

	RALatticeNode* raNode = dyn_cast<RALatticeNode>(node);
	map<Value*, ConstantRange*> RangeMap1 = this->RangeMap;
	map<Value*, ConstantRange*> RangeMap2 = raNode->RangeMap;
	map<Value*, ConstantRange*> new_RangeMap;

        set<Value *> key_set;
        for (std::map<Value*, ConstantRange*>::iterator iter = RangeMap1.begin(); iter != RangeMap1.end(); ++iter){
                Value* key_tmp = iter->first;
                key_set.insert(key_tmp);
          }
        for (std::map<Value*, ConstantRange*>::iterator iter = RangeMap2.begin(); iter != RangeMap2.end(); ++iter){
                Value* key_tmp = iter->first;
                key_set.insert(key_tmp);
          }
          for (std::set<Value *>::iterator iter = key_set.begin(); iter != key_set.end(); ++iter){
                if (RangeMap1.count(*iter) > 0 && RangeMap2.count(*iter) > 0){
                        ConstantRange* UpdatedRange = new ConstantRange(32,true);
                        ConstantRange* Range1 = RangeMap1[*iter];
                        ConstantRange* Range2 = RangeMap2[*iter];
                        *UpdatedRange = Range1->unionWith(*Range2);
                        new_RangeMap[*iter] = UpdatedRange;
                  }
                  else if (RangeMap1.count(*iter) > 0 && RangeMap2.count(*iter) == 0){
                            new_RangeMap[*iter] = RangeMap1[*iter];
                  }
                  else if (RangeMap1.count(*iter) == 0 && RangeMap2.count(*iter) > 0){
                                new_RangeMap[*iter] = RangeMap2[*iter];
                  }
          }

	RALatticeNode* new_node = new RALatticeNode(false, false, new_RangeMap);
	return new_node;
}


bool RALatticeNode::equal(LatticeNode* node){
	RALatticeNode* raNode = dyn_cast<RALatticeNode>(node);
	//errs() << "RALatticeNode equal function\n";
	map<Value*, ConstantRange*> RangeMap1 = this->RangeMap;
	map<Value*, ConstantRange*> RangeMap2 = raNode->RangeMap;

	if(this->isBottom == true || raNode->isBottom == true)
		return this->isBottom == raNode->isBottom;
	if(this->isTop == true || raNode->isTop == true)
		return this->isTop == raNode->isTop;
	
	for(map<Value*, ConstantRange*>::iterator iter = RangeMap1.begin(); iter != RangeMap1.end(); iter++){
		if(RangeMap2.find(iter->first) != RangeMap2.end()){
			if(*RangeMap2[iter->first] != *RangeMap1[iter->first])
				return false;
		}else{
			return false;
		}
	}
//	errs() << "RALatticeNode equal function\n";
	return true;
}

void RALatticeNode::print(){
	//OS = new raw_ostream();
	errs() << "---RALatticeNode Info---\n";
	errs() << "Bottom:" <<this->isBottom << "  Top:" << this->isTop << "\n";
	for(map<Value*, ConstantRange*>::iterator iter = RangeMap.begin(); iter != RangeMap.end(); iter++){
		//errs() << iter->first->dump() << "->" ;
	        iter->first->dump();		
		iter->second->dump();
                errs() << "\n";
	}
	errs() << "\n";
}

std::vector<Value *> RALatticeNode::differInRange(RALatticeNode* in){
  std::map<Value*, ConstantRange*> RangeMap1 = this->RangeMap;
  std::map<Value*, ConstantRange*> RangeMap2 = in->RangeMap;
  
  std::set<Value *> key_set;
  for (std::map<Value*, ConstantRange*>::iterator it=RangeMap1.begin(); it!=RangeMap1.end(); ++it){
    Value* elm = it->first;
    key_set.insert(elm);
  }
  for (std::map<Value*, ConstantRange*>::iterator it=RangeMap2.begin(); it!=RangeMap2.end(); ++it){
    Value* elm = it->first;
    key_set.insert(elm);
  }
  std::vector<Value *> result;
  for (std::set<Value *>::iterator it = key_set.begin(); it != key_set.end(); ++it){
    if (RangeMap1.count(*it) > 0 && RangeMap2.count(*it) > 0){
      ConstantRange* c1 = RangeMap1[*it];
      ConstantRange* c2 = RangeMap2[*it];
      if (*c1 != *c2) {
        result.push_back(*it);
      }
    }
    else{
      result.push_back(*it);
    }
  }
  return result;
}
